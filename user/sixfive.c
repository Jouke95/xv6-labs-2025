#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0) {
			exit(1);
		}
		int total_number = 0;
		int in_number = 0;
		while(1) {
			char buf;
			int bytes_read = read(fd, &buf, 1);
			if (bytes_read <= 0) {
				break;
			}
			if (buf >= '0' && buf <= '9') {
				in_number = 1;
				int number = buf - '0';
				total_number = total_number * 10 + number;
			}
			else if (strchr(" -\r\t\n./,", buf)) {
				if ((total_number % 6 == 0 || total_number % 5 == 0) && in_number)
					printf("%d\n", total_number);
				total_number = 0;
				in_number = 0;
			}
		}
		if ((total_number % 6 == 0 || total_number % 5 == 0) && in_number)
			printf("%d\n", total_number);
	}
	return (0);
}