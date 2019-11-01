#include<stdio.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/sendfile.h>
#include<unistd.h>
void copy(const char * src, const char * dest);
