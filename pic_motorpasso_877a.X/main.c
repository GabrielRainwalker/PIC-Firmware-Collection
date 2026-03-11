#pragma config FOSC = HS        // Oscilador de cristal de alta velocidade
#pragma config WDTE = OFF       // Watchdog Timer desligado
#pragma config PWRTE = OFF      // Power-up Timer desligado
#pragma config BOREN = OFF      // Brown-out Reset desligado
#pragma config LVP = OFF        // Programação em baixa tensão desligada (pino RB3 é I/O)
#pragma config CPD = OFF        // Proteção de dados da EEPROM desligada
#pragma config WRT = OFF        // Proteção contra escrita na memória Flash desligada
#pragma config CP = OFF         // Proteção de código da memória Flash desligada

#define _XTAL_FREQ 4000000 // Define a frequência do cristal para 4MHz

#include <xc.h>
#include <string.h>

// Módulos do projeto
#include "controle_motor.h"
#include "feedback_robo.h"
#include "comunicar_esp32.h"

void inicializar_sistema(void) {
    ADCON1 = 0x06;
    CMCON = 0x07;

    inicializar_motor();
    feedback_inicializar();
    comunicar_esp32_inicializar_usart(9600);
}

void main(void) {
    inicializar_sistema();
    
    feedback_indicar_sistema_pronto();
    
    char comando_recebido[TAMANHO_BUFFER_RX_USART];

    while (1) {
        if (comunicar_esp32_comando_recebido_disponivel()) {
            
            feedback_indicar_comunicacao_uart();

            if (comunicar_esp32_ler_comando_recebido(comando_recebido, sizeof(comando_recebido))) {
                
                if (strcmp(comando_recebido, "FRENTE") == 0) {
                    motores_frente();
                    feedback_indicar_movimento_andamento();
                } 
                else if (strcmp(comando_recebido, "RE") == 0) {
                    motores_re();
                    feedback_indicar_movimento_andamento();
                }
                else if (strcmp(comando_recebido, "DIREITA") == 0) {
                    motores_virar_direita();
                    feedback_indicar_movimento_andamento();
                }
                else if (strcmp(comando_recebido, "ESQUERDA") == 0) {
                    motores_virar_esquerda();
                    feedback_indicar_movimento_andamento();
                }
                else if (strcmp(comando_recebido, "PARAR") == 0) {
                    motores_parar_freio();
                    feedback_indicar_sistema_pronto();
                }
                comunicar_esp32_enviar_string("ACK\n");
            }
        }
    }
}