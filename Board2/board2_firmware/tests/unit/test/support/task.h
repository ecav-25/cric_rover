#ifndef INC_TASK_H
#define INC_TASK_H

// CMock genererà i mock per queste funzioni
void vTaskEnterCritical(void);
void vTaskExitCritical(void);

// Macro usate nel tuo codice C
#define taskENTER_CRITICAL() vTaskEnterCritical()
#define taskEXIT_CRITICAL()  vTaskExitCritical()

#endif