#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "string.h"

#define LED_PIN         PICO_DEFAULT_LED_PIN

#if !defined WIFI_STA_SSID
#error "WIFI_STA_SSID is not defined"
#endif
#if !defined WIFI_STA_PASSWORD
#error "WIFI_STA_PASSWORD is not defined"
#endif

class StringBuffer {
private:
    char *buffer;
    char *ptr;

public:
    StringBuffer(size_t size) {
        buffer = new char[size];
        reset();
    }

    ~StringBuffer() {
        delete buffer;
    }

    void reset() {
        ptr = buffer;
        *ptr = '\0';
    }

    void putc(char c) {
        *ptr++ = c;
        *ptr = '\0';
    };

    const char *gets() {
        return buffer;
    }
};


bool esp_send(const char *command, const char *expect_ack, uint32_t timeout_ms = 2000) {
    uart_puts(uart0, command);
    uart_puts(uart0, "\r\n");
    printf("> %s\n", command);

    StringBuffer rx(128);

    absolute_time_t timeout_time = make_timeout_time_ms(timeout_ms);
    while (!time_reached(timeout_time)) {
        if (!uart_is_readable(uart0)) {
            continue;
        }

        uint8_t c = uart_getc(uart0);

        switch (c) {
            case '\r':
                break;
            case '\n':
                printf("< %s\n", rx.gets());

                if (strcmp(expect_ack, rx.gets()) == 0) {
                    return true;
                }

                rx.reset();
                break;
            default:
                rx.putc(c);
                break;
        }
    }

    return false;
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_put(LED_PIN, 1);
    sleep_ms(2000);
    gpio_put(LED_PIN, 0);

    uart_init(uart0, 115200);
    gpio_init(0);
    gpio_init(ESP8285_RX_PIN);
    gpio_init(ESP8285_TX_PIN);
    gpio_set_function(ESP8285_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(ESP8285_TX_PIN, GPIO_FUNC_UART);

    uart_puts(uart0, "+++");
    sleep_ms(1000);

    esp_send("AT", "OK", 2000);
    esp_send("AT+CWMODE=3", "OK", 2000);
    esp_send(
            "AT+CWJAP=\"" WIFI_STA_SSID "\",\"" WIFI_STA_PASSWORD "\"",
            "OK",
            20000
    );

    esp_send("AT+CIFSR", "OK");

    // make tcp connection
    esp_send("AT+CIPSTART=\"TCP\",\"35.169.171.109\",80", "OK", 10000);
    esp_send("AT+CIPMODE=1", "OK");
    esp_send("AT+CIPSEND", ">");

    // send HTTP request to TCP connection
    uart_puts(uart0, "GET /get HTTP/1.0\n");
    uart_puts(uart0, "Host: httpbin.org\n");
    uart_puts(uart0, "Accept: */*\n");
    uart_puts(uart0, "\n");

    // read char from TCP and put it to stdout
    for (;;) {
        if (uart_is_readable(uart0)) {
            uint8_t c = uart_getc(uart0);
            putchar(c);
        }
    }

    return 0;
}
