#ifndef FEEDBACK_ROBO_H
#define FEEDBACK_ROBO_H

#include <xc.h>    
#include <stdbool.h>

// LED

#define LED_R_TRIS  TRISDbits.TRISD5
#define LED_R_PIN   PORTDbits.RD5

#define LED_G_TRIS  TRISDbits.TRISD7
#define LED_G_PIN   PORTDbits.RD7

#define LED_B_TRIS  TRISDbits.TRISD6
#define LED_B_PIN   PORTDbits.RD6

// BUZZER
#define BUZZER_TRIS TRISDbits.TRISD1
#define BUZZER_PIN  PORTDbits.RD1

// MACROS
// LED
#define LED_R_ON()    (LED_R_PIN = 1)
#define LED_R_OFF()   (LED_R_PIN = 0)

#define LED_G_ON()    (LED_G_PIN = 1)
#define LED_G_OFF()   (LED_G_PIN = 0)

#define LED_B_ON()    (LED_B_PIN = 1)
#define LED_B_OFF()   (LED_B_PIN = 0)

// BUZZER
#define BUZZER_ON()   (BUZZER_PIN = 1)
#define BUZZER_OFF()  (BUZZER_PIN = 0)

// --- Enumeração para Cores do LED RGB ---
typedef enum {
    RGB_DESLIGADO, RGB_VERMELHO, RGB_VERDE, RGB_AZUL,
    RGB_AMARELO, RGB_MAGENTA, RGB_CIANO, RGB_BRANCO
} rgb_cor_t;

// --- Enumeração para Padrões de Buzzer ---
typedef enum {
    BUZZER_SILENCIO, BUZZER_BIPE_CURTO, BUZZER_BIPE_LONGO,
    BUZZER_BIPES_CONFIRMACAO, BUZZER_ALERTA_CONTINUO, BUZZER_ALERTA_INTERMITENTE
} buzzer_padrao_t;

// FUNÇÕES
void feedback_inicializar(void);
void feedback_led_rgb_definir_cor(rgb_cor_t cor);
void feedback_buzzer_controlar(buzzer_padrao_t padrao, unsigned int duracao_ms_on, unsigned int duracao_ms_off, unsigned char repeticoes);
void feedback_buzzer_parar(void);

// FUNÇÃO GLOBAL
void feedback_indicar_erro_critico(void);
void feedback_indicar_sistema_pronto(void);
void feedback_indicar_modo_espera(void);

// FEEDBACK OPERAÇÃO
void feedback_indicar_movimento_andamento(void);
void feedback_indicar_comunicacao_uart(void);
void feedback_indicar_aviso_processamento(void);
void feedback_indicar_modo_especial(void);

// ALERTA
void feedback_indicar_falha_grave_imediata(void);
void feedback_indicar_entrega_correta(void);
void feedback_indicar_estado_desligado(void);

#endif // FEEDBACK_ROBO_H