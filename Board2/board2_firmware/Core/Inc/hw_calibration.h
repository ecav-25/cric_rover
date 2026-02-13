// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    hw_calibration.h
 * @author  Gruppo 2
 * @brief   Costanti di calibrazione hardware per i motori.
 * @details Questo file contiene i valori empirici rilevati per centrare il segnale
 * PWM (punto neutro) e definire l'escursione massima (scaling) per ogni motore.
 * Questi valori compensano le differenze fisiche tra gli ESC/Driver.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef INC_HW_CALIBRATION_H_
#define INC_HW_CALIBRATION_H_

/* ========================================================================== */
/* PWM STOP (NEUTRAL POINT)                                                   */
/* ========================================================================== */

/**
 * @brief Valore del registro CCR corrispondente alla velocità zero (punto neutro).
 * @note  Varia leggermente tra i motori a causa delle tolleranze hardware.
 * @{
 */
#define PWM_STOP_FA            (3230u) /**< Punto neutro Motore Front-A */
#define PWM_STOP_FB            (3230u) /**< Punto neutro Motore Front-B */
#define PWM_STOP_BA            (3215u) /**< Punto neutro Motore Back-A */
#define PWM_STOP_BB            (3215u) /**< Punto neutro Motore Back-B */
/** @} */

/* ========================================================================== */
/* PWM SCALE FORWARD                                                          */
/* ========================================================================== */

/**
 * @brief Escursione massima del registro CCR per la marcia avanti.
 * @details Rappresenta il delta da aggiungere a @ref PWM_STOP_xx per ottenere
 * la massima velocità in avanti (Duty 100%).
 * @{
 */
#define PWM_SCALE_FORWARD_FA   (690u)
#define PWM_SCALE_FORWARD_FB   (710u)
#define PWM_SCALE_FORWARD_BA   (690u)
#define PWM_SCALE_FORWARD_BB   (690u)
/** @} */

/* ========================================================================== */
/* PWM SCALE BACKWARD                                                         */
/* ========================================================================== */

/**
 * @brief Escursione massima del registro CCR per la retromarcia.
 * @details Rappresenta il delta da sottrarre a @ref PWM_STOP_xx per ottenere
 * la massima velocità indietro (Duty -100%).
 * @{
 */
#define PWM_SCALE_BACKWARD_FA  (640u)
#define PWM_SCALE_BACKWARD_FB  (630u)
#define PWM_SCALE_BACKWARD_BA  (630u)
#define PWM_SCALE_BACKWARD_BB  (620u)
/** @} */

#endif /* INC_HW_CALIBRATION_H_ */
