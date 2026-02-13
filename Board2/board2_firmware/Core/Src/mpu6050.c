// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    mpu6050.c
 * @author  Gruppo 2
 * @brief   Implementazione del driver per IMU MPU6050.
 * @details Questo modulo gestisce la comunicazione I2C con il sensore,
 * la conversione dei dati grezzi in unità fisiche (g, °/s) e la configurazione
 * dei registri interni (DLPF, Sample Rate, Full Scale).
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "mpu6050.h"
#include "stddef.h"

/* ========================================================================== */
/* INTERNAL CONSTANTS                                                         */
/* ========================================================================== */

/** @brief Timeout di default per le operazioni I2C bloccanti (in ms). */
#define MPU6050_I2C_TIMEOUT_MS   100u

/* ========================================================================== */
/* STATIC LOW-LEVEL HELPERS                                                   */
/* ========================================================================== */

/**
 * @brief Scrive un singolo byte in un registro specifico.
 * @param dev Puntatore all'handle del dispositivo.
 * @param reg Indirizzo del registro di destinazione.
 * @param val Valore da scrivere.
 * @return MPU60X0_StatusTypeDef MPU6050_OK se la scrittura I2C ha successo.
 */
static MPU60X0_StatusTypeDef mpu6050_write_reg(mpu6050_t* dev, uint8_t reg, uint8_t val)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR_COMM;

    if (dev != NULL)
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Write(dev->i2c, dev->address, (uint16_t)reg,
                                                         I2C_MEMADD_SIZE_8BIT, &val, 1U,
                                                         MPU6050_I2C_TIMEOUT_MS);

        if (hal_status == HAL_OK)
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Legge un singolo byte da un registro specifico.
 * @param dev Puntatore all'handle del dispositivo.
 * @param reg Indirizzo del registro da leggere.
 * @param val Puntatore alla variabile dove salvare il dato letto.
 * @return MPU60X0_StatusTypeDef MPU6050_OK se la lettura I2C ha successo.
 */
static MPU60X0_StatusTypeDef mpu6050_read_reg(mpu6050_t* dev, uint8_t reg, uint8_t* val)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR_COMM;

    if ((dev != NULL) && (val != NULL))
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read(dev->i2c, dev->address, (uint16_t)reg,
                                                        I2C_MEMADD_SIZE_8BIT, val, 1U,
                                                        MPU6050_I2C_TIMEOUT_MS);

        if (hal_status == HAL_OK)
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Legge una sequenza di byte (Burst Read) partendo da un registro.
 * @details Fondamentale per leggere i dati dei sensori (H e L byte) in un'unica
 * transazione atomica, garantendo la coerenza temporale dei campioni.
 * @param dev Puntatore all'handle del dispositivo.
 * @param reg Indirizzo del registro di partenza.
 * @param buf Puntatore al buffer di destinazione.
 * @param len Numero di byte da leggere.
 * @return MPU60X0_StatusTypeDef MPU6050_OK se la lettura I2C ha successo.
 */
static MPU60X0_StatusTypeDef mpu6050_read_bytes(mpu6050_t* dev, uint8_t reg, uint8_t* buf, uint16_t len)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR_COMM;

    if ((dev != NULL) && (buf != NULL))
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read(dev->i2c, dev->address, (uint16_t)reg,
                                                        I2C_MEMADD_SIZE_8BIT, buf, len,
                                                        MPU6050_I2C_TIMEOUT_MS);

        if (hal_status == HAL_OK)
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Legge un asse specifico (2 registri), combina i byte e converte in float.
 * @details Legge i registri High e Low, ricostruisce il valore a 16 bit (int16_t)
 * e lo divide per la sensibilità per ottenere il valore fisico.
 * @param dev Puntatore all'handle del dispositivo.
 * @param reg_h Indirizzo del registro High byte.
 * @param sensitivity Fattore di sensibilità (LSB/unit).
 * @param out Puntatore (float) dove salvare il valore convertito.
 * @return MPU60X0_StatusTypeDef MPU6050_OK se l'operazione ha successo.
 */
static MPU60X0_StatusTypeDef mpu6050_read_axis(mpu6050_t* dev, uint8_t reg_h, float sensitivity, float* out)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t buf[2];

    if ((dev != NULL) && (out != NULL))
    {
        if (mpu6050_read_bytes(dev, reg_h, buf, 2U) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            uint16_t raw_u = ((uint16_t)buf[0] << 8U) | (uint16_t)buf[1];
            int16_t raw_s = (int16_t)raw_u;

            *out = (float)raw_s / sensitivity;
            status = MPU6050_OK;
        }
    }

    return status;
}


