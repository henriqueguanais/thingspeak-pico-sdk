#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "thingspeak.h"

int main() {
    stdio_init_all();
    // tempo para exibir as mensagens
    sleep_ms(3000);

    // Inicializa Wi-Fi
    if (cyw43_arch_init()) {
        printf("Falha ao iniciar Wi-Fi\n");
        return -1;
    }
    cyw43_arch_enable_sta_mode();

    // Conectar ao Wi-Fi
    printf("Conectando ao Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return -1;
    }
    printf("Wi-Fi conectado!\n");

    // Enviar dado ao ThingSpeak (exemplo: temperatura)
    int temperatura = 0;
    int reservatorio = 0;

    while (1) {
        send_data_to_thingspeak(temperatura, "field1");
        temperatura += 1;

        send_data_to_thingspeak(reservatorio, "field2");
        reservatorio += 2;

        sleep_ms(10000);  // Coletar e enviar dados a cada 10 segundos
    }

    return 0;
}
