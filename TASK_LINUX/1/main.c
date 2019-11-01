#include<locale.h>
#include "CopyFile.h"

int main(int argc, char** argv) {


	setlocale(LC_ALL, "");
	if (argc != 3)
	{
		fprintf(stderr, "Invalid parameters. Usage: %s <in-file> <out-file>\n", argv[0]);
		return 1;
	}

	copy(argv[1], argv[2]);
	return 0;


}
