/**
  ******************************************************************************
  * @file    deadline_watchdog.c
  * @brief   Implementazione del driver Deadline Watchdog.
  ******************************************************************************
  */

#include "deadline_watchdog.h"
#include "FreeRTOS.h"
#include "task.h"

void DWD_Init(Deadline_Watchdog_t *hwd, TIM_HandleTypeDef *htim, uint32_t target_mask, DWD_Callback_t callback)
{
    hwd->htim = htim;
    hwd->target_mask = target_mask;
    hwd->on_deadline_cb = callback;

    hwd->current_mask = 0;

    __HAL_TIM_SET_COUNTER(hwd->htim, 0);
    __HAL_TIM_CLEAR_FLAG(hwd->htim, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(hwd->htim);
}

void DWD_Notify(Deadline_Watchdog_t *hwd, uint32_t flag)
{
    taskENTER_CRITICAL();
    hwd->current_mask |= flag;
    if (hwd->current_mask == hwd->target_mask)
    {
        hwd->current_mask = 0;
        __HAL_TIM_SET_COUNTER(hwd->htim, 0);
    }
    taskEXIT_CRITICAL();
}

void DWD_CheckStatus(Deadline_Watchdog_t *hwd, TIM_HandleTypeDef *htim_irq)
{
    if (htim_irq->Instance == hwd->htim->Instance)
    {
        HAL_TIM_Base_Stop_IT(hwd->htim);

        if (hwd->on_deadline_cb != NULL)
        {
            hwd->on_deadline_cb();
        }
    }
}
