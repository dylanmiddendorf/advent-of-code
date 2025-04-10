// Copyright (c) 2025 Dylan Middendorf
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int
main (int argc, char *argv[])
{
  size_t i, bytes_read;

  char *puzzle;
  long puzzle_size;
  FILE *puzzle_input;
  struct stat statbuf;

  long floor = 0;

  if (stat ("input.txt", &statbuf) == -1)
    {
      perror ("Failed to get file size");
      return EXIT_FAILURE;
    }

  if (!(puzzle_input = fopen ("input.txt", "r")))
    {
      perror ("Failed to open file");
      return EXIT_FAILURE;
    }

  puzzle_size = statbuf.st_size;
  if (!(puzzle = malloc (puzzle_size + 1)))
    {
      perror ("Memory allocation failed");

      fclose (puzzle_input);
      return EXIT_FAILURE;
    }

  bytes_read = fread (puzzle, 1, puzzle_size, puzzle_input);
  fclose (puzzle_input); // Release the system resource
  puzzle[bytes_read] = '\0';

  if (bytes_read != puzzle_size)
    fprintf (stderr, "Warning: Read %zu bytes, but file size is %ld bytes\n",
             bytes_read, puzzle_size);

  for (i = 0; i < bytes_read; ++i)
    {
      assert (puzzle[i] == '(' || puzzle[i] == ')');
      floor += puzzle[i] == '(' ? 1 : -1;
    }

  printf ("Part 1: %ld\n", floor);

  floor = 0;
  for (i = 0; i < bytes_read; ++i)
    {
      floor += puzzle[i] == '(' ? 1 : -1;

      if (floor == -1)
        {
          printf ("Part 2: %ld\n", i + 1);
          break;
        }
    }

  free (puzzle);
  return EXIT_SUCCESS;
}