#define SELECT 1

#ifndef NOT_GENODE
/* Genode includes */
#include <base/printf.h>
#include <base/thread.h>
#include <util/string.h>
#include <base/sleep.h>
#include <base/env.h>
#include <timer_session/connection.h>
#include <os/config.h>

#include <lwip/genode.h>

#else
#define PDBG(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
#define PINF(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)
#define PLOG(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)
#define PERR(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
#endif /* NOT_GENODE */

/* libc includes */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>

#ifndef NOT_GENODE
/*
namespace Fiasco {
#include <l4/sys/ktrace.h>
}

# define TBUF_LOG(s) Fiasco::fiasco_tbuf_log(s)
*/
# define TBUF_LOG(s)
#else
# define TBUF_LOG(s)
#endif

#ifdef NOT_GENODE
namespace Timer {
	class Connection
	{
		public:
			void msleep(int ms) { sleep(ms/1000); }
	};
}

namespace Genode {
	static void sleep_forever() { }
}
#endif

/***********************************
  Constant
 ***********************************/
const int MONITOR_INTERVAL     = 5;
const int SERVER_PORTNUM       = 10000;

const int BUFFER_SIZE          = ( 32768 + 16 );
const int ITEM_PACKET_SIZE     = 2048;
const int RESPONSE_PACKET_SIZE = 16;

const int Numpackets           = 2048;

int packetsize;

typedef enum Node_type
{
	NODE_TYPE_SERVER,
	NODE_TYPE_CLIENT
} Node_type;

Node_type   node_type;

/*************************************
  for network
 *************************************/
typedef enum Packet_type
{
	PACKET_TYPE_PRINT_MESSAGE,
	PACKET_TYPE_REQUEST,
	PACKET_TYPE_RESPONSE,
	PACKET_TYPE_COUNT
} Packet_type;

typedef struct Packet_header
{
	int type;
	int size;
	char data[16384];
} Packet_header;

int  Packet_statistics_recv[ PACKET_TYPE_COUNT ] = {0};
int  Packet_statistics_proc[ PACKET_TYPE_COUNT ] = {0};
int  Packet_statistics_send[ PACKET_TYPE_COUNT ] = {0};

/*************************************
  Declare function
 *************************************/
int openServer(char * ip, int port );
int closeServer(int conn);
int openClient(char * ip, int port);
int closeClient(int conn);
void *server_connector(void *argv );
void *listener(void *argv );
static int _send( int conn, Packet_type packet_type, int size, char * data );
static int  _recv( int conn, int size, char * buffer );
void print_packet_statistics();

void doServer( char * ip );
void doClient( char * ip );

/*************************************
  packet functions
 *************************************/
typedef int (*Packet_callback)(int conn, Packet_header packet_header );

int print_message(int conn, Packet_header packet_header );
int request( int conn, Packet_header packet_header );
int response(int conn, Packet_header packet_header );

Packet_callback packet_function_map[]={
	print_message,
	request,
	response
};

/*************************************
  network functions
 *************************************/
int openServer(char * ip, int port )
{
	pthread_t          thread_id;
	struct sockaddr_in in_addr;
	int                server;

	PLOG("Create new socket ...");
	if((server = socket(AF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/)) < 0) {
		PERR("No socket available!");
		return -1;
	}

	PLOG("Now, I will bind ...");
	in_addr.sin_family      = AF_INET;
	in_addr.sin_port        = htons( port );
	in_addr.sin_addr.s_addr = inet_addr( ip );
	if(bind(server, (struct sockaddr*)&in_addr, sizeof(in_addr))) {
		PERR("bind failed!");
		return -1;
	}

	PINF("Server - [%s:%d]", ip, port );

	PLOG("Now, I will listen ...");
	if(listen(server, 5)) {
		PERR("listen failed!");
		return -1;
	}

	PLOG("Start the server loop ... %d", server);

	if (pthread_create( &thread_id, 0, server_connector, (void*)server ) !=0 )
	{
		printf("error: pthread_create() failed\n");
		return -1;
	}

	return 0;
}

int closeServer(int conn)
{
	close( conn );

	return 0;
}

int openClient(char * ip, int port)
{
	int                 conn;
	struct sockaddr_in  addr;
	pthread_t           thread_id;

	PDBG("Create new socket ...");
	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( conn < 0 )
	{
		PERR("No socket available!");
		return -1;
	}

	PDBG("Connect to server ... [%s:%d]", ip, port );
	addr.sin_port        = htons( port );
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr( ip );

	if((connect( conn , (struct sockaddr *)&addr, sizeof(addr))) < 0)
	{
		PERR("Could not connect!");
		close(conn);
		return -1;
	}

	if (pthread_create( &thread_id,
			    0,
			    listener,
			    (void*)conn ) !=0 )
	{
		printf("error: pthread_create() failed\n");
		return 0;
	}


	return conn;
}

