#include <xc.h>             // ESSENCIAL: Inclui as definições do dispositivo PIC16F877A
#include "feedback_robo.h"  // Inclui as novas definições de macros e protótipos

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000 // Cristal de 4MHz
#endif

// TIMER BUZZER
#define DURACAO_BIPE_CURTO_MS 100
#define DURACAO_BIPE_LONGO_MS 500

// Função auxiliar para delay variável
void delay_ms_variavel(unsigned int milissegundos) {
    while (milissegundos > 0) {
        __delay_us(999);
        milissegundos--;
    }
}

void feedback_inicializar(void) {
    // Configurar pinos do LED RGB e Buzzer como SAÍDA (0)
    LED_R_TRIS = 0;
    LED_G_TRIS = 0;
    LED_B_TRIS = 0;
    BUZZER_TRIS = 0;

    // Estado inicial: LED desligado e Buzzer silencioso
    LED_R_OFF();
    LED_G_OFF();
    LED_B_OFF();
    BUZZER_OFF();
}

void feedback_led_rgb_definir_cor(rgb_cor_t cor) {
    // Apaga todas as cores primeiro
    LED_R_OFF();
    LED_G_OFF();
    LED_B_OFF();

    switch (cor) {
        case RGB_VERMELHO: LED_R_ON(); break;
        case RGB_VERDE:    LED_G_ON(); break;
        case RGB_AZUL:     LED_B_ON(); break;
        case RGB_AMARELO:  LED_R_ON(); LED_G_ON(); break;
        case RGB_MAGENTA:  LED_R_ON(); LED_B_ON(); break;
        case RGB_CIANO:    LED_G_ON(); LED_B_ON(); break;
        case RGB_BRANCO:   LED_R_ON(); LED_G_ON(); LED_B_ON(); break;
        case RGB_DESLIGADO: default: break;
    }
}

void feedback_buzzer_controlar(buzzer_padrao_t padrao, unsigned int duracao_ms_on, unsigned int duracao_ms_off, unsigned char repeticoes) {
    unsigned int on_duration = duracao_ms_on;
    unsigned int off_duration = duracao_ms_off;

    switch (padrao) {
        case BUZZER_BIPE_CURTO:
            BUZZER_ON();
            delay_ms_variavel(DURACAO_BIPE_CURTO_MS);
            BUZZER_OFF();
            break;
        case BUZZER_BIPE_LONGO:
            BUZZER_ON();
            delay_ms_variavel(DURACAO_BIPE_LONGO_MS);
            BUZZER_OFF();
            break;
        case BUZZER_ALERTA_CONTINUO:
            BUZZER_ON();
            break;
        case BUZZER_SILENCIO:
        default:
            BUZZER_OFF();
            break;
    }
}

void feedback_buzzer_parar(void) {
    BUZZER_OFF();
}

void feedback_indicar_erro_critico(void) {
    feedback_led_rgb_definir_cor(RGB_VERMELHO);
    feedback_buzzer_controlar(BUZZER_ALERTA_CONTINUO, 0, 0, 0);
}

void feedback_indicar_sistema_pronto(void) {
    feedback_led_rgb_definir_cor(RGB_VERDE);
    feedback_buzzer_controlar(BUZZER_BIPE_CURTO, 0, 0, 0); 
}

void feedback_indicar_modo_espera(void) {
    feedback_led_rgb_definir_cor(RGB_AZUL);
    feedback_buzzer_parar();
}

void feedback_indicar_movimento_andamento(void) {
    feedback_led_rgb_definir_cor(RGB_MAGENTA);
    feedback_buzzer_parar(); // 
}

void feedback_indicar_comunicacao_uart(void) {
    feedback_led_rgb_definir_cor(RGB_AMARELO); // 
    feedback_buzzer_controlar(BUZZER_BIPE_CURTO, 0, 0, 0);
}

void feedback_indicar_aviso_processamento(void) {
    feedback_led_rgb_definir_cor(RGB_AMARELO);
    feedback_buzzer_controlar(BUZZER_ALERTA_INTERMITENTE, 150, 400, 3); 
}

void feedback_indicar_modo_especial(void) {
    feedback_led_rgb_definir_cor(RGB_MAGENTA);
    feedback_buzzer_parar();
}

void feedback_indicar_falha_grave_imediata(void) {
    feedback_led_rgb_definir_cor(RGB_VERMELHO);
    feedback_buzzer_controlar(BUZZER_ALERTA_INTERMITENTE, 100, 100, 10);
}

void feedback_indicar_entrega_correta(void) {
    feedback_led_rgb_definir_cor(RGB_BRANCO);
    feedback_buzzer_controlar(BUZZER_BIPES_CONFIRMACAO, 0, 0, 0);
}

void feedback_indicar_estado_desligado(void) {
    feedback_led_rgb_definir_cor(RGB_DESLIGADO);
    feedback_buzzer_parar();
}
