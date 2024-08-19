#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

# define DEVICE_FILE "/dev/myjiffies"

int main()
{
  int fd;
  unsigned long long old_jiffies;
  unsigned long long new_jiffies;

  fd = open(DEVICE_FILE, O_RDWR);
  if (fd < 0) {
    perror("Please insmod the kernel module char driver and set DEVICE_FILE to mod 666!");
    perror("Open Failed");
    exit(2);
  }
  if (read(fd, &old_jiffies, sizeof(old_jiffies)) != sizeof(old_jiffies)) {
  	printf("Failed in reading first jiffies\n");
  	exit(3);
  } else {
   	printf("First Read: %llu\n", old_jiffies);
  }
  sleep(1);
  if (read(fd, &new_jiffies, sizeof(new_jiffies)) != sizeof(new_jiffies)) {
    printf("Failed in reading second jiffies\n");
    exit(4);
  } else {
 	printf("Second Read: %llu\n", new_jiffies);
  }
  printf("Difference: %llu\n", new_jiffies - old_jiffies);

  printf("Closing File\n");
  close(fd);

  return 0;
}
