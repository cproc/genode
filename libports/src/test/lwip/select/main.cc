/* Genode includes */
#include <pthread.h>

#include <base/printf.h>
#include <base/thread.h>
#include <util/string.h>
#include <base/sleep.h>
#include <assert.h>
#include <base/env.h>
#include <timer_session/connection.h>
#include <os/config.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>



/***********************************
  Constant
 ***********************************/
const int MONITOR_INTERVAL     = 5;
const int SERVER_PORTNUM       = 10000;

const int BUFFER_SIZE          = ( 1024 + 16 );
const int ITEM_PACKET_SIZE     = 16;
const int RESPONSE_PACKET_SIZE = 16;

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
void send( int conn, Packet_type packet_type, int size, char * data );
int  recv( int conn, int size, char * buffer );
void print_packet_statistics();

void doServer( char * ip );
void doClient( char * ip );

/*************************************
  packet functions
 *************************************/
typedef void (*Packet_callback)(int conn, Packet_header packet_header );

void print_message(int conn, Packet_header packet_header );
void request( int conn, Packet_header packet_header );
void response(int conn, Packet_header packet_header );

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

    if (pthread_create( &thread_id,
                        0, 
                        server_connector,
                        (void*)server ) !=0 ) 
    {
        Genode::printf("error: pthread_create() failed\n");
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
        return 0;
    }

    PDBG("Connect to server ... [%s:%d]", ip, port );
    addr.sin_port        = htons( port );
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr( ip );

    if((connect( conn , (struct sockaddr *)&addr, sizeof(addr))) < 0)
    {
        PERR("Could not connect!");
        close(conn);
        return 0;
    }
#if 0
    if (pthread_create( &thread_id,
                0, 
                listener,
                (void*)conn ) !=0 ) 
    {
        Genode::printf("error: pthread_create() failed\n");
        return 0;
    }
#endif

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
        if(client < 0) 
        {
            PINF("Invalid socket from accept!");
            continue;
        }

        if (pthread_create( &thread_id,
                            0, 
                            listener,
                            (void*)client ) !=0 ) 
        {
            Genode::printf("error: pthread_create() failed\n");
            return 0;
        }
    }

    (void)closeServer( server );

    return 0;
}

void *listener(void *argv )
{
    Packet_header   packet_header;
    int             recv_len;
    int             conn = (int)argv;
    int             readsocket;
    fd_set          socks;

    PINF( "Establish connection : %d", conn );

    while( 1 )
    {                             
        FD_ZERO(&socks);
        FD_SET( conn, &socks);
        /*
        readsocket = select( 10, &socks, 0,0, NULL );
        if( readsocket <= 0 )
        {
            continue;
        }
        */

        /* receive packet type & size */
        recv_len = recv( conn, sizeof(Packet_header), (char*)&packet_header );
        if( recv_len < 0 )
        {
            /* Disconnected */
            break;
        }

        if( packet_header.type < PACKET_TYPE_COUNT )
        {
            Packet_statistics_recv[ packet_header.type ] ++;
            packet_function_map[ packet_header.type ]( conn, packet_header );
            Packet_statistics_proc[ packet_header.type ] ++;
        }
        else
        {
            PINF("RECV:%d [%d]%d,%d\n",conn, recv_len, packet_header.type, packet_header.size );
            assert(0);
        }
    }

    PINF( "Disconnected : %d", conn );

    return 0;
}

void send( int conn, Packet_type packet_type, int size, char * data )
{
    Packet_header packet_header;

    /* send header */
    packet_header.type = packet_type;
    packet_header.size = size;
    send(conn, &packet_header, sizeof( Packet_header ), 0 );

    Packet_statistics_send[ packet_header.type ] ++;

    /* send data */
    if( size > 0 )
    {
        send(conn, data, size , 0 );
    }
}
int  recv( int conn, int size, char * buffer )
{
    int               recv_len = 0; 
    int               recv_val; 

    assert( size > 0 );

    while( recv_len < size )
    {
        recv_val = recv( conn, 
                         buffer + recv_len, 
                         size - recv_len, 
                         0 );
        if( recv_val < 0 )
        {
            return -1;
        }
        recv_len += recv_val;
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
        Genode::printf( "%20s : recv( %d / %d ) send( %d )\n",
                        packet_type_name[ i ],
                        Packet_statistics_recv[ i ],
                        Packet_statistics_proc[ i ],
                        Packet_statistics_send[ i ] );
    }
    Genode::printf( "\n\n" );
}

