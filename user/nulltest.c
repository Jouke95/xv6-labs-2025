#include "kernel/types.h"
#include "user/user.h"

int main()
{
    printf("Test voor null-pointer:\n");

    int x = *(volatile int*)0;
    printf("gelezen waarde: %d\n", x);
    printf("pte van pagina 0: %p\n", (void*)pgpte((void*)0));

    for (int i = 0; i < 6; i++)
        printf("pagina %d: pte %p\n", i, (void*)pgpte((void*)(uint64)(i * 4096)));

    exit (0);
}