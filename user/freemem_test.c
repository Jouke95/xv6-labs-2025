#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(void)
{
    printf("voor fork: %d\n", freemem());

    if (fork() == 0) {
        // child
        printf("in child: %d\n", freemem());
        exit(0);
    }

    wait(0);
    printf("na wait (child klaar): %d\n", freemem());

    exit(0);
}