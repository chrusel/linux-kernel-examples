#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

# define DEVICE_FILE "/dev/mystruct"

#define EXPLICIT_INLINE 1
#ifdef EXPLICIT_INLINE
	typedef struct ics
	{
		int i;
		char c;
		char s[32];
	} ics;
#elif
	#include "mystruct.h"
#endif

int main()
{
  int fd;
  int retval;
  ics user_struct = {4711, 'C', "hello structure"};

  printf("Opening File: %s\n", DEVICE_FILE);
  fd = open(DEVICE_FILE, O_RDWR);

  if (fd < 0) {
    perror("Open Failed");
    exit(1);
  }

  getchar();

  retval = write(fd, &user_struct, sizeof(user_struct));
  printf("Write user_struct to %s, retval: %d\n", DEVICE_FILE, retval);
  getchar();
  
  // clean foo instance of user_struct:
  memset(&user_struct, '\0', sizeof(user_struct));

  retval = read(fd, &user_struct, sizeof(user_struct));
  printf("Read user_struct from %s, retval: %d\n", DEVICE_FILE, retval);
  printf("  - i = %i\n", user_struct.i);
  printf("  - c = %c\n", user_struct.c);
  printf("  - s = %s\n", user_struct.s);
  getchar();

  printf("Closing File\n");
  close(fd);

  getchar();

  return 0;
}
