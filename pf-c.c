#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREY "\033[90m"

int get_terminal_width();
char *extract_filename(const char *path);
void print_header(int count, const char *filename);
void print_footer(int count);

int main(int argc, char *argv[]) {
  // Check if command line argument for path is passed
  if (argc != 2) {
    fprintf(stderr, "Path not entered!\n");
    return 1;
  }

  // Open file and check if path is correct or openable
  FILE *fPtr = fopen(argv[1], "r");
  if (fPtr == NULL) {
    fprintf(stderr, "Path not found or inaccessible!\n");
    return 1;
  }

  // Get terminal width and filename
  int count = get_terminal_width() - 8;
  char *filename = extract_filename(argv[1]);

  // Print header
  print_header(count, filename);

  // Read and print line by line
  char line[128];
  int line_number = 1;
  while (fgets(line, sizeof(line), fPtr) != NULL) {
    if (line_number <= 9) {
      fprintf(stdout, "   %s%d%s   │ %s", GREY, line_number, RESET, line);
    } else if (line_number <= 99) {
      fprintf(stdout, "  %s%d%s   │ %s", GREY, line_number, RESET, line);
    } else {
      fprintf(stdout, "%s%d%s   │ %s", GREY, line_number, RESET, line);
    }
    line_number++;
  }

  // Print footer
  print_footer(count);

  fclose(fPtr);
  free(filename);

  return 0;
}

int get_terminal_width() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

char *extract_filename(const char *path) {
  char *filename = strrchr(path, '/');
  if (filename != NULL) {
    return strdup(filename +
                  1); // Return a copy of the filename without the leading '/'
  } else {
    return strdup(path); // Return a copy of the path if no '/' is found
  }
}

void print_header(int count, const char *filename) {
  printf("───────┬");
  for (int i = 0; i < count; i++) {
    printf("─");
  }
  printf("       │ File: %s%s%s\n", BOLD, filename, RESET);
  printf("───────┼");
  for (int i = 0; i < count; i++) {
    printf("─");
  }
  printf("\n");
}

void print_footer(int count) {
  printf("───────┴");
  for (int i = 0; i < count; i++) {
    printf("─");
  }
  printf("\n");
}
