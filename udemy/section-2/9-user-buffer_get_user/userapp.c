#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

# define DEVICE_FILE "/dev/myint"

int main()
{
  int fd;
  int retval;
  int value = 10;

  printf("Opening File: %s\n", DEVICE_FILE);
  fd = open(DEVICE_FILE, O_RDWR);

  if (fd < 0) {
    perror("Open Failed");
    exit(1);
  }

  getchar();

  retval = write(fd, &value, sizeof(value));
  printf("Write %i to %s, retval: %d\n", value, DEVICE_FILE, retval);
  getchar();

  value = 0;
  retval = read(fd, &value, sizeof(value));
  printf("Read from %s, value= %i, retval: %d\n", DEVICE_FILE, value, retval);
  getchar();

  printf("Closing File\n");
  close(fd);

  getchar();

  return 0;
}
