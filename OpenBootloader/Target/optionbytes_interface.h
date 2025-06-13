/**
  ******************************************************************************
  * @file    optionbytes_interface.h
  * @author  MCD Application Team
  * @brief   Header for optionbytes_interface.c module
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
#ifndef OPTIONBYTES_INTERFACE_H
#define OPTIONBYTES_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"
/* Exported types ------------------------------------------------------------*/
#define FLASH_FLAG_ALL_ERRORS         ((uint32_t)(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                                                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR))
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t OPENBL_OB_Read(uint32_t Address);
//void OPENBL_OB_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength);
void OPENBL_OB_Launch(void);

#ifdef __cplusplus
}
#endif

#endif /* OPTIONBYTES_INTERFACE_H */
