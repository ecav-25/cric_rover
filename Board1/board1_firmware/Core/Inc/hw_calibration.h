/**
 * @file hw_calibration.h
 * @author Gruppo 2
 * @brief Definizioni dei parametri di calibrazione hardware per motori ed encoder.
 * * Contiene i valori di offset PWM (punti di arresto) e i fattori di scala per la
 * linearizzazione del movimento dei motori, oltre ai dati costanti dell'hardware
 * degli encoder.
 */

#ifndef INC_HW_CALIBRATION_H_
#define INC_HW_CALIBRATION_H_

/**
 * @name Parametri Calibrazione Motori
 * @{
 */

/** * @brief Valori PWM di arresto (Zero-thrust offset).
 * Definiscono il duty cycle al quale i motori smettono di ruotare.
 */
#define PWM_STOP_FA            (3230u) /**< Stop Front-A */
#define PWM_STOP_FB            (3230u) /**< Stop Front-B */
#define PWM_STOP_BA            (3215u) /**< Stop Back-A  */
#define PWM_STOP_BB            (3215u) /**< Stop Back-B  */

/** * @brief Fattori di scala per la marcia avanti.
 * Utilizzati per normalizzare la velocità dei motori in direzione forward.
 */
#define PWM_SCALE_FORWARD_FA   (690u)  /**< Scale Forward Front-A */
#define PWM_SCALE_FORWARD_FB   (710u)  /**< Scale Forward Front-B */
#define PWM_SCALE_FORWARD_BA   (690u)  /**< Scale Forward Back-A  */
#define PWM_SCALE_FORWARD_BB   (690u)  /**< Scale Forward Back-B  */

/** * @brief Fattori di scala per la marcia indietro.
 * Utilizzati per normalizzare la velocità dei motori in direzione backward.
 */
#define PWM_SCALE_BACKWARD_FA  (640u)  /**< Scale Backward Front-A */
#define PWM_SCALE_BACKWARD_FB  (630u)  /**< Scale Backward Front-B */
#define PWM_SCALE_BACKWARD_BA  (630u)  /**< Scale Backward Back-A  */
#define PWM_SCALE_BACKWARD_BB  (620u)  /**< Scale Backward Back-B  */

/** @} */

/**
 * @name Caratteristiche Meccaniche Encoder
 * @{
 */

/** * @brief Numero di impulsi generati dall'encoder per ogni giro dell'albero motore.
 */
#define PULSES_PER_REVOLUTION  (24u)

/** * @brief Rapporto di riduzione tra l'albero motore e la ruota finale.
 */
#define GEAR_RATIO             (51.0f)

/** @} */

#endif /* INC_HW_CALIBRATION_H_ */
