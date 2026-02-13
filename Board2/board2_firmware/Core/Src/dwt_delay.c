// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    dwt_delay.c
 * @author  Gruppo 2
 * @brief   Implementazione del driver per ritardi basati su DWT (Data Watchpoint and Trace).
 * @details Fornisce funzioni di ritardo bloccante (busy-wait) con risoluzione al microsecondo,
 * indipendenti dal SysTick e basate sulla frequenza di core (SystemCoreClock).
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "dwt_delay.h"

/** @brief Flag interno per indicare l'avvenuta inizializzazione del DWT. */
static uint8_t s_ready = 0U;

/**
 * @brief Inizializza il contatore di cicli DWT e ne verifica il funzionamento.
 * @details Abilita il bit TRCENA nel registro DEMCR del CoreDebug e il bit CYCCNTENA
 * nel registro di controllo DWT. Esegue un test pratico per verificare che il registro
 * CYCCNT stia effettivamente incrementando.
 *
 * @return dwt_status_t DWT_OK se il contatore è attivo e funzionante,
 * DWT_ERR_UNSUPPORTED se l'hardware non supporta DWT o l'abilitazione fallisce.
 */
dwt_status_t DWT_Delay_Init(void)
{
    dwt_status_t status = DWT_ERR_UNSUPPORTED;

#if defined(DWT) && defined(CoreDebug) && defined(CoreDebug_DEMCR_TRCENA_Msk) && defined(DWT_CTRL_CYCCNTENA_Msk)
    uint32_t before;
    uint32_t after;
    volatile int32_t i;

    /* Abilita l'accesso al DWT tramite il CoreDebug */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Reset e abilitazione del contatore di cicli */
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Test di verifica funzionamento */
    before = DWT->CYCCNT;
    for (i = 0; i < 64; i++)
    {
        __NOP();
    }
    after = DWT->CYCCNT;

    if (after != before)
    {
        s_ready = 1U;
        status = DWT_OK;
    }
    else
    {
        s_ready = 0U;
        status = DWT_ERR_UNSUPPORTED;
    }
#endif

    return status;
}

/**
 * @brief Verifica se il driver DWT è pronto all'uso.
 * @return uint8_t 1 se inizializzato e attivo, 0 altrimenti.
 */
uint8_t DWT_Delay_IsReady(void)
{
    uint8_t ready_flag = 0U;

#if defined(DWT) && defined(DWT_CTRL_CYCCNTENA_Msk)
    /* Verifica sia il flag software che il bit hardware */
    if ((s_ready != 0U) && ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) != 0U))
    {
        ready_flag = 1U;
    }
#endif

    return ready_flag;
}

/**
 * @brief Funzione core per l'attesa attiva basata sui cicli di clock.
 * @details Utilizza l'aritmetica unsigned a 32 bit per gestire automaticamente
 * l'eventuale overflow del registro CYCCNT durante l'attesa.
 * @param cycles Numero di cicli di clock da attendere.
 */
static inline void dwt_delay_cycles(uint32_t cycles)
{
    uint32_t start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < cycles)
    {
        /* Busy wait */
    }
}

/**
 * @brief Esegue un ritardo bloccante in microsecondi.
 * @details Calcola il numero di cicli necessari basandosi su SystemCoreClock.
 * $$Cycles = \frac{SystemCoreClock \cdot \mu s}{1000000}$$
 * @param us Numero di microsecondi di ritardo.
 * @return dwt_status_t DWT_OK se eseguito, DWT_ERR_NOT_READY se non inizializzato.
 */
dwt_status_t DWT_Delay_us(uint32_t us)
{
    dwt_status_t status = DWT_ERR_NOT_READY;
    uint64_t cycles;

    if (DWT_Delay_IsReady() != 0U)
    {
        cycles = ((uint64_t)SystemCoreClock * (uint64_t)us) / 1000000ULL;

        if (cycles == 0ULL)
        {
            cycles = 1ULL;
        }

        dwt_delay_cycles((uint32_t)cycles);
        status = DWT_OK;
    }

    return status;
}

/**
 * @brief Esegue un ritardo bloccante in millisecondi.
 * @details Implementato come loop di chiamate a DWT_Delay_us(1000) per evitare
 * overflow nel calcolo dei cicli per ritardi lunghi (> alcuni secondi a frequenze elevate).
 * @param ms Numero di millisecondi di ritardo.
 * @return dwt_status_t DWT_OK se eseguito, DWT_ERR_NOT_READY se non inizializzato.
 */
dwt_status_t DWT_Delay_ms(uint32_t ms)
{
    dwt_status_t status = DWT_ERR_NOT_READY;
    uint32_t ms_count = ms;

    if (DWT_Delay_IsReady() != 0U)
    {
        while (ms_count > 0U)
        {
            (void)DWT_Delay_us(1000U);
            ms_count--;
        }
        status = DWT_OK;
    }

    return status;
}
