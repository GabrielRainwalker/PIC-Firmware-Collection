/*
 * File:   main.c
 * Author: gduar
 *
 * Created on 28 de Abril de 2025, 08:54
 */


#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000

// Configurações do PIC (ajuste conforme seu hardware)
#pragma config FOSC = XT        // Oscilador externo (cristal)
#pragma config WDTE = OFF       // Watchdog desativado
#pragma config PWRTE = OFF      // Power-up Timer desativado
#pragma config MCLRE = ON      // MCLR desabilitado
#pragma config BOREN = OFF      // Brown-out Reset desativado
#pragma config LVP = OFF        // Low Voltage Programming desativado
#pragma config CPD = OFF        // Proteção de dados desativada
#pragma config CP = OFF         // Proteção de código desativada

/*************** TRANSMISSOR (PIC16F628A) ***************/

/*
#define TX_PIN RB0

// Função Manchester para bit '1': 01, bit '0': 10
void manchester_send(unsigned char bit) {
    TX_PIN = !bit;      // Primeira metade: complemento
    __delay_us(500);
    TX_PIN = bit;       // Segunda metade: bit real
    __delay_us(500);
}

void send_command(unsigned char cmd) {
    // Pré-amplo de sincronização
    for(int i=0; i<8; i++) manchester_send(1);
    
    // Envio do comando
    for(int i=0; i<8; i++) {
        manchester_send((cmd >> i) & 0x01);
    }
    
    // Checksum XOR simples
    unsigned char checksum = cmd ^ 0xFF;
    for(int i=0; i<8; i++) {
        manchester_send((checksum >> i) & 0x01);
    }
}

void main() {
    TRISB = 0x00;  // RB0 como saída
    CMCON = 0x07;   // Desativa comparadores
    
    while(1) {
        send_command(0xAA); // Liga LED
        __delay_ms(2000);
        send_command(0x55); // Desliga LED
        __delay_ms(2000);
    }
}
*/

/*************** RECEPTOR (PIC16F628A) **************/

#define RX_PIN RA0
#define LED_PIN RB7 

unsigned char manchester_read() {
    unsigned char data = 0;
    for(int i=0; i<8; i++) {
        while(RX_PIN == 0); // Espera borda de subida
        __delay_us(250);    // Amostra no meio do bit
        unsigned char bit = RX_PIN;
        __delay_us(250);
        data |= (bit << i);
    }
    return data;
}

void main() {
    TRISA = 0xFF;   // RA0 como entrada
    TRISB = 0x00;   // RB7 como saída
    CMCON = 0x07;    // Desativa comparadores

    
    while(1) {
        if(RX_PIN == 1) { // Detecção de sinal
            unsigned char cmd = manchester_read();
            unsigned char checksum = manchester_read();
            
            if((cmd ^ checksum) == 0xFF) {
                if(cmd == 0xAA) LED_PIN = 1;
                if(cmd == 0x55) LED_PIN = 0;
            }
        }
        __delay_us(100);
    }
}