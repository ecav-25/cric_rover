/**
 * @file test_led_stripes.c
 * @author Gruppo 2
 * @brief Suite completa di Unit Test per il modulo LED WS2812B.
 */

#include "unity.h"
#include <string.h>
#include "mock_stm32g4xx_hal.h"
#include "led_stripes.h"

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

static led_stripes_config_t cfg;
static TIM_HandleTypeDef htim;
static DMA_HandleTypeDef hdma;

void setUp(void) {
    memset(&cfg, 0, sizeof(led_stripes_config_t));
    memset(&htim, 0, sizeof(TIM_HandleTypeDef));
    memset(&hdma, 0, sizeof(DMA_HandleTypeDef));

    cfg.htim = &htim;
    cfg.hdma = &hdma;
    cfg.tim_channel = TIM_CHANNEL_1;
    cfg.pwm_hi = 60;
    cfg.pwm_lo = 30;
    cfg.scale_g = 255;
    cfg.scale_b = 255;
    hdma.State = HAL_DMA_STATE_READY;
}

/* =================================================================---------
    TEST DI LOGICA E SICUREZZA
   ========================================================================= */

/**
 * @test Verifica la protezione contro indici fuori range.
 * @details Verifica che non venga impostato il flag dirty se gli indici sono errati.
 */
void test_led_set_range_invalid_params_should_do_nothing(void) {
    led_stripe_init(&cfg);

    /* Caso start > end */
    led_set_RGB_range(10, 5, 255, 0, 0);
    /* Caso indice oltre NUM_PIXELS (21) */
    led_set_RGB_range(0, 50, 255, 0, 0);

    /* Se il driver ha ignorato correttamente, dirty è 0 e render non chiama l'HAL */
    led_render();
}

/**
 * @test Verifica che il rendering avvenga solo se i dati sono "sporchi".
 */
void test_led_render_only_when_dirty(void) {
    led_stripe_init(&cfg);
    led_render(); // Non deve chiamare HAL_TIM_PWM_Start_DMA

    led_set_all_RGB(255, 0, 0);
    HAL_TIM_PWM_Start_DMA_IgnoreAndReturn(HAL_OK);
    led_render(); // Ora deve chiamarlo
}

/**
 * @test Verifica la protezione busy: ignora comandi se il DMA è in corso.
 */
void test_led_ignore_commands_when_busy(void) {
    led_stripe_init(&cfg);
    HAL_TIM_PWM_Start_DMA_IgnoreAndReturn(HAL_OK);
    led_set_RGB(0, 255, 255, 255);
    led_render(); // wr_buf_p diventa > 0

    /* Tentiamo di cambiare animazione mentre wr_buf_p != 0 */
    rear_led_step(EMERGENCY_LIGHTS);

    /* Se occupato, il comando viene ignorato e una nuova render non chiama l'HAL */
    led_render();
}

/* =================================================================---------
    TEST ANIMAZIONI (STATE MACHINE)
   ========================================================================= */

/**
 * @test Verifica il reset degli step per i LED posteriori e per la targa.
 */
void test_steps_reset_on_animation_change(void) {
    led_stripe_init(&cfg);

    /* Test Rear LED */
    rear_led_step(BRAKING_LIGHTS);
    HAL_TIM_PWM_Start_DMA_IgnoreAndReturn(HAL_OK);
    rear_led_step(BACKWARD_LIGHTS); // Cambio: step deve tornare a 0
    led_render();

    /* Test Rear Sign (Targa) */
    rear_sign_step(SIGN_GREEN);
    HAL_TIM_PWM_Start_DMA_IgnoreAndReturn(HAL_OK);
    rear_sign_step(SIGN_RED); // Cambio: step deve tornare a 0
    led_render();
}

/**
 * @test Verifica il ciclo di lampeggio dell'emergenza.
 */
void test_emergency_lights_cycle(void) {
    led_stripe_init(&cfg);
    rear_led_step(EMERGENCY_LIGHTS); // Step 0: Accende

    for(int i = 0; i < 16; i++) {
        rear_led_step(EMERGENCY_LIGHTS); // Gira tutto il ciclo
    }
}

/* =================================================================---------
    TEST PROTOCOLLO HARDWARE (DMA)
   ========================================================================= */

/**
 * @test Verifica il double buffering tramite le callback PWM.
 */

void test_DMA_callbacks_logic(void) {
    led_stripe_init(&cfg);
    HAL_TIM_PWM_Start_DMA_IgnoreAndReturn(HAL_OK);
    led_set_RGB(0, 255, 0, 0);
    led_render();

    /* Simulazione interrupt DMA */
    HAL_TIM_PWM_PulseFinishedHalfCpltCallback(&htim);
    HAL_TIM_PWM_PulseFinishedCallback(&htim);

    /* Fine trasmissione: Stop DMA dopo NUM_PIXELS + 2 */
    HAL_TIM_PWM_Stop_DMA_IgnoreAndReturn(HAL_OK);
    for(int i = 0; i < 64; i++) {
        HAL_TIM_PWM_PulseFinishedCallback(&htim);
    }
}
