/*
 * File:   main.c
 * Author: gduar
 *
 * Created on 3 de Abril de 2025, 11:02
 */

// CONFIG
#pragma config FOSC = XT // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#define _XTAL_FREQ 4000000
#define LED PORTBbits.RB0

volatile uint8_t temporizador = 0;

volatile struct {
    volatile uint8_t SEG;
    volatile uint8_t MIN;
} relogio = {0, 0};


void main(void) {
    TRISB = 0x00;
    PORTB = 0x00;
    
    iniciar_timer0();
    
    while (1){
    LCD_SetCursor(0,0);
    LCD_Print(relogio.SEG);
    LCD.SetCursor(0,1);
    LCD.Print(relogio.MIN);
    }
}

void __interrupt() isr() {  
    if (INTCONbits.T0IF) {
        temporizador++;
        if(temporizador >= 15){
            relogio.SEG++;  
            temporizador = 0;
        }
        if(relogio.SEG >= 60) {
            relogio.SEG = 0;
            relogio.MIN++;
        }
        TMR0 = 0;            // Ajusta para overflow a cada ~65.5ms[7][12]  
        INTCONbits.T0IF = 0;  // Limpa flag de interrupção[14]  
    }  
}  


void iniciar_timer0() {  
    OPTION_REG = 0b00000111; // Prescaler 1:256, clock interno[6][14]  
    TMR0 = 0;                // Inicia contagem em 0  
    INTCONbits.T0IE = 1;     // Habilita interrupção do Timer0[14]  
    INTCONbits.GIE = 1;      // Habilita interrupções globais[14]  
}  
