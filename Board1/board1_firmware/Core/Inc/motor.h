#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#define MAX_DUTY (100)               // Duty cycle massimo espresso in percentuale
#define MIN_DUTY (0)                 // Duty cycle minimo (motore fermo)

typedef enum wise_enum{
	CLOCKWISE = 1,				// Rotazione oraria
	COUNTERCLOCKWISE = -1		// Rotazione antioraria
}wise_t;

typedef struct {
	TIM_HandleTypeDef* htim;	// Handle del timer usato per generare il PWM
	uint32_t channel;			// Canale del timer associato al PWM del motore
	uint16_t pwm_scale_forward;
	uint16_t pwm_scale_backward;
	uint16_t pwm_stop;
} Motor_t;

/*
 * Inizializza la struttura del motore associandola al timer e al canale PWM specificati.
 * Imposta il registro CCR a zero e avvia il segnale PWM sul canale configurato.
 */
void motor_init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, uint16_t pwm_stop, uint16_t pwm_scale_forward, uint16_t pwm_scale_backward);

/*
 * Imposta il valore PWM e la direzione di rotazione del motore.
 * - Limita il duty cycle al massimo consentito.
 * - Calcola il valore PWM in base alla direzione:
 *   	CLOCKWISE incrementa rispetto a PWM_STOP,
 *   	COUNTERCLOCKWISE decrementa rispetto a PWM_STOP.
 * - Se il duty è minimo, il motore è fermo.
 * Aggiorna infine il registro di confronto del timer con il valore calcolato.
 */
void motor_set(Motor_t* motor, uint8_t duty, wise_t dir);

#endif /* INC_MOTOR_H_ */
