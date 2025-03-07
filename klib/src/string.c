#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  // panic("Not implemented");
  size_t len = 0;
  for (const char *c = s; *c != '\0'; ++c) {
    len++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  // panic("Not implemented");
  size_t i = 0;
  while (src[i] != '\0') {
    dst[i] = src[i];
    ++i;
  }
  dst[i] = '\0';
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  // panic("Not implemented");
  size_t i = 0;
  while ((src[i] != '\0') && (i < (n - 1)) ) {
    dst[i] = src[i];
    ++i;
  }
  dst[i] = '\0';
  return dst;
}

char *strcat(char *dst, const char *src) {
  // panic("Not implemented");
  size_t i = 0;
  while (dst[i] != '\0') {
    ++i;
  }
  size_t j = 0;
  while (src[j] != '\0') {
    dst[i] = src[j];
    ++i;
    ++j;
  }
  dst[i] = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  // panic("Not implemented");
  size_t i = 0;
  while ((s1[i] == s2[i]) && (s1[i] != '\0')) {
    ++i;
  }
  int res = (s1[i] > s2[i]) ? 1 : ((s1[i] < s2[i]) ? -1 : 0);
  return res;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  // panic("Not implemented");
  size_t i = 0;
  while ((s1[i] == s2[i]) && (s1[i] != '\0') && (i < n)) {
    ++i;
  }
  int res = (s1[i] > s2[i]) ? 1 : ((s1[i] < s2[i]) ? -1 : 0);
  return res;
}

void *memset(void *s, int c, size_t n) {
  // panic("Not implemented");
  unsigned char *ptr = (unsigned char *) s;
  for (size_t i = 0; i < n; ++i) {
    ptr[i] = (unsigned char) c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  // panic("Not implemented");
  unsigned char tmp[n];
  const unsigned char *src_ptr = (const unsigned char *) src;
  for (size_t i = 0; i < n; ++i) {
    tmp[i] = src_ptr[i];
  }
  unsigned char *dst_ptr = (unsigned char *) dst;
  for (size_t i = 0; i < n; ++i) {
    dst_ptr[i] = tmp[i];
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  // panic("Not implemented");
  unsigned char *out_ptr = (unsigned char *) out;
  const unsigned char *in_ptr = (const unsigned char *) in;
  for (size_t i = 0; i < n; ++i) {
    out_ptr[i] = in_ptr[i];
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  // panic("Not implemented");
  const unsigned char *s1_ptr = (const unsigned char *) s1;
  const unsigned char *s2_ptr = (const unsigned char *) s2;
  int res = 0;
  for (size_t i = 0; i < n; ++i) {
    if (s1_ptr[i] > s2_ptr[i]) {
      res = 1;
      break;
    } else if (s1_ptr[i] < s2_ptr[i]) {
      res = -1;
      break;
    }
  }
  return res;
}

#endif
