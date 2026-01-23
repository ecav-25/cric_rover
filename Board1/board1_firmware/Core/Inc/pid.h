#ifndef INC_PID_H_
#define INC_PID_H_

#define U_MAX (12.0)                 // Valore massimo del segnale di controllo in direzione CLOCKWISE (in volt)
#define U_MIN (-12.0)                // Valore massimo del segnale di controllo in direzione COUNTERCLOCKWISE (in volt)

/*
 * Struttura dati per la gestione di un controllore PID discreto con filtro derivativo.
 * Contiene i guadagni proporzionale, integrale e derivativo, i termini intermedi
 * per l’integrazione e la derivata filtrata, e il tempo di campionamento.
 */
typedef struct {
    float kp;              // Guadagno proporzionale
    float ki;              // Guadagno integrale (continuo)
    float kd;              // Guadagno derivativo (continuo)
    float prev_feedback;   // Valore precedente della misura per il calcolo della derivata
    float period;          // Periodo di campionamento (s)
    float tau;             // Costante di tempo del filtro derivativo
    float I_term;          // Termine integrale aggiornato a ogni ciclo
    float d_filt;          // Derivata filtrata

    float u_prev;
    float e_prev;
    float a1;
    float b0;
    float b1;
} PID_t;

/*
 * Inizializzazione del controllore PID:
 *  - imposta i guadagni e i parametri di tempo,
 *  - inizializza i termini integrale, derivativo e di feedback precedente.
 */
void  PID_init(PID_t* pid, float kp, float ki, float kd, float period, float tau, float a1, float b0, float b1);

/*
 * Calcolo dell’uscita PID discreta con limitazione e filtro derivativo:
 * 1. Calcola l’errore tra setpoint e feedback.
 * 2. Determina il termine proporzionale (P_term).
 * 3. Calcola un valore non saturato dell’uscita (u_unsat) e lo limita a U_MAX/U_MIN.
 * 4. Aggiorna il termine integrale con compensazione anti-windup in base alla differenza tra u_sat e u_unsat.
 * 5. Calcola la derivata filtrata (d_filt) usando un filtro passa-basso con costante di tempo tau.
 * 6. Determina il termine derivativo (D_term) e corregge l’uscita finale.
 * 7. Aggiorna il feedback precedente per il prossimo ciclo.
 * 8. Restituisce il valore di controllo da applicare al sistema.
 */
float PID_compute(PID_t* pid, float setpoint, float feedback);

float PID_compute_law(PID_t* pid, float setpoint, float feedback);

void PID_reset(PID_t* pid);

#endif /* INC_PID_H_ */



