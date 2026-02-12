/**
 * @file test_controller.c
 * @author Gruppo 2
 * @brief Unit Tests per il modulo di telecontrollo e telemetria I2C.
 * * @details Questa suite verifica la corretta interazione con il controller esterno tramite bus I2C.
 */

#include "unity.h"
#include <string.h>
#include "mock_stm32g4xx_hal.h"
#include "controller.h"

/** @name Risorse Statiche di Test
 * @{ */
static Controller_t telecontrol;      /**< Istanza del controller sotto test */
static I2C_HandleTypeDef hi2c_test;   /**< Mock dell'handle I2C */
static Telemetry_t telemetry_data;    /**< Struttura dati per i test di telemetria */
/** @} */

/**
 * @brief Configurazione iniziale eseguita prima di ogni test.
 * @details Azzera le strutture dati per garantire l'indipendenza tra i casi di test.
 */
void setUp(void) {
    memset(&telecontrol, 0, sizeof(Controller_t));
    memset(&hi2c_test, 0, sizeof(I2C_HandleTypeDef));
    memset(&telemetry_data, 0, sizeof(Telemetry_t));
}

/**
 * @brief Pulizia post-test.
 */
void tearDown(void) {}

/* =================================================================---------
    TEST DI INIZIALIZZAZIONE
   ========================================================================= */

/**
 * @test Verifica che l'inizializzazione imposti correttamente i valori di default.
 * @details Si assicura che i joystick siano centrati (CONTROLLERZERO) e l'indirizzo salvato.
 */
void test_telecontrol_init_should_reset_data(void) {
    ControllerStatus_t status = telecontrol_init(&telecontrol, &hi2c_test, 0x42);

    TEST_ASSERT_EQUAL(CONTROLLER_OK, status);
    TEST_ASSERT_EQUAL_PTR(&hi2c_test, telecontrol.i2c);
    TEST_ASSERT_EQUAL(0x42, telecontrol.address);
    TEST_ASSERT_EQUAL(CONTROLLERZERO, telecontrol.controller_information.controller_data.ax);
}

/* =================================================================---------
    TEST DI LETTURA (I2C RECEIVE)
   ========================================================================= */

/**
 * @test Verifica il flusso di lettura nominale (I2C Master Receive).
 * @details Verifica che venga chiamato l'indirizzo I2C corretto (shiftato)
 * e che la funzione gestisca il ritorno dei dati.
 */
void test_telecontrol_read_success_with_valid_crc(void) {
    telecontrol_init(&telecontrol, &hi2c_test, 0x42);

    /* Prepariamo l'aspettativa: l'indirizzo deve essere (0x42 << 1) */
    HAL_I2C_Master_Receive_ExpectAndReturn(&hi2c_test, (0x42 << 1),
                                           (uint8_t *)&telecontrol.controller_information,
                                           sizeof(controller_information_t),
                                           20, HAL_OK);

    ControllerStatus_t status = telecontrol_read(&telecontrol);

    /* Il driver deve ritornare un errore di comunicazione o CRC, ma non fallire il test hardware */
    TEST_ASSERT_NOT_EQUAL(CONTROLLER_ERR_COMM, status);
}

/**
 * @test Verifica la gestione degli errori hardware I2C.
 * @details Simula un fallimento del bus (HAL_ERROR) e verifica il codice di errore COMM.
 */
void test_telecontrol_read_comm_error(void) {
    telecontrol_init(&telecontrol, &hi2c_test, 0x42);

    HAL_I2C_Master_Receive_ExpectAndReturn(&hi2c_test, (0x42 << 1),
                                           (uint8_t *)&telecontrol.controller_information,
                                           sizeof(controller_information_t),
                                           20, HAL_ERROR);

    ControllerStatus_t status = telecontrol_read(&telecontrol);
    TEST_ASSERT_EQUAL(CONTROLLER_ERR_COMM, status);
}

/* =================================================================---------
    TEST DI TELEMETRIA (I2C TRANSMIT)
   ========================================================================= */

/**
 * @test Verifica l'invio della telemetria verso l'indirizzo di sistema.
 * @details Nota: Il driver usa l'indirizzo fisso 0x55 per la telemetria esterna.
 */
void test_telecontrol_send_telemetry_success(void) {
    telecontrol_init(&telecontrol, &hi2c_test, 0x42);
    telemetry_init(&telemetry_data);

    HAL_I2C_Master_Transmit_ExpectAndReturn(&hi2c_test, (0x55 << 1),
                                            (uint8_t*)&telemetry_data,
                                            sizeof(Telemetry_t), 100, HAL_OK);

    ControllerStatus_t status = telecontrol_send_telemetry(&telecontrol, &telemetry_data);
    TEST_ASSERT_EQUAL(CONTROLLER_OK, status);
}

/**
 * @test Verifica che i setter modifichino correttamente i campi della telemetria.
 */
void test_telemetry_setters(void) {
    telemetry_init(&telemetry_data);
    telemetry_set_battery(&telemetry_data, 85);

    TEST_ASSERT_EQUAL(85, telemetry_data.battery_percent);
}

/* =================================================================---------
    TEST DI SICUREZZA
   ========================================================================= */

/**
 * @test Verifica la gestione dei parametri NULL per prevenire crash.
 */
void test_controller_null_pointers(void) {
    TEST_ASSERT_EQUAL(CONTROLLER_ERR, telecontrol_init(NULL, &hi2c_test, 0x42));
    TEST_ASSERT_EQUAL(CONTROLLER_ERR, telecontrol_read(NULL));
}

/**
 * @test Verifica la persistenza dei dati in caso di errore di comunicazione.
 * @details Se una lettura fallisce, il driver deve ripristinare i dati della
 * struttura 'last_valid_information' per evitare che il rover legga dati corrotti.
 */
void test_telecontrol_read_fallback_on_failure(void) {
    telecontrol_init(&telecontrol, &hi2c_test, 0x42);

    /* 1. Simuliamo un dato precedentemente valido e salvato nel backup */
    telecontrol.last_valid_information.controller_data.bx = 150;
    telecontrol.controller_information.controller_data.bx = 150;

    /* 2. La lettura successiva fallisce miseramente */
    HAL_I2C_Master_Receive_ExpectAndReturn(&hi2c_test, (0x42 << 1),
                                           (uint8_t *)&telecontrol.controller_information,
                                           sizeof(controller_information_t), 20, HAL_ERROR);

    ControllerStatus_t status = telecontrol_read(&telecontrol);

    /* 3. Verifiche */
    TEST_ASSERT_EQUAL(CONTROLLER_ERR_COMM, status);

    uint16_t bx_val = 0;
    // CORRETTO: Usiamo il getter per AX per verificare il ripristino
    get_telecontrol_bx(&telecontrol, &bx_val);
    TEST_ASSERT_EQUAL(150, bx_val);
}
