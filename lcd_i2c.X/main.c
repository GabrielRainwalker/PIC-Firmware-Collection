/*
 * PIC16F628A + LCD I2C (PCF8574) + DHT11
 * Conexões:
 * - RA1 (SCL) ? PCF8574 SCL
 * - RA2 (SDA) ? PCF8574 SDA
 * Endereço I2C: 0x27 (A0-A2=GND)
 */

#include <xc.h>
#include <stdio.h>  // Para sprintf
#define _XTAL_FREQ 4000000UL

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// Definição dos pinos I2C
#define SCL_TRIS TRISC4
#define SDA_TRIS TRISC3
#define SCL_PIN  RC4
#define SDA_PIN  RC3
#define SDA_PORT PORTC
#define SDA_BIT  3             // RA2 é o bit 2 do PORTA

// Endereço I2C do PCF8574 (A0-A2=GND ? 0x27)
#define I2C_ADDR 0x27

// Mapeamento PCF8574 ? LCD (conexões físicas)
#define RS  0  // P0 do PCF8574
#define RW  1  // P1 do PCF8574 (adicionado)
#define EN  2  // P2 do PCF8574
#define BL  3  // P3 do PCF8574 (backlight)
#define D4  4  // P4 do PCF8574
#define D5  5  // P5 do PCF8574
#define D6  6  // P6 do PCF8574
#define D7  7  // P7 do PCF8574

// Protótipos de função - I2C e LCD
void I2C_Init();
void I2C_Start();
void I2C_Stop();
uint8_t I2C_Write(uint8_t data);
void LCD_Send(uint8_t value, uint8_t mode);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_Init();
void LCD_Print(const char *str);
void LCD_SetCursor(uint8_t linha, uint8_t coluna);

void main(void) {
    ADCON1 = 0x0F; 
    CMCON = 0x07;       // Desabilita comparadores
    TRISA = 0x00;       // PORTA como saída
    TRISB = 0x00;      // PORTB como saída inicialmente
    TRISC = 0x00;
    TRISD = 0x00;
    PORTA = 0x00;       // Inicializa portas
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    __delay_ms(100);    // Aguarda estabilização
    
    I2C_Init();         // Inicializa comunicação I2C
    __delay_ms(100);    // Aguarda estabilização
    
    LCD_Init();         // Configura LCD
    
    LCD_SetCursor(0, 0);
    LCD_Print("Testando");
    LCD_SetCursor(1, 0);
    LCD_Print("LCD PIC16F877A");
    __delay_ms(1000);
    LCD_Command(0x01);
    __delay_ms(10);
    
    __delay_ms(1000);   // Aguarda estabilização do DHT11
    
    while(1) {
    LCD_Print("Testando");
    LCD_SetCursor(1, 0);
    LCD_Print("LCD PIC16F877A");   
    }
}

// ==============================================
// Implementação das funções I2C (bit-banging)
// ==============================================

void I2C_Init() {
    SCL_TRIS = 0;   // SCL como saída
    SDA_TRIS = 0;   // SDA inicialmente como saída
    SCL_PIN = 1;    // Linhas inicialmente altas
    SDA_PIN = 1;
    __delay_ms(10); // Aguarda estabilização
}

void I2C_Start() {
    // Condição de START: SDA passa de 1 para 0 com SCL em 1
    SDA_TRIS = 0;   // SDA como saída
    SDA_PIN = 1; __delay_us(10);
    SCL_PIN = 1; __delay_us(10);
    SDA_PIN = 0; __delay_us(10);
    SCL_PIN = 0; __delay_us(10);
}

void I2C_Stop() {
    // Condição de STOP: SDA passa de 0 para 1 com SCL em 1
    SDA_TRIS = 0;   // SDA como saída
    SDA_PIN = 0; __delay_us(10);
    SCL_PIN = 1; __delay_us(10);
    SDA_PIN = 1; __delay_us(10);
}

uint8_t I2C_Write(uint8_t data) {
    uint8_t i, ack;
    
    SDA_TRIS = 0;   // SDA como saída
    
    // Envia os 8 bits, MSB primeiro
    for(i = 0; i < 8; i++) {
        SDA_PIN = (data & 0x80) ? 1 : 0;  // MSB primeiro
        __delay_us(5);
        SCL_PIN = 1; __delay_us(10);
        SCL_PIN = 0; __delay_us(5);
        data <<= 1;
    }
    
    // Leitura do ACK
    SDA_TRIS = 1;       // SDA como entrada para receber ACK
    __delay_us(5);
    SCL_PIN = 1; 
    __delay_us(10);
    ack = (SDA_PORT & (1 << SDA_BIT)) ? 1 : 0;  // Lê ACK (0 = ACK, 1 = NACK)
    SCL_PIN = 0;
    __delay_us(10);
    SDA_TRIS = 0;       // SDA como saída novamente
    
    return ack;         // Retorna o status do ACK (0 = ACK recebido)
}

