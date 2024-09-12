#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"

#define INTERVAL 150

void dot() {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(INTERVAL);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(INTERVAL);
}

void dash() {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(INTERVAL * 3);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(INTERVAL);
}

void task() {
    printf("%s begin\n", __PRETTY_FUNCTION__);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (true) {
        dot();
        dot();
        dot();
        dash();
        dash();
        dash();
        dot();
        dot();
        dot();

        sleep_ms(INTERVAL * 2);
    }
}

int main() {
    bi_decl(bi_program_description("This is a hello-world binary."));
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_LED_PIN, "On-board LED"));

    stdio_init_all();
    sleep_ms(2000);

    task();

    return 0;
}
