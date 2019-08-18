/*
 * ciotuil.h: simple i/o routines for C
 */


#ifndef CIOUTIL_H
#define CIOUTIL_H


#include <stdio.h>


#if !defined(bool) && !defined(__cplusplus)
  typedef int bool;
  #define true    1
  #define false   0
#endif /* !bool */

#ifndef byte
  typedef unsigned char byte;
#endif /* !byte */
#ifndef sbyte
  typedef signed char sbyte;
#endif /* !sbyte */

#ifndef countof
  #define countof(a)    (sizeof(a) / sizeof(a[0]))
#endif

char* removeExt(char* path);

/* unsigned byte to signed */
__inline int utos1(unsigned int value)
{
  return (value & 0x80) ? -(signed)(value ^ 0xff)-1 : value;
}

/* unsigned 2bytes to signed */
__inline int utos2(unsigned int value)
{
  return (value & 0x8000) ? -(signed)(value ^ 0xffff)-1 : value;
}

/* unsigned 3bytes to signed */
__inline int utos3(unsigned int value)
{
  return (value & 0x800000) ? -(signed)(value ^ 0xffffff)-1 : value;
}

/* unsigned 4bytes to signed */
__inline int utos4(unsigned int value)
{
  return (value & 0x80000000) ? -(signed)(value ^ 0xffffffff)-1 : value;
}

/* get a byte */
__inline int mget1(const byte* data)
{
  return data[0];
}

/* get 2bytes as little-endian */
__inline int mget2l(const byte* data)
{
  return data[0] | (data[1] * 0x0100);
}

/* get 3bytes as little-endian */
__inline int mget3l(const byte* data)
{
  return data[0] | (data[1] * 0x0100) | (data[2] * 0x010000);
}

/* get 4bytes as little-endian */
__inline int mget4l(const byte* data)
{
  return data[0] | (data[1] * 0x0100) | (data[2] * 0x010000) | (data[3] * 0x01000000);
}

/* get 2bytes as big-endian */
__inline int mget2b(const byte* data)
{
  return data[1] | (data[0] * 0x0100);
}

/* get 3bytes as big-endian */
__inline int mget3b(const byte* data)
{
  return data[2] | (data[1] * 0x0100) | (data[0] * 0x010000);
}

/* get 4bytes as big-endian */
__inline int mget4b(const byte* data)
{
  return data[3] | (data[2] * 0x0100) | (data[1] * 0x010000) | (data[0] * 0x01000000);
}

/* put a byte */
__inline int mput1(int value, byte* data)
{
  int lastPut = value;
  data[0] = lastPut & 0xff;
  return lastPut & 0xff;
}

/* put 2bytes as little-endian */
__inline int mput2l(int value, byte* data)
{
  int lastPut = value;
  data[0] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[1] = lastPut & 0xff;
  return lastPut & 0xff;
}

/* put 3bytes as little-endian */
__inline int mput3l(int value, byte* data)
{
  int lastPut = value;
  data[0] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[1] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[2] = lastPut & 0xff;
  return lastPut & 0xff;
}

/* put 4bytes as little-endian */
__inline int mput4l(int value, byte* data)
{
  int lastPut = value;
  data[0] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[1] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[2] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[3] = lastPut & 0xff;
  return lastPut & 0xff;
}

/* put 2bytes as big-endian */
__inline int mput2b(int value, byte* data)
{
  int lastPut = value;
  data[1] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[0] = lastPut & 0xff;
  return lastPut & 0xff;
}

/* put 3bytes as big-endian */
__inline int mput3b(int value, byte* data)
{
  int lastPut = value;
  data[2] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[1] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[0] = lastPut & 0xff;
  return lastPut & 0xff;
}

/* put 4bytes as big-endian */
__inline int mput4b(int value, byte* data)
{
  int lastPut = value;
  data[3] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[2] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[1] = lastPut & 0xff;
  lastPut /= 0x0100;
  data[0] = lastPut & 0xff;
  return lastPut & 0xff;
}

/* get a byte from file */
__inline int fget1(FILE* stream)
{
  return fgetc(stream);
}

/* get 2bytes as little-endian from file */
__inline int fget2l(FILE* stream)
{
  int b1;
  int b2;

  b1 = fgetc(stream);
  b2 = fgetc(stream);
  if((b1 != EOF) && (b2 != EOF))
  {
    return b1 | (b2 * 0x0100);
  }
  return EOF;
}

