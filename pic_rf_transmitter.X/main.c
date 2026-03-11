// TRASMISSOR
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 8000000

#include <xc.h>

// Define o pino DATA do FS1000A
#ifdef __PIC16F877A__
    #define TX_PIN PORTCbits.RC6  // Pino RC6 no PIC16F877A
#else // Para PIC16F628A
    #define TX_PIN PORTBbits.RB0  // Pino RB0 no PIC16F628A
#endif

void main() {
    // Configura o pino como saída
    #ifdef __PIC16F877A__
        TRISCbits.TRISC6 = 0;  // RC6 como saída
        PORTCbits.RC6 = 0;     // Inicia em LOW
    #else // Para PIC16F628A
        TRISBbits.TRISB0 = 0;  // RB0 como saída
        PORTBbits.RB0 = 0;     // Inicia em LOW
    #endif
    
    while(1) {
        TX_PIN = 1;         // Liga o transmissor (envia sinal HIGH)
        __delay_ms(500);    // Mantém o sinal por 500ms
        TX_PIN = 0;         // Desliga o transmissor
        __delay_ms(500);    // Aguarda 500ms antes de enviar novamente
    }
}