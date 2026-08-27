#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *filename, char **exec_argv);
void fork_and_exec(char *path, char **exec_argv);

int main(int argc, char **argv)
{
	if (!((argc == 3) || (argc >= 5 && strcmp("-exec", argv[3]) == 0))) {
		fprintf(2, "Usage: find <directory> <filename>\n");
		exit(1);
	}

	if (argc == 3)
		find(argv[1], argv[2], 0);
	else
		find(argv[1], argv[2], &argv[4]);

	exit(0);
}

void find(char *path, char *filename, char **exec_argv)
{
	int fd;
	struct stat st;
	struct dirent de;
	char buf[512], *p;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat");
		close(fd);
		return;
	}

	switch (st.type) {
		case T_FILE:
			for(p=path+strlen(path); p >= path && *p != '/'; p--)
				;
			p++;
			if (strcmp(p, filename) == 0) {
				if (exec_argv == 0)
					printf("%s\n", path);
				else
					fork_and_exec(path, exec_argv);
			}
			break;
		case T_DIR:
			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)) {
				if(de.inum == 0)
					continue;
				if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
					continue;
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = '\0';
				find(buf, filename, exec_argv);
			}
	}
	close(fd);
}

int count_args(char **argv)
{
	int count = 0;
	while (argv[count] != 0)
	count++;
	return count;
}

void fork_and_exec(char *path, char **exec_argv)
{
	int num_args = count_args(exec_argv);
	char **new_argv = malloc((num_args + 2) * sizeof(char*));
	if (!new_argv)
		exit(1);

	int i = 0;
	for (; i < num_args; i++) {
		new_argv[i] = exec_argv[i];
	}
	new_argv[i++] = path;
	new_argv[i] = 0;
	int pid = fork();
	if (pid < 0)
		return;

	if (pid == 0) {
		exec(new_argv[0], new_argv);
		free(new_argv);
		exit(1);
	}
	else {
		wait(0);
		free(new_argv);
	}
}
