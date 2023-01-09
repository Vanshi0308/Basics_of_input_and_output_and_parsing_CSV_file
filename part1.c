#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 256

int
main ( int argc, char *argv[] ) {
    char buf[MAX_BUFFER];
    int age;

    printf("Name >> ");
    scanf("%s", buf);
    printf("Age >> ");
    scanf("%d", &age);

    fprintf(stdout, "Hello %s (%d)\n", buf, age);
    
    return EXIT_SUCCESS;
}