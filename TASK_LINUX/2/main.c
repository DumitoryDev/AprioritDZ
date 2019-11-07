#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int pipefds[2];
	pipe(pipefds);

	switch (pid = fork()) {

		case -1:
		{
			perror("fork");
			break;
		}

		case 0:
		{
			const char* msg = "hi daddy, I'm your child.";
			close(pipefds[0]);
			write(pipefds[1], msg, strlen(msg) + 1);
			close(pipefds[1]);
			
		}


		default:
		{
			
			const int max_size = 1024;
			char buff[max_size];
			const char* msg = "Hello.";
			close(pipefds[1]);

			if (read(pipefds[0], buff, (max_size - strlen(msg) - 1))) {

				strcat(buff, msg);
				FILE* file = fopen("tmp.txt", "w");

				if (file == NULL)
					printf("Error create file!");

				else {
					fwrite(&buff[0], sizeof(char), strlen(buff), file);
					fclose(file);
				}

			}

			close(pipefds[0]);
			break;
					
			
			
		}

	}

	return 0;
}
