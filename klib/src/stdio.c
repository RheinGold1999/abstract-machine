#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// 反转字符串
void reverse(char *str, int len) {
  int i = 0, j = len - 1;
  while (i < j) {
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++;
    j--;
  }
}

// 将 int 转换为字符串
int intToStr(int num, char *str) {
  int i = 0;
  int isNegative = 0;

  // 处理负数
  if (num < 0) {
    isNegative = 1;
    num = -num;
  }

  // 处理 0 的特殊情况
  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return i;
  }

  // 逐位提取数字并转换为字符
  while (num != 0) {
    int digit = num % 10;
    str[i++] = digit + '0';
    num = num / 10;
  }

  // 添加负号（如果需要）
  if (isNegative) {
    str[i++] = '-';
  }

  // 反转字符串
  reverse(str, i);
  str[i] = '\0'; // 添加字符串结束符
  return i;
}

int printf(const char *fmt, ...) {
  // panic("Not implemented");
  va_list ap;
  int d;
  char c;
  char *s;

  va_start(ap, fmt);
  int i = 0;
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
        case 's': {
          s = va_arg(ap, char *);
          while (*s) {
            putch(*s);
            i++;
            s++;
          }
          break;
        }

        case 'd': {
          d = va_arg(ap, int);
          char num[32];
          intToStr(d, num);
          int n = 0;
          while (num[n]) {
            putch(num[n]);
            i++;
            n++;
          }
          break;
        }

        case 'c': {
          c = (char) va_arg(ap, int);
          putch(c);
          i++;
          break;
        }

        case '%': {
          putch('%');
          i++;
          break;
        }

        default: {
          printf("fmt `%%%c` is not supported.\n", *fmt);
          assert(false);
          break;
        }
      }
      fmt++;
    } else {
      putch(*fmt);
      i++;
      fmt++;
    }
  }
  return i;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  // panic("Not implemented");
  va_list ap;
  int d;
  char c;
  char *s;

  va_start(ap, fmt);
  int i = 0;
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
        case 's': {
          s = va_arg(ap, char *);
          while (*s) {
            out[i] = *s;
            // printf("out[%d] = %c\n", i, out[i]);
            i++;
            s++;
          }
          break;
        }

        case 'd': {
          d = va_arg(ap, int);
          char num[32];
          intToStr(d, num);
          int n = 0;
          while (num[n]) {
            out[i] = num[n];
            // printf("out[%d] = %c\n", i, out[i]);
            i++;
            n++;
          }
          break;
        }

        case 'c': {
          c = (char) va_arg(ap, int);
          out[i] = c;
          // printf("out[%d] = %c\n", i, out[i]);
          i++;
          break;
        }

        case '%': {
          out[i] = '%';
          i++;
          break;
        }

        default: {
          printf("fmt `%%%c` is not supported.", *fmt);
          assert(false);
          break;
        }
      }
      fmt++;
    } else {
      out[i] = *fmt;
      // printf("out[%d] = %c\n", i, out[i]);
      i++;
      fmt++;
    }
  }
  out[i] = '\0';
  // printf("out[%d] = %c\n", i, out[i]);
  return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