/* get 3bytes as little-endian from file */
__inline int fget3l(FILE* stream)
{
  int b1;
  int b2;
  int b3;

  b1 = fgetc(stream);
  b2 = fgetc(stream);
  b3 = fgetc(stream);
  if((b1 != EOF) && (b2 != EOF) && (b3 != EOF))
  {
    return b1 | (b2 * 0x0100) | (b3 * 0x010000);
  }
  return EOF;
}

/* get 4bytes as little-endian from file */
__inline int fget4l(FILE* stream)
{
  int b1;
  int b2;
  int b3;
  int b4;

  b1 = fgetc(stream);
  b2 = fgetc(stream);
  b3 = fgetc(stream);
  b4 = fgetc(stream);
  if((b1 != EOF) && (b2 != EOF) && (b3 != EOF) && (b4 != EOF))
  {
    return b1 | (b2 * 0x0100) | (b3 * 0x010000) | (b4 * 0x01000000);
  }
  return EOF;
}

/* get 2bytes as big-endian from file */
__inline int fget2b(FILE* stream)
{
  int b1;
  int b2;

  b1 = fgetc(stream);
  b2 = fgetc(stream);
  if((b1 != EOF) && (b2 != EOF))
  {
    return b2 | (b1 * 0x0100);
  }
  return EOF;
}

/* get 3bytes as big-endian from file */
__inline int fget3b(FILE* stream)
{
  int b1;
  int b2;
  int b3;

  b1 = fgetc(stream);
  b2 = fgetc(stream);
  b3 = fgetc(stream);
  if((b1 != EOF) && (b2 != EOF) && (b3 != EOF))
  {
    return b3 | (b2 * 0x0100) | (b1 * 0x010000);
  }
  return EOF;
}

/* get 4bytes as big-endian from file */
__inline int fget4b(FILE* stream)
{
  int b1;
  int b2;
  int b3;
  int b4;

  b1 = fgetc(stream);
  b2 = fgetc(stream);
  b3 = fgetc(stream);
  b4 = fgetc(stream);
  if((b1 != EOF) && (b2 != EOF) && (b3 != EOF) && (b4 != EOF))
  {
    return b4 | (b3 * 0x0100) | (b2 * 0x010000) | (b1 * 0x01000000);
  }
  return EOF;
}

/* put a byte to file */
__inline int fput1(int value, FILE* stream)
{
  return fputc(value & 0xff, stream);
}

/* put 2bytes as little-endian to file */
__inline int fput2l(int value, FILE* stream)
{
  int result;

  result = fputc(value & 0xff, stream);
  if(result != EOF)
  {
    result = fputc((value >> 8) & 0xff, stream);
  }
  return result;
}

/* put 3bytes as little-endian to file */
__inline int fput3l(int value, FILE* stream)
{
  int result;

  result = fputc(value & 0xff, stream);
  if(result != EOF)
  {
    result = fputc((value >> 8) & 0xff, stream);
    if(result != EOF)
    {
      result = fputc((value >> 16) & 0xff, stream);
    }
  }
  return result;
}

/* put 4bytes as little-endian to file */
__inline int fput4l(int value, FILE* stream)
{
  int result;

  result = fputc(value & 0xff, stream);
  if(result != EOF)
  {
    result = fputc((value >> 8) & 0xff, stream);
    if(result != EOF)
    {
      result = fputc((value >> 16) & 0xff, stream);
      if(result != EOF)
      {
        result = fputc((value >> 24) & 0xff, stream);
      }
    }
  }
  return result;
}

/* put 2bytes as big-endian to file */
__inline int fput2b(int value, FILE* stream)
{
  int result;

  result = fputc((value >> 8) & 0xff, stream);
  if(result != EOF)
  {
    result = fputc(value & 0xff, stream);
  }
  return result;
}

/* put 3bytes as big-endian to file */
__inline int fput3b(int value, FILE* stream)
{
  int result;

  result = fputc((value >> 16) & 0xff, stream);
  if(result != EOF)
  {
    result = fputc((value >> 8) & 0xff, stream);
    if(result != EOF)
    {
      result = fputc(value & 0xff, stream);
    }
  }
  return result;
}

/* put 4bytes as big-endian to file */
__inline int fput4b(int value, FILE* stream)
{
  int result;

  result = fputc((value >> 24) & 0xff, stream);
  if(result != EOF)
  {
    result = fputc((value >> 16) & 0xff, stream);
    if(result != EOF)
    {
      result = fputc((value >> 8) & 0xff, stream);
      if(result != EOF)
      {
        result = fputc(value & 0xff, stream);
      }
    }
  }
  return result;
}


#endif /* !CIOUTIL_H */
