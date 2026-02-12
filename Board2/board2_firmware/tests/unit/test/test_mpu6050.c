/**
 * @file test_mpu6050.c
 * @brief Unit Tests per il driver MPU6050 utilizzando Ceedling e CMock.
 * @author Gruppo 2
 * * @details Questo file contiene i test di unità per verificare la logica di
 * inizializzazione, la gestione degli errori I2C e la corretta conversione
 * dei dati grezzi (raw) in unità fisiche.
 */

#include "unity.h"
#include "mock_stm32g4xx_hal.h"
#include "mpu6050.h"
#include <string.h>

/** * @name Oggetti Globali di Test
 * @{
 */
static mpu6050_t dev;                       /**< Istanza del driver sotto test */
static I2C_HandleTypeDef hi2c;              /**< Mock dell'handle I2C STM32 */
static uint8_t mock_receive_buffer[10];     /**< Buffer simulato per le letture I2C */
static HAL_StatusTypeDef mock_return_status; /**< Stato di ritorno simulato per la HAL */
/** @} */

/**
 * @brief Stub personalizzato per la funzione HAL_I2C_Mem_Read.
 * * @details Sostituisce la funzione originale della HAL. Quando il driver chiama la lettura,
 * questa funzione copia i dati da #mock_receive_buffer nel buffer `pData` fornito dal driver.
 * Permette di simulare i registri dell'MPU6050 senza hardware reale.
 * *
 * * @param hi2c Puntatore all'handle I2C (Mock)
 * @param DevAddress Indirizzo I2C del sensore
 * @param MemAddress Indirizzo del registro da leggere
 * @param MemAddSize Dimensione dell'indirizzo di memoria
 * @param pData Puntatore al buffer del driver dove scrivere i dati
 * @param Size Numero di byte da leggere
 * @param Timeout Timeout dell'operazione
 * @param cmock_num_calls Numero di chiamate effettuate durante il test
 * @return HAL_StatusTypeDef Ritorna lo stato impostato in #mock_return_status
 */
HAL_StatusTypeDef HAL_I2C_Mem_Read_Callback(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
                                           uint16_t MemAddSize, uint8_t *pData, uint16_t Size,
                                           uint32_t Timeout, int cmock_num_calls) {
    if (pData != NULL) {
        memcpy(pData, mock_receive_buffer, Size);
    }
    return mock_return_status;
}

/**
 * @brief Setup iniziale eseguito prima di ogni test.
 * @details Inizializza le strutture, configura il driver con parametri di default
 * e registra lo stub per la comunicazione I2C.
 */
void setUp(void) {
    memset(&dev, 0, sizeof(mpu6050_t));
    memset(&hi2c, 0, sizeof(I2C_HandleTypeDef));
    memset(mock_receive_buffer, 0, sizeof(mock_receive_buffer));

    /* Configurazione di default per i test */
    dev.i2c = &hi2c;
    dev.address = 0x68 << 1;
    dev.mpu6050_cfg.accel_range = MPU6050_ACCEL_FS_2G;

    /* Registrazione dello stub e configurazione Ignore per le funzioni non critiche */
    HAL_I2C_Mem_Read_StubWithCallback(HAL_I2C_Mem_Read_Callback);
    HAL_I2C_Mem_Write_IgnoreAndReturn(HAL_OK);
    HAL_Delay_Ignore();

    mock_return_status = HAL_OK;
}

/**
 * @test Verifica dell'identificazione del dispositivo (WHO_AM_I).
 * @details Testa se la funzione mpu6050_who_am_i() estrae correttamente il valore
 * dal registro 0x75.
 */
void test_mpu6050_who_am_i_success(void) {
    uint8_t id = 0;
    mock_receive_buffer[0] = 0x68; /* ID tipico MPU6050 */

    MPU60X0_StatusTypeDef status = mpu6050_who_am_i(&dev, &id);

    TEST_ASSERT_EQUAL(MPU6050_OK, status);
    TEST_ASSERT_EQUAL_HEX8(0x68, id);
}

/**
 * @test Verifica della lettura dell'accelerometro (1g sull'asse Z).
 * @details Simula il valore raw 16384 (0x4000) sui registri Z_H e Z_L.
 * Con fondo scala ±2g, 16384 deve corrispondere esattamente a 1.0g.
 * *
 */
void test_mpu6050_get_accel_value_1g_z(void) {
    imu_vector_t accel;

    /* Dati in formato Big Endian: X=0, Y=0, Z=16384 */
    mock_receive_buffer[0] = 0x00; mock_receive_buffer[1] = 0x00; // Accel_X
    mock_receive_buffer[2] = 0x00; mock_receive_buffer[3] = 0x00; // Accel_Y
    mock_receive_buffer[4] = 0x40; mock_receive_buffer[5] = 0x00; // Accel_Z

    MPU60X0_StatusTypeDef status = mpu6050_get_accel_value(&dev, &accel);

    TEST_ASSERT_EQUAL(MPU6050_OK, status);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, accel.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, accel.y);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, accel.z);
}

/**
 * @test Verifica della gestione degli errori di comunicazione.
 * @details Simula un fallimento del bus I2C (HAL_ERROR) e verifica che il driver
 * restituisca il codice d'errore MPU6050_ERR_COMM.
 */
void test_mpu6050_comm_error(void) {
    uint8_t id;
    mock_return_status = HAL_ERROR;

    MPU60X0_StatusTypeDef status = mpu6050_who_am_i(&dev, &id);

    TEST_ASSERT_EQUAL(MPU6050_ERR_COMM, status);
}

/**
 * @test Verifica la corretta gestione dei numeri negativi.
 * @details Con FS=2G, 0xC000 corrisponde a -16384 raw, ovvero -1.0g.
 */
void test_mpu6050_get_accel_negative_g(void) {
    imu_vector_t accel;
    // 0xC000 = -16384 in complemento a 2
    mock_receive_buffer[4] = 0xC0; mock_receive_buffer[5] = 0x00;

    mpu6050_get_accel_value(&dev, &accel);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.0f, accel.z);
}

/**
 * @test Verifica che il cambio di range modifichi la conversione.
 * @details In FS_8G, 4096 raw deve corrispondere a 1.0g (non più 0.25g).
 */
void test_mpu6050_accel_range_8g(void) {
    imu_vector_t accel;
    dev.mpu6050_cfg.accel_range = MPU6050_ACCEL_FS_8G; // Sensibilità: 4096 LSB/g

    mock_receive_buffer[4] = 0x10; mock_receive_buffer[5] = 0x00; // 4096 raw

    mpu6050_get_accel_value(&dev, &accel);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, accel.z);
}

/**
 * @test Verifica la conversione dei dati del giroscopio.
 * @details Con FS_250DPS, 131 raw deve corrispondere a 1.0 °/s.
 */
void test_mpu6050_get_gyro_conversion(void) {
    imu_vector_t gyro;
    dev.mpu6050_cfg.gyro_range = MPU6050_GYRO_FS_250DPS;

    // 131 in hex è 0x0083
    mock_receive_buffer[0] = 0x00; mock_receive_buffer[1] = 0x83;

    mpu6050_get_gyro_value(&dev, &gyro);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, gyro.x);
}

/**
 * @test Verifica la gestione del timeout I2C.
 */
void test_mpu6050_timeout_error(void) {
    uint8_t id;
    mock_return_status = HAL_TIMEOUT;

    MPU60X0_StatusTypeDef status = mpu6050_who_am_i(&dev, &id);

    TEST_ASSERT_EQUAL(MPU6050_ERR_COMM, status);
}
