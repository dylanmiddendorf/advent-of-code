// Copyright (c) 2025 Dylan Middendorf
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char VOWELS[] = "aeiou";

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

  int x, y;
  int x1, y1, x2, y2;

  int lights_on = 0;
  int part2 = 0;

  bool p1_grid[1000][1000] = { 0 };
  int p2_grid[1000][1000] = { 0 };

  puzzle_size = load_file ("input.txt", &puzzle);
  if (puzzle_size == -1)
    return EXIT_FAILURE;

  line = strtok (puzzle, "\n");
  for (; line; line = strtok (NULL, "\n"))
    {
      if (strncmp ("turn on", line, 7) == 0)
        {
          sscanf (line, "turn on %d,%d through %d,%d", &x1, &y1, &x2, &y2);
          for (y = y1; y <= y2; ++y)
            {
              for (x = x1; x <= x2; ++x)
                {
                  p1_grid[y][x] = true;
                  p2_grid[y][x]++;
                }
            }
        }
      else if (strncmp ("turn off", line, 8) == 0)
        {
          sscanf (line, "turn off %d,%d through %d,%d", &x1, &y1, &x2, &y2);
          for (y = y1; y <= y2; ++y)
            {
              for (x = x1; x <= x2; ++x)
                {
                  p1_grid[y][x] = 0;
                  if (p2_grid[y][x])
                    p2_grid[y][x]--;
                }
            }
        }
      else
        {
          sscanf (line, "toggle %d,%d through %d,%d", &x1, &y1, &x2, &y2);
          for (y = y1; y <= y2; ++y)
            {
              for (x = x1; x <= x2; ++x)
                {
                  p1_grid[y][x] = !p1_grid[y][x];
                  p2_grid[y][x] += 2;
                }
            }
        }
    }

  for (y = 0; y < 1000; ++y)
    {
      for (x = 0; x < 1000; ++x)
        {
          lights_on += p1_grid[y][x];
          part2 += p2_grid[y][x];
        }
    }
  printf ("Part 1: %d\n", lights_on);
  printf ("Part 2: %d\n", part2);

  free (puzzle);
  return EXIT_SUCCESS;
}
