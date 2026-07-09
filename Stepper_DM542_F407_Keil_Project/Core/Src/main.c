#include "board.h"
#include "stepper.h"
#include "usart.h"

static int parse_i32(const char *s, int32_t *out)
{
    int sign = 1;
    int32_t value = 0;

    while (*s == ' ' || *s == '\t') {
        s++;
    }

    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    if (*s < '0' || *s > '9') {
        return 0;
    }

    while (*s >= '0' && *s <= '9') {
        value = (value * 10L) + (int32_t)(*s - '0');
        s++;
    }

    while (*s == ' ' || *s == '\t') {
        s++;
    }

    if (*s != '\0') {
        return 0;
    }

    *out = value * sign;
    return 1;
}

static void print_i32(int32_t value)
{
    char buf[12];
    int i = 0;
    uint32_t v;

    if (value < 0) {
        usart1_putc('-');
        v = (uint32_t)(-value);
    } else {
        v = (uint32_t)value;
    }

    do {
        buf[i++] = (char)('0' + (v % 10U));
        v /= 10U;
    } while (v != 0U);

    while (i > 0) {
        usart1_putc(buf[--i]);
    }
}

static void print_status(const char *prefix)
{
    usart1_puts(prefix);
    usart1_puts(" pos=");
    print_i32(stepper_position());
    usart1_puts(" left=");
    print_i32(stepper_left_limit_coord());
    usart1_puts(" right=");
    print_i32(stepper_right_limit_coord());
    usart1_puts("\r\n");
}

int main(void)
{
    char line[32];
    StepperStatus status;

    delay_init();
    usart1_init(115200UL);
    stepper_init();

    usart1_puts("\r\nSTM32F407 DM542 stepper controller\r\n");
    usart1_puts("Homing...\r\n");

    status = stepper_home();
    if (status != STEPPER_OK) {
        usart1_puts("HOME_ERR ");
        print_i32((int32_t)status);
        usart1_puts("\r\n");
        stepper_enable(0U);
        while (1) {
        }
    }

    print_status("HOME_OK");
    usart1_puts("Send Y coordinate in pulses, newline ended. Example: 1200 or -800\r\n");

    while (1) {
        int rx = usart1_getline(line, sizeof(line));

        if (rx > 0) {
            int32_t target;

            if (parse_i32(line, &target) == 0) {
                usart1_puts("ERR bad coordinate\r\n");
                continue;
            }

            if (target < stepper_left_limit_coord() || target > stepper_right_limit_coord()) {
                usart1_puts("ERR out of range\r\n");
                continue;
            }

            status = stepper_move_to(target);
            if (status == STEPPER_OK) {
                print_status("OK");
            } else {
                usart1_puts("MOVE_ERR ");
                print_i32((int32_t)status);
                usart1_puts("\r\n");
            }
        } else if (rx < 0) {
            usart1_puts("ERR line too long\r\n");
        }
    }
}
