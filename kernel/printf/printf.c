#include "printf.h"
#include "drivers/uart/uart.h"

typedef void (*stdout_put_char)(char);
static stdout_put_char stdout_put;

void init_printf(stdout_put_char put_char) {
    stdout_put = put_char;
}

void unsigned_int_to_a (unsigned int num, unsigned int base, int upper_case, char* buffer) {
    int n = 1;
    unsigned int divider = 1;
    while (num / divider >= base) {
        divider *= base;
    }
    while (divider != 0 ) {
        int digit = num / divider;
        num = num % divider;
        divider = divider / base;
        if (n || digit > 0 || divider == 0) {
            *buffer++ = digit + (digit < 10 ? '0' : (upper_case ? 'A' : 'a') - 10);
            ++n;
        }
    }
    *buffer = 0;
}

void signed_int_to_a (int num, char* buffer) {
    if (num < 0) {
        num = -num;
        *buffer++ = '-';
    }
    unsigned_int_to_a(num, 10, 0, buffer);
}


void string_format(stdout_put_char stdout_put, char *format_string, va_list va) {
    char output_buffer[12];
    char character;

    while ((character = *(format_string++))) {
        if (character != '%') {
            uart_send(character);
        } else {
            character = *(format_string++);
            switch (character) {
                case 'd':
                    signed_int_to_a(va_arg(va, int), output_buffer);
                    uart_send_string(output_buffer);
                    break;
                case 'x':
                    unsigned_int_to_a(va_arg(va, unsigned int), 16, (character=='X'), output_buffer);
                    uart_send_string(output_buffer);
                    break;
                case 'f':
                    break;
                case 'c':
                    uart_send((char)va_arg(va, int));
                    break;
            }

        }
    }
}
void printf(char *format_string, ...) {
    va_list va;
    va_start(va, format_string);
    string_format(stdout_put, format_string, va);
    va_end(va);
}