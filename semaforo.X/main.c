//**************************************************************************
// SEMAFORO NO PIC 16F628A
//
// ACENDER LEDS
// Autor: Duarte
//
// Compilador: XC8 v3.00
// IDE MPLABX: V6.20 Windows
// Plataforma: ISIS PROTEUS
// Gravador: PICKIT3
//
//**************************************************************************

//**************************************************************************
// Bit fuses
// CONFIG

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//**************************************************************************
// Define frequencia de trabalho
#define _XTAL_FREQ 4000000
//**************************************************************************
#include <xc.h>
//**************************************************************************
// Definições de Módulo
#define LEDr PORTAbits.RA0
#define LEDg PORTAbits.RA1
#define LEDy PORTAbits.RA2

//**************************************************************************
// Função Principal
//
void main(void) {
    TRISA = 0x00;
    PORTA = 0x00;
    CMCON = 0x07;
    
 
    while(1){
        LEDr = 1;
        LEDy = 0;
        LEDg = 0;
        __delay_ms(1000);
        
        LEDr = 0;
        LEDg = 1;
        __delay_ms(2000);
        
        LEDg = 0;
        LEDy = 1;
        __delay_ms(1000);
    }
}