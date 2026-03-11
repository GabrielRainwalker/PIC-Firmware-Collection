#ifndef COMUNICAR_ESP32_H
#define COMUNICAR_ESP32_H

#include <xc.h>      
#include <stdbool.h> 
#include <stdint.h>  

#define TAMANHO_BUFFER_RX_USART 32

#define DELIMITADOR_COMANDO_USART '\n'


// --- Protótipos das Funções ---
void comunicar_esp32_inicializar_usart(long baud_rate);
void comunicar_esp32_enviar_byte(uint8_t dado);
void comunicar_esp32_enviar_string(const char* str);
bool comunicar_esp32_comando_recebido_disponivel(void);
bool comunicar_esp32_ler_comando_recebido(char* buffer_destino, unsigned char tamanho_max_buffer);
#endif // COMUNICAR_ESP32_H