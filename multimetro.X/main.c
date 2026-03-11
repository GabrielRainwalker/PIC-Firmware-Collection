/*
 * File:   main.c
 * Author: gduar
 *
 * Created on 12 de Maio de 2025, 08:22
 */

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Mapeamento dos segmentos (ânodo comum)
const uint8_t seg7[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Inicializa ADC
void init_adc(void) {
    ADCON0 = 0b01000001; // ADON=1, Fosc/8, canal AN0
    ADCON1 = 0b10000000; // Justifica à direita, Vref+=Vdd, Vref-=Vss[1]
}

// Lê canal ADC selecionado em ADCON0<5:3>
uint16_t read_adc(void) {
    GO_nDONE = 1;
    while (GO_nDONE);
    return (ADRESH << 8) | ADRESL;
}

// Exibe um valor de 0?9999 no display multiplexado
void display_value(uint16_t value) {
    uint8_t d, digit;
    for (d = 0; d < 4; d++) {
        digit = (value / (uint16_t)pow(10, 3 - d)) % 10;
        PORTB = seg7[digit];
        PORTC = ~(1 << d); // Ativa dígito d (ânodo comum)<4>
        __delay_ms(2);
    }
}

void main(void) {
    uint16_t raw;
    float medida;
    uint16_t exib;

    // Configurações iniciais
    init_adc();
    TRISB = 0x00;   // PORTB como saída (segmentos)
    TRISC = 0xF0;   // RC0?RC3 saída (ânodos), RC4?RC7 inputs
    TRISD = 0xFF;   // PORTD para chaves de seleção
    ANSEL = 0x03;   // AN0 e AN1 analógicos
    ANSELH = 0x00;

    while (1) {
        // Modo 0: Tensão DC (AN0)
        if (!RD0) {
            ADCON0 = (ADCON0 & 0xC7) | (0 << 3); // canal AN0
            raw = read_adc();
            medida = (raw * 5.0f / 1023.0f) * ((100.0f + 20.0f) / 20.0f); // divisor 100k/20k[2]
            exib = (uint16_t)(medida * 100); // exibe em centésimos de volt
        }
        // Modo 1: Corrente DC (AN1)
        else if (!RD1) {
            ADCON0 = (ADCON0 & 0xC7) | (1 << 3); // canal AN1
            raw = read_adc();
            medida = (raw * 5.0f / 1023.0f) / (10.0f * 0.1f); // ganho=10, Rshunt=0,1?[1]
            exib = (uint16_t)(medida * 1000); // em mA
        }
        // Modo 2: Resistência (pont. de 1 k? em AN0, unknown em AN1)
        else if (!RD2) {
            // mede tensão em Rknown = 1 k?
            ADCON0 = (ADCON0 & 0xC7) | (0 << 3);
            raw = read_adc();
            float Vref = raw * 5.0f / 1023.0f;
            // mede tensão em Runknown
            ADCON0 = (ADCON0 & 0xC7) | (1 << 3);
            uint16_t raw2 = read_adc();
            float Vunk = raw2 * 5.0f / 1023.0f;
            medida = (Rknown * Vunk) / (Vref - Vunk); // Runk = Rk * Vunk/(Vref?Vunk)[5]
            exib = (uint16_t)medida;
        }
        else {
            exib = 0;
        }
        display_value(exib);
    }
}