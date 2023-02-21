#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int event1_id;

char* const TRACE_FILE="/mnt/debugfs/ltt/write_event";

int main(int argc, char *argv[])
{
  char *filename;
  int fd;

  if ((argc > 1) && (argv[1][0] != '-')) {
    filename = argv[1];
  } else {
    filename = TRACE_FILE;
  }

  printf("Opening trace file %s.\n", filename);

  fd = open(filename, O_WRONLY);

  if(fd < 0)
  {
    printf("Unable to open trace file.\n");
    exit(1);
  }
  printf("\n");

  /* Create events */
  printf("Create events.\n");

  write(fd, "This is the first event.\n", 25);
  write(fd, "This is the second event.\n", 26);
  write(fd, "This is the third event.\n", 25);

  /* Close the trace file */
  if(close(fd) < 0)
  {
    printf("Unable to close trace file \n");
    exit(1);
  }  
  return 0;
}