/* ========================================================================== */
/* SENSITIVITY HELPERS                                                        */
/* ========================================================================== */

/**
 * @brief Restituisce la sensibilità dell'accelerometro (LSB/g) in base al range.
 * @param range Fondo scala configurato (2G, 4G, 8G, 16G).
 * @return float Valore di sensibilità.
 */
static float mpu6050_accel_sensitivity(mpu6050_accel_fs_t range)
{
    float sensitivity;

    switch (range)
    {
        case MPU6050_ACCEL_FS_2G:
            sensitivity = 16384.0f;
            break;
        case MPU6050_ACCEL_FS_4G:
            sensitivity = 8192.0f;
            break;
        case MPU6050_ACCEL_FS_8G:
            sensitivity = 4096.0f;
            break;
        case MPU6050_ACCEL_FS_16G:
            sensitivity = 2048.0f;
            break;
        default:
            sensitivity = 16384.0f;
            break;
    }

    return sensitivity;
}

/**
 * @brief Restituisce la sensibilità del giroscopio (LSB/°/s) in base al range.
 * @param range Fondo scala configurato (250, 500, 1000, 2000 DPS).
 * @return float Valore di sensibilità .
 */
static float mpu6050_gyro_sensitivity(mpu6050_gyro_fs_t range)
{
    float sensitivity;

    switch (range)
    {
        case MPU6050_GYRO_FS_250DPS:
            sensitivity = 131.0f;
            break;
        case MPU6050_GYRO_FS_500DPS:
            sensitivity = 65.5f;
            break;
        case MPU6050_GYRO_FS_1000DPS:
            sensitivity = 32.8f;
            break;
        case MPU6050_GYRO_FS_2000DPS:
            sensitivity = 16.4f;
            break;
        default:
            sensitivity = 131.0f;
            break;
    }

    return sensitivity;
}

/* ========================================================================== */
/* CONFIGURATION HELPERS                                                      */
/* ========================================================================== */

/**
 * @brief Esegue il reset software del dispositivo.
 * @details Scrive il bit DEVICE_RESET nel registro PWR_MGMT_1.
 * @param dev Handle del dispositivo.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
static MPU60X0_StatusTypeDef mpu6050_reset(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, PWR_MGMT_1, DEVICE_RESET) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            HAL_Delay(100U); /* Attesa completamento reset */
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Sveglia il dispositivo dalla modalità Sleep (default al power-up).
 * @details Azzera il bit SLEEP nel registro PWR_MGMT_1.
 * @param dev Handle del dispositivo.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
static MPU60X0_StatusTypeDef mpu6050_wake(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, PWR_MGMT_1, PWR_MGMT_WAKE) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Helper di basso livello per impostare il range accelerometro.
 * @details Esegue Read-Modify-Write sul registro ACCEL_CONFIG per preservare gli altri bit.
 */
