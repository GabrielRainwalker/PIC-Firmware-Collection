// RECEPTOR
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

// Define os pinos para DATA do MX-RM-5V e LED
#ifdef __PIC16F877A__
    #define RX_PIN PORTCbits.RC7  // Pino RC7 no PIC16F877A (entrada)
    #define LED_PIN PORTDbits.RD0 // Pino RD0 no PIC16F877A (saída LED)
#else // Para PIC16F628A
    #define RX_PIN PORTBbits.RB0  // Pino RB0 no PIC16F628A (entrada)
    #define LED_PIN PORTBbits.RB1 // Pino RB1 no PIC16F628A (saída LED)
#endif

void main() {
    // Configura os pinos como entrada (RX) e saída (LED)
    #ifdef __PIC16F877A__
        TRISCbits.TRISC7 = 1;  // RC7 como entrada (RX)
        TRISDbits.TRISD0 = 0;  // RD0 como saída (LED)
        PORTDbits.RD0 = 0;     // LED inicialmente apagado
    #else // Para PIC16F628A
        TRISBbits.TRISB0 = 1;  // RB0 como entrada (RX)
        TRISBbits.TRISB1 = 0;  // RB1 como saída (LED)
        PORTBbits.RB1 = 0;     // LED inicialmente apagado
    #endif
    
    while(1) {
        if (RX_PIN == 1) {     // Detectou sinal HIGH?
            LED_PIN = 1;       // Acende o LED
            __delay_ms(500);   // Mantém aceso por 500ms
            LED_PIN = 0;       // Apaga o LED
        }
        __delay_ms(10);        // Pequeno delay para evitar leituras falsas
    }
}