void print_message(int conn, Packet_header packet_header )
{
    char recv_buffer[ BUFFER_SIZE ];
    int  recv_len; 

    recv_len = recv(conn, packet_header.size, recv_buffer );

    Genode::printf("[%d]", recv_len );
    for( int i = 0 ; i < recv_len ; i ++)
    {
        Genode::printf("%x", (recv_buffer[ i ] >> 4 ) & 15 );
        Genode::printf("%x ", (recv_buffer[ i ] >> 0 ) & 15 );
    }
    Genode::printf("\n" );
    Genode::printf("[%d]%s\n", recv_len, recv_buffer );
}

void request(int conn, Packet_header packet_header )
{
    char              recv_buffer[ BUFFER_SIZE ];
    int               recv_len; 

    assert( node_type == NODE_TYPE_SERVER );

    /* query 수신 */
    recv_len = recv(conn, packet_header.size, recv_buffer );
    if( recv_len < 0 )
    {
        /* Disconnected */
        return;
    }
    assert( recv_len == ITEM_PACKET_SIZE );

//    /* echo */
//    send( conn, PACKET_TYPE_RESPONSE, recv_len, recv_buffer );
}

void response(int conn, Packet_header packet_header )
{
    char              recv_buffer[ BUFFER_SIZE ];
    int               recv_len; 

    assert( node_type == NODE_TYPE_CLIENT );

    /* query 수신 */
    recv_len = recv(conn, packet_header.size, recv_buffer );
    if( recv_len < 0 )
    {
        /* Disconnected */
        return;
    }

    assert( recv_len == ITEM_PACKET_SIZE );
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

    while( 1 )
    {
        timer.msleep( MONITOR_INTERVAL * 1000);

        Genode::printf("Server      :\n" );
        print_packet_statistics();

        Genode::printf("Performance : %d\n\n",
                ( Packet_statistics_proc[ PACKET_TYPE_REQUEST ] 
                  - prev_packet_count ) / MONITOR_INTERVAL );
        prev_packet_count = Packet_statistics_proc[ PACKET_TYPE_REQUEST ];
    }
}

void *send_request_packet(void *argv )
{
    int                conn =(int)argv;
    char               send_buffer[ BUFFER_SIZE ];
    int                i = 0;

    for( /*i = 0*/ ; /*i < 1000000*/ ; /*i ++*/ )
    {
        send( conn, 
              PACKET_TYPE_REQUEST,
              ITEM_PACKET_SIZE,
              send_buffer );
    }
    PINF("DONE");

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

    if (pthread_create( &thread_id,
                0, 
                send_request_packet,
                (void*)conn ) !=0 ) 
    {
        Genode::printf("error: pthread_create() failed\n");
        return;
    }

    while( 1 )
    {
        timer.msleep( MONITOR_INTERVAL * 1000);

        Genode::printf("Client     :\n" );
        print_packet_statistics();

        Genode::printf("Performance : %d\n\n",
                ( Packet_statistics_send[ PACKET_TYPE_REQUEST ] 
                  - prev_packet_count ) / MONITOR_INTERVAL );
        prev_packet_count = Packet_statistics_send[ PACKET_TYPE_REQUEST  ];
    }
}

int main()
{
    unsigned int      job;
    char              my_ip[16];
    char              server_ip[16];

    Genode::Xml_node argv_node = Genode::config()->xml_node().sub_node("argv");
    try 
    {
        argv_node.attribute("I"         ).value(my_ip,     sizeof(my_ip));
        argv_node.attribute("server_ip" ).value(server_ip, sizeof(server_ip));

        argv_node.attribute("job").value( &job );
        node_type = (Node_type)job;
    } 
    catch(...) 
    {
    }

    switch( node_type )
    {
        case NODE_TYPE_SERVER:
            doServer( (char*)server_ip );
            break;
        case NODE_TYPE_CLIENT:
            doClient( (char*)server_ip );
            break;
    }

    return 0;
}