static MPU60X0_StatusTypeDef mpu6050_set_accel_range_lowlevel(mpu6050_t* dev, mpu6050_accel_fs_t range)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t reg = 0U;

    if (dev != NULL)
    {
        if (mpu6050_read_reg(dev, ACCEL_CONFIG, &reg) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            reg &= (uint8_t)(~0x18U); /* Mask bit AFS_SEL [4:3] */
            reg |= (uint8_t)((uint8_t)range << 3U);

            if (mpu6050_write_reg(dev, ACCEL_CONFIG, reg) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

/**
 * @brief Helper di basso livello per impostare il range giroscopio.
 * @details Esegue Read-Modify-Write sul registro GYRO_CONFIG per preservare gli altri bit.
 */
static MPU60X0_StatusTypeDef mpu6050_set_gyro_range_lowlevel(mpu6050_t* dev, mpu6050_gyro_fs_t range)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t reg = 0U;

    if (dev != NULL)
    {
        if (mpu6050_read_reg(dev, GYRO_CONFIG, &reg) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            reg &= (uint8_t)(~0x18U); /* Mask bit FS_SEL [4:3] */
            reg |= (uint8_t)((uint8_t)range << 3U);

            if (mpu6050_write_reg(dev, GYRO_CONFIG, reg) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

/**
 * @brief Applica l'intera struttura di configurazione al dispositivo.
 * @details Richiamato durante l'inizializzazione o il recovery.
 */
static MPU60X0_StatusTypeDef mpu6050_apply_config(mpu6050_t* dev, const mpu6050_config_t* cfg)
{
    MPU60X0_StatusTypeDef st = MPU6050_ERR;

    if ((dev != NULL) && (cfg != NULL))
    {
        /* 1. Configurazione DLPF */
        st = mpu6050_set_dlpf(dev, cfg->dlpf_cfg);

        /* 2. Configurazione Divisore Sample Rate */
        if (st == MPU6050_OK)
        {
            st = mpu6050_set_sample_div(dev, cfg->smplrt_div);
        }

        /* 3. Configurazione Range Accelerometro */
        if (st == MPU6050_OK)
        {
            st = mpu6050_set_accel_fs(dev, cfg->accel_range);
        }

        /* 4. Configurazione Range Giroscopio */
        if (st == MPU6050_OK)
        {
            st = mpu6050_set_gyro_fs(dev, cfg->gyro_range);
        }

        /* 5. Configurazione Interrupt */
        if (st == MPU6050_OK)
        {
            st = mpu6050_set_interrupt_mask(dev, cfg->int_enable);
        }
    }

    return st;
}

/* ========================================================================== */
/* CORE API                                                                   */
/* ========================================================================== */

/**
 * @brief Inizializza il dispositivo MPU6050.
 * @details Esegue la sequenza: Reset -> Wake Up -> Applicazione Configurazione.
 * @param dev Puntatore all'handle del dispositivo.
 * @param i2c Handle I2C hardware.
 * @param address Indirizzo I2C del sensore.
 * @param mpu_cfg Configurazione iniziale.
 * @return MPU60X0_StatusTypeDef Esito dell'inizializzazione.
 */
MPU60X0_StatusTypeDef mpu6050_init(mpu6050_t* dev, I2C_HandleTypeDef* i2c, uint16_t address, const mpu6050_config_t* mpu_cfg)
{
    MPU60X0_StatusTypeDef st = MPU6050_ERR;

    if ((dev != NULL) && (i2c != NULL) && (mpu_cfg != NULL))
    {
        dev->i2c = i2c;

        if (address != 0U)
        {
            dev->address = address;
        }
        else
        {
            dev->address = MPU60X0_ADDRESS;
        }

        /* Sequenza di startup */
        if (mpu6050_reset(dev) != MPU6050_OK)
        {
            st = MPU6050_ERR_COMM;
        }
        else if (mpu6050_wake(dev) != MPU6050_OK)
        {
            st = MPU6050_ERR_COMM;
        }
        else
        {
            dev->mpu6050_cfg = *mpu_cfg;
            st = mpu6050_apply_config(dev, &dev->mpu6050_cfg);
        }
    }

    return st;
}

/**
 * @brief Tenta di ripristinare il sensore reinizializzandolo.
 * @details Utile dopo errori di comunicazione o glitch di alimentazione.
 * Resetta il Signal Path e riapplica l'ultima configurazione nota.
 */
MPU60X0_StatusTypeDef mpu6050_recovery_init(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef st = MPU6050_ERR;

    if (dev != NULL)
    {
        st = mpu6050_signal_path_reset(dev);

        if (st == MPU6050_OK)
        {
            st = mpu6050_apply_config(dev, &dev->mpu6050_cfg);
        }
    }

    return st;
}

/**
 * @brief Legge il registro WHO_AM_I.
 * @param dev Handle del dispositivo.
 * @param id Puntatore dove salvare l'ID letto (valore atteso 0x68).
 */
MPU60X0_StatusTypeDef mpu6050_who_am_i(mpu6050_t* dev, uint8_t* id)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (id != NULL))
    {
        status = mpu6050_read_reg(dev, WHO_AM_I, id);
    }

    return status;
}

/**
 * @brief Acquisisce i dati dell'accelerometro (X, Y, Z).
 * @details Legge 6 byte in burst e converte i valori in g.
 * Aggiorna la struttura dati interna del driver.
 * @param dev Handle del dispositivo.
 * @param out Puntatore alla struttura vettore dove salvare i dati [g].
 */
MPU60X0_StatusTypeDef mpu6050_get_accel_value(mpu6050_t* dev, imu_vector_t* out)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t buf[6];

    if ((dev != NULL) && (out != NULL))
    {
        if (mpu6050_read_bytes(dev, ACCEL_XOUT_H, buf, (uint16_t)sizeof(buf)) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            /* MISRA: Operazioni bitwise su unsigned prima del cast a signed */
            uint16_t rawX_u = ((uint16_t)buf[0] << 8U) | (uint16_t)buf[1];
            uint16_t rawY_u = ((uint16_t)buf[2] << 8U) | (uint16_t)buf[3];
            uint16_t rawZ_u = ((uint16_t)buf[4] << 8U) | (uint16_t)buf[5];

            int16_t rawX_s = (int16_t)rawX_u;
            int16_t rawY_s = (int16_t)rawY_u;
            int16_t rawZ_s = (int16_t)rawZ_u;

            float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);

            out->x = (float)rawX_s / sens;
            out->y = (float)rawY_s / sens;
            out->z = (float)rawZ_s / sens;

            dev->data.accel = *out;
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Acquisisce i dati del giroscopio (X, Y, Z).
 * @details Legge 6 byte in burst e converte i valori in °/s.
 * Aggiorna la struttura dati interna del driver.
 * @param dev Handle del dispositivo.
 * @param out Puntatore alla struttura vettore dove salvare i dati [°/s].
 */
MPU60X0_StatusTypeDef mpu6050_get_gyro_value(mpu6050_t* dev, imu_vector_t* out)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t buf[6];

    if ((dev != NULL) && (out != NULL))
    {
        if (mpu6050_read_bytes(dev, GYRO_XOUT_H, buf, (uint16_t)sizeof(buf)) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
        	uint16_t rawX_u = ((uint16_t)buf[0] << 8U) | (uint16_t)buf[1];
            uint16_t rawY_u = ((uint16_t)buf[2] << 8U) | (uint16_t)buf[3];
            uint16_t rawZ_u = ((uint16_t)buf[4] << 8U) | (uint16_t)buf[5];

            int16_t rawX_s = (int16_t)rawX_u;
            int16_t rawY_s = (int16_t)rawY_u;
            int16_t rawZ_s = (int16_t)rawZ_u;

            float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);

            out->x = (float)rawX_s / sens;
            out->y = (float)rawY_s / sens;
            out->z = (float)rawZ_s / sens;

            dev->data.gyro = *out;
            status = MPU6050_OK;
        }
    }

    return status;
}

/* ========================================================================== */
/* CONFIGURATION API                                                          */
/* ========================================================================== */

/**
 * @brief Resetta i percorsi di segnale interni (Signal Path Reset).
 * @details Resetta filtri digitali e buffer per Giroscopio, Accelerometro e Temperatura.
 */
MPU60X0_StatusTypeDef mpu6050_signal_path_reset(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        status = mpu6050_write_reg(dev, SIGNAL_PATH_RESET, (uint8_t)(GYRO_RESET | ACCEL_RESET | TEMP_RESET));
    }

    return status;
}

/**
 * @brief Imposta la configurazione del filtro passa-basso digitale (DLPF).
 * @param dlpf_cfg Valore configurazione (0-6).
 */
MPU60X0_StatusTypeDef mpu6050_set_dlpf(mpu6050_t* dev, uint8_t dlpf_cfg)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t reg = 0U;
    uint8_t cfg_masked;

    if (dev != NULL)
    {
        cfg_masked = (dlpf_cfg & 0x07U);

        if (mpu6050_read_reg(dev, CONFIG, &reg) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            /* Mask DLPF_CFG bits [2:0] */
            reg &= (uint8_t)(~0x07U);
            reg |= cfg_masked;

            if (mpu6050_write_reg(dev, CONFIG, reg) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                dev->mpu6050_cfg.dlpf_cfg = cfg_masked;
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

/**
 * @brief Legge la configurazione corrente del DLPF.
 */
MPU60X0_StatusTypeDef mpu6050_get_dlpf(mpu6050_t* dev, uint8_t* dlpf_cfg)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (dlpf_cfg != NULL))
    {
        *dlpf_cfg = dev->mpu6050_cfg.dlpf_cfg;
        status = MPU6050_OK;
    }

    return status;
}

/**
 * @brief Imposta il divisore della frequenza di campionamento.
 * @param smplrt_div Divisore (Sample Rate = Gyro Output Rate / (1 + smplrt_div)).
 */
MPU60X0_StatusTypeDef mpu6050_set_sample_div(mpu6050_t* dev, uint8_t smplrt_div)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, SMPRT_DIV, smplrt_div) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            dev->mpu6050_cfg.smplrt_div = smplrt_div;
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Legge il divisore della frequenza di campionamento corrente.
 */
MPU60X0_StatusTypeDef mpu6050_get_sample_div(mpu6050_t* dev, uint8_t* smplrt_div)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (smplrt_div != NULL))
    {
        *smplrt_div = dev->mpu6050_cfg.smplrt_div;
        status = MPU6050_OK;
    }

    return status;
}

