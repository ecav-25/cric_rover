/**
 * @file pid_law.h
 * @author Gruppo 2
 * @brief Implementazione di una legge di controllo PID in forma discreta.
 * * Il modulo definisce le strutture e le funzioni per un regolatore PID digitale
 */

#ifndef PID_LAW_H_
#define PID_LAW_H_

/** @name Limiti di Saturazione
 * @{ */
#define U_MAX (12.0f)   /**< Limite massimo della variabile di controllo (Volt) */
#define U_MIN (-12.0f)  /**< Limite minimo della variabile di controllo (Volt) */
/** @} */

/**
 * @brief Codici di ritorno per le funzioni del modulo PID.
 */
typedef enum {
    PID_LAW_OK  = 0,    /**< Operazione completata con successo */
    PID_LAW_ERR = -1    /**< Errore nei parametri o puntatore nullo */
} pid_law_status;

/**
 * @brief Struttura dati per il regolatore PID discreto.
 * * Implementa la legge di controllo:
 * $$u[k] = -a_1 \cdot u[k-1] + b_0 \cdot e[k] + b_1 \cdot e[k-1]$$
 */
typedef struct {
    float a1;       /**< Coefficiente dell'uscita precedente */
    float b0;       /**< Coefficiente dell'errore attuale */
    float b1;       /**< Coefficiente dell'errore precedente */

    float u_prev;   /**< Stato precedente dell'uscita (u[k-1]) */
    float e_prev;   /**< Stato precedente dell'errore (e[k-1]) */
} PID_Law_t;

/* ================== API ================== */

/**
 * @brief Inizializza i coefficienti e resetta la memoria del PID.
 * * @param pid Puntatore alla struttura PID_Law_t.
 * @param a1 Coefficiente per il termine autoregressivo.
 * @param b0 Coefficiente per l'errore attuale.
 * @param b1 Coefficiente per l'errore precedente.
 * @return pid_law_status PID_LAW_OK se l'inizializzazione ha successo.
 */
pid_law_status PID_Law_init(PID_Law_t* pid, float a1, float b0, float b1);

/**
 * @brief Esegue il calcolo della legge di controllo.
 * * @param pid Puntatore alla struttura PID_Law_t.
 * @param setpoint Valore di riferimento desiderato.
 * @param feedback Valore misurato dalla retroazione.
 * @param u_out Puntatore dove salvare il comando calcolato e saturato.
 * @return pid_law_status PID_LAW_OK se il calcolo è corretto.
 */
pid_law_status PID_Law_compute(PID_Law_t* pid, float setpoint, float feedback, float* u_out);

/**
 * @brief Azzera gli stati interni (u_prev ed e_prev).
 * * @param pid Puntatore alla struttura PID_Law_t.
 * @return pid_law_status PID_LAW_OK.
 */
pid_law_status PID_Law_reset(PID_Law_t* pid);

#endif /* PID_LAW_H_ */
