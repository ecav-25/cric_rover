/**
 * @file test_led.c
 * @author Gruppo 2
 * @brief Unit Tests per il modulo di gestione LED onboard (GPIO).
 *
 * @details Questa suite di test valida la logica di controllo dei LED di stato.
 * Vengono verificati i driver GPIO, la corretta transizione della macchina a stati
 * interna (FSM) e il timing software dei lampeggi (blinking) basato sugli step.
 */

#include "unity.h"
#include <string.h>
#include "mock_stm32g4xx_hal.h"
#include "led.h"

/** @name Risorse Hardware Mockate
 * @{ */
static led_t led_obj;                                      /**< Istanza dell'oggetto LED sotto test */
static GPIO_TypeDef* ports[LED_COUNT] = {
    (GPIO_TypeDef*)0x1000,
    (GPIO_TypeDef*)0x2000
}; /**< Indirizzi fittizi per le porte GPIO */

static uint16_t pins[LED_COUNT] = {0x0001, 0x0002};        /**< Maschere pin per i test */
static pin_state_t init_states[LED_COUNT] = {
    GPIO_PIN_RESET,
    GPIO_PIN_RESET
}; /**< Stati iniziali nominali */
/** @} */

/**
 * @brief Setup eseguito prima di ogni singolo test.
 * Pulisce la struttura dati per garantire l'indipendenza dei test.
 */
void setUp(void) {
    memset(&led_obj, 0, sizeof(led_t));
}

/**
 * @brief Helper per l'inizializzazione standard del modulo.
 * @details Automatizza l'aspettativa delle chiamate HAL_GPIO_WritePin generate durante
 * la funzione led_init() per pulire il codice dei test successivi.
 */
void helper_init_led(void) {
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], init_states[LED_RED]);
    HAL_GPIO_WritePin_Expect(ports[LED_WHITE], pins[LED_WHITE], init_states[LED_WHITE]);
    led_init(&led_obj, ports, pins, init_states);
}

/* =================================================================---------
    TEST DI INIZIALIZZAZIONE E SCRITTURA FISICA
   ========================================================================= */

/**
 * @test Verifica la corretta configurazione GPIO durante l'init.
 * @details Controlla che vengano chiamate le funzioni HAL per impostare lo stato
 * iniziale e che la FSM parta dallo stato OFF.
 */
void test_led_init_should_setup_gpio_correctly(void) {
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(ports[LED_WHITE], pins[LED_WHITE], GPIO_PIN_RESET);

    int8_t res = led_init(&led_obj, ports, pins, init_states);

    TEST_ASSERT_EQUAL(LED_OK, res);
    TEST_ASSERT_EQUAL(OFF, led_obj.state);
}

/**
 * @test Verifica l'accensione manuale di un LED tramite astrazione led_on.
 */
void test_led_on_should_set_pin_high(void) {
    helper_init_led();

    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_SET);

    int8_t res = led_on(&led_obj, LED_RED);
    TEST_ASSERT_EQUAL(LED_OK, res);
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, led_obj.pinState[LED_RED]);
}

/* =================================================================---------
    TEST MACCHINA A STATI E LAMPEGGI (FSM)
   ========================================================================= */

/**
 * @test Verifica la transizione allo stato RED fisso.
 * @details Assicura che la FSM spenga il LED Bianco e accenda quello Rosso
 * contestualmente al cambio di stato.
 */
void test_led_step_red_solid(void) {
    helper_init_led();

    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(ports[LED_WHITE], pins[LED_WHITE], GPIO_PIN_RESET);

    led_step(&led_obj, RED);

    TEST_ASSERT_EQUAL(RED, led_obj.state);
}

/**
 * @test Verifica la sequenza temporale del lampeggio rosso (BLINKING_RED).
 * @details Valida la logica di ottimizzazione del driver:
 * 1. Al cambio di stato (Step 0), attiva il pin Rosso.
 * 2. Per gli step 1-3, non effettua chiamate HAL (pin già SET).
 * 3. Al raggiungimento della soglia RED_STEPS_ON (Step 4), disattiva il pin.
 */

void test_led_step_blinking_red_sequence(void) {
    helper_init_led();

    /* STEP 0: Cambio stato -> Attivazione iniziale */
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_SET);
    led_step(&led_obj, BLINKING_RED);

    /* STEP 1, 2, 3: Ottimizzazione GPIO
       Il driver rileva che il pin è già SET, quindi non chiama HAL_GPIO_WritePin.
       Unity fallirebbe se venissero effettuate chiamate non previste. */
    for(int i = 0; i < 3; i++) {
        led_step(&led_obj, BLINKING_RED);
    }

    /* STEP 4: Spegnimento (Toggle software basato su costanti) */
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_RESET);
    led_step(&led_obj, BLINKING_RED);
}

/**
 * @test Verifica che il passaggio tra due tipi di lampeggio resetti il timing.
 */
void test_led_transition_between_blinkings_resets_timing(void) {
    helper_init_led();

    // 1. Iniziamo con BLINKING_WHITE.
    // Lo stato iniziale è RESET per entrambi. Il driver accende solo il Bianco.
    HAL_GPIO_WritePin_Expect(ports[LED_WHITE], pins[LED_WHITE], GPIO_PIN_SET);
    led_step(&led_obj, BLINKING_WHITE);

    // Avanziamo fino a step 8 (fine fase ON del bianco)
    for(int i = 1; i < 8; i++) led_step(&led_obj, BLINKING_WHITE);

    // Step 8: Spegne il bianco
    HAL_GPIO_WritePin_Expect(ports[LED_WHITE], pins[LED_WHITE], GPIO_PIN_RESET);
    led_step(&led_obj, BLINKING_WHITE);

    TEST_ASSERT_EQUAL(9, led_obj.step);

    // 2. CAMBIO STATO -> BLINKING_RED
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_SET);
    led_step(&led_obj, BLINKING_RED);

    // Lo step deve essere resettato a 0 e poi incrementato a 1 dalla logica
    TEST_ASSERT_EQUAL(1, led_obj.step);
    TEST_ASSERT_EQUAL(BLINKING_RED, led_obj.state);
}

/**
 * @test Verifica la corretta durata del lampeggio lento (16 step totali).
 */
void test_led_step_blinking_red_slow_timing(void) {
    helper_init_led();

    // Step 0: Accensione
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_SET);
    led_step(&led_obj, BLINKING_RED_SLOW);

    // Step 1-7: Deve rimanere acceso (nessuna chiamata HAL)
    for(int i = 1; i < 8; i++) led_step(&led_obj, BLINKING_RED_SLOW);

    // Step 8: Deve spegnersi
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_RESET);
    led_step(&led_obj, BLINKING_RED_SLOW);

    // Step 15: Fine ciclo
    for(int i = 9; i < 16; i++) led_step(&led_obj, BLINKING_RED_SLOW);

    // Step 16 (0 del nuovo ciclo): Deve riaccendersi
    HAL_GPIO_WritePin_Expect(ports[LED_RED], pins[LED_RED], GPIO_PIN_SET);
    led_step(&led_obj, BLINKING_RED_SLOW);
}

/**
 * @test Verifica la robustezza del driver contro puntatori NULL.
 */
void test_led_api_should_return_error_on_null_pointer(void) {
    TEST_ASSERT_EQUAL(LED_ERR, led_init(NULL, ports, pins, init_states));
    TEST_ASSERT_EQUAL(LED_ERR, led_on(NULL, LED_RED));
    TEST_ASSERT_EQUAL(LED_ERR, led_step(NULL, RED));
}