/**
 * @brief Imposta il fondo scala dell'accelerometro.
 * @param fs Enum mpu6050_accel_fs_t (2G, 4G, 8G, 16G).
 */
MPU60X0_StatusTypeDef mpu6050_set_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (fs <= MPU6050_ACCEL_FS_16G)
        {
            if (mpu6050_set_accel_range_lowlevel(dev, fs) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                dev->mpu6050_cfg.accel_range = fs;
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

/**
 * @brief Legge il fondo scala corrente dell'accelerometro.
 */
MPU60X0_StatusTypeDef mpu6050_get_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t* fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (fs != NULL))
    {
        *fs = dev->mpu6050_cfg.accel_range;
        status = MPU6050_OK;
    }

    return status;
}

/**
 * @brief Imposta il fondo scala del giroscopio.
 * @param fs Enum mpu6050_gyro_fs_t (250, 500, 1000, 2000 DPS).
 */
MPU60X0_StatusTypeDef mpu6050_set_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (fs <= MPU6050_GYRO_FS_2000DPS)
        {
            if (mpu6050_set_gyro_range_lowlevel(dev, fs) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                dev->mpu6050_cfg.gyro_range = fs;
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

/**
 * @brief Legge il fondo scala corrente del giroscopio.
 */
MPU60X0_StatusTypeDef mpu6050_get_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t* fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (fs != NULL))
    {
        *fs = dev->mpu6050_cfg.gyro_range;
        status = MPU6050_OK;
    }

    return status;
}

