#include "comunicar_esp32.h" // Inclui os protótipos e definições do nosso header
#include <string.h>          // Para strcpy, strlen (se necessário, mas tentaremos evitar no PIC)

// Defina a frequência do cristal para as funções de delay do XC8 (se usadas)
// e para o cálculo do baud rate se não for passado como parâmetro.
#ifndef _XTAL_FREQ // Adicionado para garantir que _XTAL_FREQ seja definido se não estiver em outro lugar
#define _XTAL_FREQ 4000000 // Cristal de 4MHz
#endif

// --- Variáveis Internas do Módulo ---

// Buffer para armazenar os bytes recebidos via USART
static volatile char buffer_rx_usart[TAMANHO_BUFFER_RX_USART];
// Índice para a próxima posição livre no buffer_rx_usart
static volatile unsigned char indice_buffer_rx_usart = 0;
// Flag que indica se um comando completo (terminado pelo delimitador) foi recebido
static volatile bool flag_comando_disponivel = false;

// --- Implementação da Rotina de Serviço de Interrupção (ISR) ---

// A ISR é chamada automaticamente quando ocorre uma interrupção.
void __interrupt() usart_isr(void) {
    // Verifica se a interrupção foi causada pela recepção USART (RCIF)
    if (PIE1bits.RCIE && PIR1bits.RCIF) {
        char byte_recebido;

        // Verifica erros de Overrun (OERR)
        if (RCSTAbits.OERR) {
            RCSTAbits.CREN = 0; // Limpa o erro de overrun resetando o receptor contínuo
            RCSTAbits.CREN = 1; // Reabilita o receptor contínuo
        }

        // Verifica erros de Framing (FERR)
        if (RCSTAbits.FERR) {
            byte_recebido = RCREG; // Lê o byte para limpar FERR, mas pode estar corrompido
        } else {
            byte_recebido = RCREG; // Lê o byte recebido do registrador RCREG
        }

        if (!flag_comando_disponivel) {
            if (byte_recebido == DELIMITADOR_COMANDO_USART) {
                if (indice_buffer_rx_usart < TAMANHO_BUFFER_RX_USART) {
                    buffer_rx_usart[indice_buffer_rx_usart] = '\0';
                    flag_comando_disponivel = true;
                } else {
                    buffer_rx_usart[TAMANHO_BUFFER_RX_USART - 1] = '\0';
                    flag_comando_disponivel = true;
                }
            } else if (byte_recebido == '\r') {
                // Ignora Carriage Return
            } else if (indice_buffer_rx_usart < (TAMANHO_BUFFER_RX_USART - 1)) {
                buffer_rx_usart[indice_buffer_rx_usart++] = byte_recebido;
            }
        }
    }
}


// --- Implementação das Funções Públicas ---

void comunicar_esp32_inicializar_usart(long baud_rate) {
    TRISCbits.TRISC7 = 1; // RX como entrada
    TRISCbits.TRISC6 = 0; // TX como saída

    unsigned int spbrg_valor_calc;

    TXSTAbits.BRGH = 1;

    if (baud_rate > 0) {
        // CORRIGIDO: Casts explícitos para evitar avisos e garantir precisão
        spbrg_valor_calc = (unsigned int)(((double)_XTAL_FREQ / (16.0 * (double)baud_rate)) - 1.0);
    } else {
        // Padrão para 9600bps com Fosc=4MHz, BRGH=1 ( (4000000 / (16 * 9600)) - 1 = 25.04 -> 25)
        spbrg_valor_calc = 25;
    }
    
    // CORRIGIDO: Cast explícito para unsigned char
    SPBRG = (unsigned char)spbrg_valor_calc;

    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;

    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    indice_buffer_rx_usart = 0;
    flag_comando_disponivel = false;
    
    // Limpeza inicial do RCREG
    char dummy;
    if(PIR1bits.RCIF) dummy = RCREG;
    if(PIR1bits.RCIF) dummy = RCREG;
    if(PIR1bits.RCIF) dummy = RCREG;
}

void comunicar_esp32_enviar_byte(uint8_t dado) {
    while (!TXSTAbits.TRMT);
    TXREG = dado;
}

void comunicar_esp32_enviar_string(const char* str) {
    while (*str) {
        comunicar_esp32_enviar_byte(*str++);
    }
}

bool comunicar_esp32_comando_recebido_disponivel(void) {
    return flag_comando_disponivel;
}

bool comunicar_esp32_ler_comando_recebido(char* buffer_destino, unsigned char tamanho_max_buffer) {
    if (flag_comando_disponivel) {
        INTCONbits.GIE = 0; // Desabilita interrupções globais

        unsigned char tamanho_comando = indice_buffer_rx_usart;
        if (tamanho_comando >= tamanho_max_buffer) {
            tamanho_comando = tamanho_max_buffer - 1;
        }

        for (unsigned char i = 0; i < tamanho_comando; i++) {
            buffer_destino[i] = buffer_rx_usart[i];
        }
        buffer_destino[tamanho_comando] = '\0';

        indice_buffer_rx_usart = 0;
        flag_comando_disponivel = false;

        INTCONbits.GIE = 1; // Reabilita interrupções globais
        return true;
    }
    return false;
}
