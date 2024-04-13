#ifndef _PRINTF_H
#define _PRINTF_H

#include <stdarg.h>

void init_printf(void (*put_char) (char));
void string_format(void (*put_char) (char), char *format_string, va_list va);
void printf(char *formated_string, ...);

#endif /* _PRINTF_H */