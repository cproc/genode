#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int f;
    char *t;

    f = open ( "test0" , O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR );

    t = (char*)malloc(128*1024);

    write(f, t, 128*1024);
    read(f, t, 128*1024);
    close(f);

    free(t);

    return 0;
}
