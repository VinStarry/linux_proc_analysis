#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_SIZE 1024
#define FNAME_SIZE 256

int main(void) {
	int fd;
	char buf[MAX_SIZE];
	char get[MAX_SIZE];
	char devName[FNAME_SIZE];
	char filename[FNAME_SIZE] = "/dev/";
	system("ls /dev/");
	printf("Enter the device's name you wan to use :");
	fgets(devName, FNAME_SIZE, stdin);
	devName[strlen(devName) - 1] = '\0';
	strcat(filename, devName);
	printf("%s\n", filename);
	fd = open(filename, O_RDWR | O_NONBLOCK);
	if (fd != -1) {
		read(fd, buf, sizeof(buf));
		printf("The device was inited with a string: %s\n", buf);

		printf("Enter the string you want to write:\n");
		fgets(get, MAX_SIZE, stdin);
		write(fd, get, sizeof(get));
		
		read(fd, buf, sizeof(buf)); 
		printf("The string in the device now is: %s\n", buf);
		close(fd);
		return 0;
	}

	else {
		printf("Device open failed\n");
		return -1;
	}
	return 0;
}