// Copyright (c) 2025 Dylan Middendorf
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define RENDER_DISTANCE 256

typedef struct pair_s
{
  int x;
  int y;
} pair_t;

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

  char *puzzle;
  long puzzle_size;

  int x = (RENDER_DISTANCE >> 1) - 1, y = x;
  pair_t entity_posititons[2] = { { x, y }, { x, y } };
  pair_t *selected;
  bool map[RENDER_DISTANCE][RENDER_DISTANCE] = { false };
  int houses = 1;

  map[x][y] = true; // Santa always visits the origin

  puzzle_size = load_file ("input.txt", &puzzle);
  if (puzzle_size == -1)
    return EXIT_FAILURE;

  for (i = 0; i < puzzle_size; ++i)
    {
      switch (puzzle[i])
        {
        case '^':
          y++;
          break;
        case '>':
          x++;
          break;
        case 'v':
          y--;
          break;
        case '<':
          x--;
          break;
        case '\n':
          break; // EOF newline...
        default:
          fprintf (stderr, "Warning: invalid instruction detected\n");
        }

      assert (0 <= x && x < RENDER_DISTANCE && 0 <= y && y < RENDER_DISTANCE);
      if (!map[y][x])
        houses++;
      map[y][x] = true;
    }

  printf ("Part 1: %d\n", houses);

  houses = 0;
  memset (map, '\0', sizeof (map));
  for (i = 0; i < puzzle_size; ++i)
    {
      selected = entity_posititons + (i & 1);
      switch (puzzle[i])
        {
        case '^':
          selected->y++;
          break;
        case '>':
          selected->x++;
          break;
        case 'v':
          selected->y--;
          break;
        case '<':
          selected->x--;
          break;
        case '\n':
          break; // EOF newline...
        default:
          fprintf (stderr, "Warning: invalid instruction detected\n");
        }

      assert (0 <= selected->x && selected->x < RENDER_DISTANCE
              && 0 <= selected->y && selected->y < RENDER_DISTANCE);
      if (!map[selected->y][selected->x])
        houses++;
      map[selected->y][selected->x] = true;
    }
  printf ("Part 2: %d\n", houses);

  free (puzzle);
  return EXIT_SUCCESS;
}
