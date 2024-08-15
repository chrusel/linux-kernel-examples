#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

# define DEVICE_FILE "/dev/chruseldev"

int main()
{
  int fd;
  int retval;
  char buffer[10];
  
  printf("Opening File: %s\n", DEVICE_FILE);
  fd = open(DEVICE_FILE, O_RDWR);
  if (fd < 0) {
    perror("Please insmod the kernel module char driver and set /dev/chruseldev to mod 666!");
    perror("Open Failed");
    exit(1);
  }
  close(fd);
  getchar();

  printf("open with O_RDONLY flag:\n");
  fd = open(DEVICE_FILE, O_RDONLY);
  if (fd < 0) {
    perror("Open Failed");
    exit(1);
  }
  close(fd);
  getchar();

  printf("open with O_WRONLY flag:\n");
  fd = open(DEVICE_FILE, O_WRONLY);
  if (fd < 0) {
    perror("Open Failed");
    exit(1);
  }
  close(fd);
  getchar();

  printf("open with O_WRONLY|O_NONBLOCK flags:\n");
  fd = open(DEVICE_FILE, O_WRONLY|O_NONBLOCK);
  if (fd < 0) {
    perror("Open Failed");
    exit(1);
  }
  close(fd);
  getchar();

  printf("open with O_WRONLY |O_NONBLOCK|O_APPEND flags:\n");
  fd = open(DEVICE_FILE, O_WRONLY | O_NONBLOCK | O_APPEND);
  if (fd < 0) {
    perror("Open Failed");
    exit(1);
  }
  close(fd);
  getchar();  
  
  return 0;
}
