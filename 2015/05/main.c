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

bool
p1_is_nice (const char *s)
{
  size_t i = 0;
  int vowels = 0;
  bool hasDuplicate = false;

  for (; s[i]; ++i)
    {
      if (strchr (VOWELS, s[i]))
        vowels++;

      if (i && s[i] == s[i - 1])
        hasDuplicate = true;

      if (i && (s[i - 1] == 'a' && s[i] == 'b')
          || (s[i - 1] == 'c' && s[i] == 'd')
          || (s[i - 1] == 'p' && s[i] == 'q')
          || (s[i - 1] == 'x' && s[i] == 'y'))
        return false;
    }

  return vowels >= 3 && hasDuplicate;
}

bool
p2_is_nice (const char *s)
{
  size_t i = 0;
  bool hasTwoPair = 0;
  bool hasFamily = false;
  char buffer[3] = { 0 };

  for (; s[i]; ++i)
    {
      if (i)
        {
          buffer[0] = s[i - 1];
          buffer[1] = s[i];

          if (strstr (s + i + 1, buffer))
            hasTwoPair = true;
        }

      if (i >= 2 && s[i] == s[i - 2])
        hasFamily = true;
    }

  return hasTwoPair && hasFamily;
}

int
main (int argc, char *argv[])
{
  size_t i;

  char *puzzle, *line;
  long puzzle_size;

  int part1 = 0;
  int part2 = 0;

  puzzle_size = load_file ("input.txt", &puzzle);
  if (puzzle_size == -1)
    return EXIT_FAILURE;

  line = strtok (puzzle, "\n");
  for (; line; line = strtok (NULL, "\n"))
    {
      part1 += p1_is_nice (line);
      part2 += p2_is_nice (line);
    }
  printf ("Part 1: %d\n", part1);
  printf ("Part 2: %d\n", part2);

  free (puzzle);
  return EXIT_SUCCESS;
}
