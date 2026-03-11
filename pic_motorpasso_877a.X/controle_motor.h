#ifndef CONTROLE_MOTOR_H
#define CONTROLE_MOTOR_H

#include <xc.h> // ESSENCIAL: Para definições de TRISDbits, PORTDbits, CCPxCONbits etc.

// --- Definições de Hardware (PIC16F877A para L298N) ---

//ESQUERDO
#define IN1_MOTOR_ESQUERDO PORTBbits.RB1
#define IN2_MOTOR_ESQUERDO PORTBbits.RB2

//DIREITO
#define IN3_MOTOR_DIREITO  PORTBbits.RB4
#define IN4_MOTOR_DIREITO  PORTBbits.RB5


// --- Protótipos das Funções ---
void inicializar_motor(void);
void motores_parar_freio(void);
void motores_ligar(void);
void motores_frente(void);
void motores_re(void);
void motores_virar_direita(void);
void motores_virar_esquerda(void);

#endif // CONTROLE_MOTOR_H
