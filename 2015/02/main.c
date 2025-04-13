// Copyright (c) 2025 Dylan Middendorf
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static int
min (int a, int b)
{
  return (a <= b) ? a : b;
}

static int
max (int a, int b)
{
  return (a >= b) ? a : b;
}

static long
load_file (const char *filename, char **contents)
{
  size_t bytes_read;
  long file_size;
  FILE *file;
  struct stat statbuf;

  if (stat (filename, &statbuf) == -1)
    {
      perror ("Failed to get file size");
      *contents = NULL;
      return -1;
    }

  if (!(file = fopen (filename, "r")))
    {
      perror ("Failed to open file");
      *contents = NULL;
      return -1;
    }

  file_size = statbuf.st_size;
  if (!(*contents = malloc (file_size + 1)))
    {
      perror ("Memory allocation failed");

      fclose (file);
      *contents = NULL;
      return -1;
    }

  bytes_read = fread (*contents, 1, file_size, file);
  fclose (file); // Release the system resource
  (*contents)[bytes_read] = '\0';

  if (bytes_read != file_size)
    fprintf (stderr, "Warning: Read %zu bytes, but file size is %ld bytes\n",
             bytes_read, file_size);

  return bytes_read;
}

int
main (int argc, char *argv[])
{
  size_t i;

  char *puzzle, *line;
  long puzzle_size;

  int l, w, h, largest;
  long required_paper = 0;
  long required_ribbon = 0;

  puzzle_size = load_file ("input.txt", &puzzle);
  if (puzzle_size == -1)
    return EXIT_FAILURE;

  line = strtok (puzzle, "\n");
  for (; line; line = strtok (NULL, "\n"))
    {
      sscanf (line, "%dx%dx%d", &l, &w, &h);
      required_paper += (l * w + w * h + h * l) << 1;
      required_paper += min (l * w, min (w * h, h * l));

      // Remove the null-terminators placed by strtok
      if (line != puzzle)
        line[-1] = '\n';
    }
  printf ("Part 1: %ld\n", required_paper);

  line = strtok (puzzle, "\n");
  for (; line; line = strtok (NULL, "\n"))
    {
      sscanf (line, "%dx%dx%d", &l, &w, &h);
      required_ribbon += ((l + w + h) - max (l, max (w, h))) << 1;
      required_ribbon += l * w * h;
    }
  printf ("Part 2: %ld\n", required_ribbon);

  free (puzzle);
  return EXIT_SUCCESS;
}
