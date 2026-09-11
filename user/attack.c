#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  int print_from_here = -1;

  char *start = sbrk(8*4096);
  for (int i = 0; i < (8*4096); i++) {
    if (memcmp(start + i, "This may help", 13) == 0) {
      print_from_here = i;
      break;
    }
  }
  printf("found at index %d\n", print_from_here);
  if (print_from_here != -1)
    printf("%s\n", &start[print_from_here + 16]);
  else
    printf("Niks gevonden helaas.\n");

  exit(1);
}