int closeClient(int conn)
{
	close( conn );

	return 0;
}

void *server_connector(void *argv )
{
	pthread_t          thread_id;
	struct sockaddr    addr;
	socklen_t          len = sizeof(addr);
	int                client;
	int                server = (int)argv;

	PINF( "server : %d", server );

	while(true)
	{
		PINF( "wait..." );
		client = accept(server, &addr, &len);
		PINF("Client socket:%d\n",client );
		if(client < 0) {
			PINF("Invalid socket from accept!");
			continue;
		} else {
			if (pthread_create( &thread_id, 0, listener, (void*)client ) !=0 ) {
				printf("error: pthread_create() failed\n");
				return 0;
			}
		}
		PDBG("create client thread: %d", client);
	}

	(void)closeServer( server );

	return 0;
}

void *listener(void *argv )
{
	Packet_header   packet_header;
	int             recv_len;
	int             conn = (int)argv;
	size_t count = 0;
	fd_set          socks;
	char buffer[ITEM_PACKET_SIZE];

	PINF( "Establish connection : %d", conn );

	while (1) {
		/* receive packet type & size */
		recv_len = _recv( conn, sizeof(Packet_header), (char*)&packet_header );
		if( recv_len <= 0 ) {
			/* Disconnected */
			PERR("_recv() <= 0, break");
			break;
		}

		if (recv_len != sizeof (Packet_header)) {
			PERR("recv_len too small: %d", recv_len);
			break;
		}

		if (packet_header.type < PACKET_TYPE_COUNT) {
			Packet_statistics_recv[ packet_header.type ] ++;
			/* receive data */
//			recv_len = packet_function_map[packet_header.type](conn, packet_header);

			Packet_statistics_proc[ packet_header.type ] ++;
		} else {
			PINF("RECV:%d [%d]%d,%d\n",conn, recv_len, packet_header.type, packet_header.size );
			assert(0);
		}

		PDBG("count: %d, recv_len: %d", ++count, recv_len);
	}

	PINF( "Disconnected : %d", conn );
	close(conn);

	return 0;
}

static int _send( int conn, Packet_type packet_type, int size, char * data )
{
	static int count = 0;
	ssize_t nbytes1, nbytes2;
	Packet_header packet_header;

	/* send header */
	packet_header.type = packet_type;
	packet_header.size = size;
	nbytes1 = send(conn, &packet_header, sizeof( Packet_header ), 0 );
	if (nbytes1 <= 0)
		return nbytes1;

	Packet_statistics_send[ packet_header.type ] ++;

	/* send data */
	/*
	nbytes2 = send(conn, data, size , 0 );
	if (nbytes2 <= 0) {
		PDBG("send(): <= 0");
		return nbytes2;
	}
	*/

	PDBG("count: %d", ++count);

	return nbytes1;//+ nbytes2;
}

static int  _recv( int conn, int size, char * buffer )
{
	int               recv_len = 0;
	int               recv_val;
	size_t count = 0;

	assert(size > 0);

	while(recv_len < size) {
		recv_val = recv( conn, buffer + recv_len, size - recv_len, 0 );

		if (recv_val == 0)
			break;
		else if (recv_val == -1)
			return -1;

		recv_len += recv_val;
		count++;
	}

	return recv_len;
}

void print_packet_statistics()
{
	static const char packet_type_name[][ 64 ] = {
		"PRINT_MESSAGE",
		"REQUEST",
		"RESPONSE" };
	int i;

	for( i = 0 ; i < PACKET_TYPE_COUNT ; i ++ )
	{
		printf( "%20s : recv( %d / %d ) send( %d )\n",
				packet_type_name[ i ],
				Packet_statistics_recv[ i ],
				Packet_statistics_proc[ i ],
				Packet_statistics_send[ i ] );
	}
	printf( "\n\n" );
}

/*
 * callback functions
 */
int print_message(int conn, Packet_header packet_header )
{
	char recv_buffer[ BUFFER_SIZE ];
	int  recv_len;

	recv_len = _recv(conn, packet_header.size, recv_buffer );

	printf("[%d]", recv_len );
	for( int i = 0 ; i < recv_len ; i ++)
	{
		printf("%x", (recv_buffer[ i ] >> 4 ) & 15 );
		printf("%x ", (recv_buffer[ i ] >> 0 ) & 15 );
	}
	printf("\n" );
	printf("[%d]%s\n", recv_len, recv_buffer );

	return 0;
}

