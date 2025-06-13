/**
  ******************************************************************************
  * @file    interfaces_conf.h
  * @author  MCD Application Team
  * @brief   Contains Interfaces configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACES_CONF_H
#define INTERFACES_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_spi.h"

#define MEMORIES_SUPPORTED                7U


/*-------------------------- Definitions for USART ---------------------------*/
#define USARTx                            USART2
#define USARTx_CLK_ENABLE()               __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_CLK_DISABLE()              __HAL_RCC_USART2_CLK_DISABLE()
#define USARTx_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_DEINIT()                   LL_USART_DeInit(USARTx)

#define USARTx_TX_PIN                     GPIO_PIN_2 /* PA 2 => CN9/pin2 */
#define USARTx_TX_GPIO_PORT               GPIOA
#define USARTx_RX_PIN                     GPIO_PIN_3 /* PA 3 => CN9/pin1 */
#define USARTx_RX_GPIO_PORT               GPIOA
#define USARTx_ALTERNATE                  GPIO_AF7_USART2



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERFACES_CONF_H */
