#include "CopyFile.h"

void copy(const char* src, const char* dest) {

	if (src == NULL || dest == NULL) {

		printf("File path is empty!");
		return;

	}

	int in_file = 0, out_file = 0;

	if ((in_file = open(src, O_RDONLY)) == -1) {

		perror("Error open src!");
		return;

	}

	if ((out_file = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {

		close(in_file);
		perror("Error open dest!");
		return;

	}


	struct stat statbuf;
	stat(src, &statbuf);

	if (sendfile(out_file, in_file, NULL, statbuf.st_size) == -1)
		perror("Error copy file!");


	close(in_file);
	close(out_file);
	printf("Ok!\n");


}