int request(int conn, Packet_header packet_header )
{
	char              recv_buffer[ BUFFER_SIZE ];
	int               recv_len;

	assert(node_type == NODE_TYPE_SERVER);

	recv_len = _recv(conn, packet_header.size, recv_buffer );
	if(recv_len <= 0) {
		return recv_len;
	}

	assert(recv_len == ITEM_PACKET_SIZE);
	return recv_len;
}

int response(int conn, Packet_header packet_header )
{
	char              recv_buffer[ BUFFER_SIZE ];
	int               recv_len;

	assert( node_type == NODE_TYPE_CLIENT );

	recv_len = _recv(conn, packet_header.size, recv_buffer );
	if( recv_len <= 0 ) {
		return recv_len;
	}

	assert( recv_len == ITEM_PACKET_SIZE );
	return recv_len;
}

void doServer( char * ip )
{
	Timer::Connection timer;
	int               prev_packet_count = 0;
	int               conn ;

	conn = openServer( ip, SERVER_PORTNUM );
	if( conn < 0 )
	{
		return;
	}

	Genode::sleep_forever();
	/*
	while( 1 )
	{
		timer.msleep( MONITOR_INTERVAL * 1000);

		printf("Server      :\n" );
		print_packet_statistics();

		printf("Performance : %d\n\n",
			       ( Packet_statistics_proc[ PACKET_TYPE_REQUEST ]
				 - prev_packet_count ) / MONITOR_INTERVAL );
		prev_packet_count = Packet_statistics_proc[ PACKET_TYPE_REQUEST ];
	}
	*/
}

void *send_request_packet(void *argv )
{
	int                conn =(int)argv;
	char               buffer[ ITEM_PACKET_SIZE ];
	int                i = 0;
	ssize_t nbytes, pbytes;

	pbytes = sizeof (Packet_header) + ITEM_PACKET_SIZE;

	PINF("SEND REQUESTS: %d", Numpackets);
	for( i = 0 ; i < Numpackets ; i ++ )
	{
		nbytes = _send( conn, PACKET_TYPE_REQUEST, ITEM_PACKET_SIZE, buffer );
		if (nbytes <= 0)
			break;
		/*
		if (nbytes != pbytes) {
			PERR("_send(): %d != %d", nbytes, pbytes);
			break;
		}
		*/
	}
	PINF("SEND REQUEST DONE: %d from: %d, val: %d", i, Numpackets, nbytes);

	closeClient(conn);

	Genode::sleep_forever();

	return NULL;

}

void doClient( char * ip )
{
	Timer::Connection  timer;
	int                prev_packet_count = 0;
	int                conn ;
	pthread_t          thread_id;

	/* wait for starting server */
	timer.msleep(1000);

	conn = openClient( ip, SERVER_PORTNUM );
	if( conn < 0 )
	{
		return;
	}

	if (pthread_create( &thread_id, 0, send_request_packet, (void*)conn ) !=0 )
	{
		printf("error: pthread_create() failed\n");
		return;
	}

	Genode::sleep_forever();
/*
	while( 1 )
	{
		timer.msleep( MONITOR_INTERVAL * 1000);

		printf("Client     :\n" );
		print_packet_statistics();

		printf("Performance : %d\n\n",
			       ( Packet_statistics_send[ PACKET_TYPE_REQUEST ]
				 - prev_packet_count ) / MONITOR_INTERVAL );
		prev_packet_count = Packet_statistics_send[ PACKET_TYPE_REQUEST  ];
	}
*/
}

int main(int argc, char *argv[])
{
	unsigned int      job;
	char              my_ip[16];
	char              server_ip[16];

#ifndef NOT_GENODE
	/* Initialize network stack and do DHCP */
	if (lwip_nic_init(0, 0, 0)) {
		PERR("We got no IP address!");
		return -1;
	}

	Genode::Xml_node argv_node = Genode::config()->xml_node().sub_node("argv");
	try
	{
		argv_node.attribute("I"         ).value(my_ip,     sizeof(my_ip));
		argv_node.attribute("server_ip" ).value(server_ip, sizeof(server_ip));
	//	argv_node.attribute("packetsize" ).value(&packetsize);

		argv_node.attribute("job").value( &job );
		node_type = (Node_type)job;
	}
	catch(...)
	{
	}
#else
	if (argc < 3) {
		printf("usage: %s <ip> <type>\n", argv[0]);
		return 1;
	}

	node_type = (Node_type) atoi(argv[2]);
	strncpy(server_ip, argv[1], sizeof (server_ip) - 1);

#endif
	switch( node_type )
	{
		case NODE_TYPE_SERVER:
			doServer( (char*)server_ip );
			break;
		case NODE_TYPE_CLIENT:
			doClient( (char*)server_ip );
			break;
		default:
			PERR("wrong node_type: %d", node_type);
			break;
	}

	return 0;
}