/**
 * @brief Imposta la maschera di abilitazione degli interrupt.
 * @param int_mask Bitmask (vedi INT_ENABLE register).
 */
MPU60X0_StatusTypeDef mpu6050_set_interrupt_mask(mpu6050_t* dev, uint8_t int_mask)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, INT_ENABLE, int_mask) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            dev->mpu6050_cfg.int_enable = int_mask;
            status = MPU6050_OK;
        }
    }

    return status;
}

/**
 * @brief Legge la maschera di abilitazione degli interrupt corrente.
 */
MPU60X0_StatusTypeDef mpu6050_get_interrupt_mask(mpu6050_t* dev, uint8_t* int_mask)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (int_mask != NULL))
    {
        *int_mask = dev->mpu6050_cfg.int_enable;
        status = MPU6050_OK;
    }

    return status;
}

/* ========================================================================== */
/* SINGLE AXIS GETTERS                                                        */
/* ========================================================================== */

/** @brief Legge l'accelerazione sull'asse X [g]. */
MPU60X0_StatusTypeDef mpu6050_get_accel_x(mpu6050_t* dev, float* ax){
    float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);
    return mpu6050_read_axis(dev, ACCEL_XOUT_H, sens, ax);
}

/** @brief Legge l'accelerazione sull'asse Y [g]. */
MPU60X0_StatusTypeDef mpu6050_get_accel_y(mpu6050_t* dev, float* ay){
    float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);
    return mpu6050_read_axis(dev, ACCEL_YOUT_H, sens, ay);
}

/** @brief Legge l'accelerazione sull'asse Z [g]. */
MPU60X0_StatusTypeDef mpu6050_get_accel_z(mpu6050_t* dev, float* az){
    float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);
    return mpu6050_read_axis(dev, ACCEL_ZOUT_H, sens, az);
}

/** @brief Legge la velocità angolare sull'asse X [°/s]. */
MPU60X0_StatusTypeDef mpu6050_get_gyro_x(mpu6050_t* dev, float* gx){
    float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);
    return mpu6050_read_axis(dev, GYRO_XOUT_H, sens, gx);
}

/** @brief Legge la velocità angolare sull'asse Y [°/s]. */
MPU60X0_StatusTypeDef mpu6050_get_gyro_y(mpu6050_t* dev, float* gy){
    float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);
    return mpu6050_read_axis(dev, GYRO_YOUT_H, sens, gy);
}

/** @brief Legge la velocità angolare sull'asse Z [°/s]. */
MPU60X0_StatusTypeDef mpu6050_get_gyro_z(mpu6050_t* dev, float* gz){
    float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);
    return mpu6050_read_axis(dev, GYRO_ZOUT_H, sens, gz);
}