// ==============================================
// Implementação das funções do LCD
// ==============================================

void LCD_Send(uint8_t value, uint8_t mode) {
    uint8_t high_nibble, low_nibble, data;
    
    high_nibble = value & 0xF0;           // Primeiros 4 bits (D7-D4)
    low_nibble = (value << 4) & 0xF0;     // Últimos 4 bits (D3-D0) deslocados
    
    // Backlight sempre ligado (bit BL = 1)
    data = high_nibble | (mode << RS) | (0 << RW) | (1 << BL);
    
    // Envia nibble alto com pulso em EN
    I2C_Start();
    I2C_Write(I2C_ADDR << 1);      // Endereço + Write bit (0)
    I2C_Write(data);               // RS, RW, dados, sem EN
    __delay_us(1);
    I2C_Write(data | (1 << EN));   // EN = 1 (pulso)
    __delay_us(5);
    I2C_Write(data);               // EN = 0
    
    // Envia nibble baixo com pulso em EN
    data = low_nibble | (mode << RS) | (0 << RW) | (1 << BL);
    I2C_Write(data);               // RS, RW, dados, sem EN
    __delay_us(1);
    I2C_Write(data | (1 << EN));   // EN = 1 (pulso)
    __delay_us(5);
    I2C_Write(data);               // EN = 0
    I2C_Stop();
    
    __delay_us(50);                // Tempo para processamento do comando
}

void LCD_Command(uint8_t cmd) {
    LCD_Send(cmd, 0);  // RS=0 para comando
}

void LCD_Data(uint8_t data) {
    LCD_Send(data, 1);  // RS=1 para dados
}

void LCD_Init() {
    __delay_ms(100);  // Aguarda estabilização após power-on
    
    // Sequência de inicialização para modo 4 bits (conforme datasheet)
    // Envio direto de comandos (sem verificar BUSY)
    I2C_Start();
    I2C_Write(I2C_ADDR << 1);
    
    // Garante backlight ligado
    I2C_Write(0x08);  // Backlight ON, RS=RW=EN=0
    I2C_Stop();
    __delay_ms(50);
    
    // Reset inicial (3 vezes 0x03)
    I2C_Start();
    I2C_Write(I2C_ADDR << 1);
    
    // Primeiro comando 0x03
    I2C_Write(0x30 | (1 << BL));
    __delay_us(1);
    I2C_Write(0x30 | (1 << EN) | (1 << BL));
    __delay_us(5);
    I2C_Write(0x30 | (1 << BL));
    __delay_ms(5);
    
    // Segundo comando 0x03
    I2C_Write(0x30 | (1 << BL));
    __delay_us(1);
    I2C_Write(0x30 | (1 << EN) | (1 << BL));
    __delay_us(5);
    I2C_Write(0x30 | (1 << BL));
    __delay_ms(1);
    
    // Terceiro comando 0x03
    I2C_Write(0x30 | (1 << BL));
    __delay_us(1);
    I2C_Write(0x30 | (1 << EN) | (1 << BL));
    __delay_us(5);
    I2C_Write(0x30 | (1 << BL));
    __delay_ms(1);
    
    // Configura modo 4 bits (0x02)
    I2C_Write(0x20 | (1 << BL));
    __delay_us(1);
    I2C_Write(0x20 | (1 << EN) | (1 << BL));
    __delay_us(5);
    I2C_Write(0x20 | (1 << BL));
    I2C_Stop();
    
    __delay_ms(1);
    
    // Agora já podemos usar a função LCD_Command
    LCD_Command(0x28);  // Function set: 2 linhas, 5x8 pontos, modo 4 bits
    LCD_Command(0x0C);  // Display ligado, cursor off, blink off
    LCD_Command(0x06);  // Entry mode: incremento automático, display não desloca
    LCD_Command(0x01);  // Clear display
    __delay_ms(2);      // Clear precisa de mais tempo
}

void LCD_Print(const char *str) {
    while(*str) {
        LCD_Data(*str++);
    }
}

void LCD_SetCursor(uint8_t linha, uint8_t coluna) {
    uint8_t posicao;
    
    // Calcula endereço DDRAM baseado na linha e coluna
    if (linha == 0) {
        posicao = 0x80 + coluna;  // Primeira linha começa em 0x80
    } else {
        posicao = 0xC0 + coluna;  // Segunda linha começa em 0xC0
    }
    
    LCD_Command(posicao);
}