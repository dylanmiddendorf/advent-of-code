#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>

// TODO: optimize for the fun of it...
//   3-run average (-O0): 4.828s
//   3-run average (-O3): 4.461s

// NOTE: MAX_PRCISION <= 8
#define MAX_PRECISION 6

#define HEX_ALPHABET "0123456789abcdef"

static char _md5_digest_buffer[32 + 1];

static unsigned char *
MD5 (const unsigned char *d, unsigned long cnt, unsigned char *md)
{
  int i = 0;
  char digest[16]; // MD5 output buffer
  EVP_MD_CTX *ctx = EVP_MD_CTX_new ();
  assert (d);

  if (md == NULL)
    md = _md5_digest_buffer;

  if (!ctx)
    {
      return NULL;
    }

  if (EVP_DigestInit_ex (ctx, EVP_md5 (), NULL) != 1
      || EVP_DigestUpdate (ctx, d, cnt) != 1
      || EVP_DigestFinal_ex (ctx, digest, NULL) != 1)
    {
      EVP_MD_CTX_free (ctx);
      return NULL;
    }

  EVP_MD_CTX_free (ctx);

  for (i = 0; i < 16; ++i)
    {
      md[i << 1] = HEX_ALPHABET[(digest[i] & 0xF0) >> 4];
      md[(i << 1) + 1] = HEX_ALPHABET[digest[i] & 0x0F];
    }
  md[32] = '\0';
  return md;
}

static const int _width[] = { -1, 2, 3, 4, 5, 7, 8, 9, 10 };

static unsigned
md5_crack (const char *seed, int precision)
{
  unsigned i;

  // upper_bound: 1 -> 0xF, 2 -> 0xFF, 3 -> 0xFFF, ...
  unsigned upper_bound = (1 << ((long)precision * 4)) - 1;
  char *buffer, *digest, target[MAX_PRECISION];
  int buffer_size;

  assert (0 < precision && precision <= MAX_PRECISION);
  if (!(buffer = malloc (strlen (seed) + _width[precision] + 1)))
    {
      perror ("Memory allocation failed");
      return -1;
    }

  memset (target, '0', precision);
  for (i = 0; /* assume match exists */; ++i)
    {
      buffer_size = sprintf (buffer, "%s%d", seed, i);
      digest = MD5 (buffer, buffer_size, NULL);
      if (memcmp (digest, target, precision) == 0)
        {
          free (buffer);
          return i;
        }
    }

  free (buffer);
  return -1;
}

int
main (int argc, char *argv[])
{
  char *seed = "iwrupvqb";
  printf ("Part 1: %d\n", md5_crack (seed, 5));
  printf ("Part 2: %d\n", md5_crack (seed, 6));
}