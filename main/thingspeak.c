#include <stdio.h>
#include <string.h>
#include "lwip/ip4_addr.h"
#include "lwip/dns.h"
#include "lwip/err.h"
#include "thingspeak.h"

// Callback para quando a requisição HTTP for concluída
err_t http_request_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err == ERR_OK) {
        printf("Conectado ao servidor!\n");
    } else {
        printf("Erro na conexão: %d\n", err);
    }
    return ERR_OK;
}

// Função para enviar dados ao ThingSpeak
void send_data_to_thingspeak(float value, const char *thingspeak_field) {
    char request[256];
    snprintf(request, sizeof(request), "GET /update?api_key=%s&%s=%.2f HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
             THINGSPEAK_API_KEY, thingspeak_field, value, THINGSPEAK_URL);

    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB TCP\n");
        return;
    }

    ip_addr_t server_ip;
    err_t err = dns_gethostbyname(THINGSPEAK_URL, &server_ip, NULL, NULL);
    if (err == ERR_OK) {
        err = tcp_connect(pcb, &server_ip, 80, http_request_callback);
        if (err == ERR_OK) {
            tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
            tcp_output(pcb);
            printf("Enviando dado ao ThingSpeak: %.2f\n", value);
        }
    }
}