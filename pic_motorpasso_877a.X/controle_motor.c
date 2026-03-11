#include <xc.h>             // ESSENCIAL: Incluir no topo de cada .c
#include "controle_motor.h" // Inclui as definições de pinos e protótipos de função

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000 // Cristal de 4MHz
#endif

void inicializar_motor(void) {

    TRISBbits.TRISB1 = 0; 
    TRISBbits.TRISB2 = 0; 
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;

    motores_parar_freio();
}

void motores_ligar(void) {
    IN1_MOTOR_ESQUERDO = 0;
    IN2_MOTOR_ESQUERDO = 0;
    IN3_MOTOR_DIREITO = 0;
    IN4_MOTOR_DIREITO = 0;
}

void motores_parar_freio(void) {
    IN1_MOTOR_ESQUERDO = 1;
    IN2_MOTOR_ESQUERDO = 1;
    IN3_MOTOR_DIREITO = 1;
    IN4_MOTOR_DIREITO = 1;
}

void motores_frente(void) {
    IN1_MOTOR_ESQUERDO = 0;
    IN2_MOTOR_ESQUERDO = 1;
    IN3_MOTOR_DIREITO = 0;
    IN4_MOTOR_DIREITO = 1;
}

void motores_re(void) {
    IN1_MOTOR_ESQUERDO = 1;
    IN2_MOTOR_ESQUERDO = 0;
    IN3_MOTOR_DIREITO = 1;
    IN4_MOTOR_DIREITO = 0;
}

void motores_virar_direita(void) {
    IN1_MOTOR_ESQUERDO = 0;
    IN2_MOTOR_ESQUERDO = 1;
    IN3_MOTOR_DIREITO = 1;
    IN4_MOTOR_DIREITO = 0;
}

void motores_virar_esquerda(void) {
    IN1_MOTOR_ESQUERDO = 1;
    IN2_MOTOR_ESQUERDO = 0;
    IN3_MOTOR_DIREITO = 0;
    IN4_MOTOR_DIREITO = 1;
}