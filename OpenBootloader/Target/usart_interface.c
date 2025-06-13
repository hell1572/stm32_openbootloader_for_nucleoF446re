/**
  ******************************************************************************
  * @file    usart_interface.c
  * @author  MCD Application Team
  * @brief   Contains USART HW configuration
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

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "interfaces_conf.h"
#include "openbl_core.h"
#include "openbl_usart_cmd.h"
#include "usart_interface.h"
// #include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEFAULT_USART_BAUDRATE   115200U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t UsartDetected = 0U;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USART_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the used USART instance.
  * @retval None.
  */
static void OPENBL_USART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct;

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;


  LL_USART_Init(USARTx, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USARTx);
  LL_USART_Enable(USARTx);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure USART pins and then initialize the used USART instance.
  * @retval None.
  */
void OPENBL_USART_Configuration(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable all resources clocks ---------------------------------------------*/
  /* Enable used GPIOx clocks */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /* USARTx pins configuration -----------------------------------------------*/

  GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  OPENBL_USART_Init();

  /* For debugging */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  This function is used to De-initialize the USART pins and instance.
  * @retval None.
  */
void OPENBL_USART_DeInit(void)
{
  /* Only de-initialize the USART if it is not the current detected interface */
  if (UsartDetected == 0U)
  {
    LL_USART_Disable(USARTx);

    USARTx_CLK_DISABLE();
  }
}

/**
  * @brief  This function is used to detect if there is any activity on USART protocol.
  * @retval Returns 1 if interface is detected else 0.
  */
uint8_t OPENBL_USART_ProtocolDetection(void)
{
  /* Check if the USARTx is addressed */
  /* Read byte in order to flush the 0x7F synchronization byte */
  
  if ((OPENBL_USART_ReadByte() == 0x7F))
  {
    /* Acknowledge the host */
    OPENBL_USART_SendByte(ACK_BYTE);

    UsartDetected = 1U;
  }
  else
  {
    UsartDetected = 0U;
  }

  return UsartDetected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @retval Returns the command.
  */
uint8_t OPENBL_USART_GetCommandOpcode(void)
{
  uint8_t command_opc = 0x0;

  /* Get the command opcode */
  command_opc = OPENBL_USART_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_USART_ReadByte()) != 0xFF)
  {
    command_opc = ERROR_COMMAND;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from USART pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_USART_ReadByte(void)
{
  while (!LL_USART_IsActiveFlag_RXNE(USARTx))
  {
    for (volatile uint32_t i = 0; i < 100; i++)
    {
      __NOP();
    }
  }

  return LL_USART_ReceiveData8(USARTx);
}

/**
  * @brief  This function is used to send one byte through USART pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_USART_SendByte(uint8_t Byte)
{
  LL_USART_TransmitData8(USARTx, (Byte & 0xFF));

  while (!LL_USART_IsActiveFlag_TC(USARTx))
  {
  }
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  SpecialCmd Pointer to the OPENBL_SpecialCmdTypeDef structure.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
void OPENBL_USART_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *SpecialCmd)
{
  switch (SpecialCmd->OpCode)
  {
    /* Unknown command opcode */
    default:
      // if (SpecialCmd->CmdType == OPENBL_SPECIAL_CMD)
      // {
      //   /* Send NULL data size */
      //   OPENBL_USART_SendByte(0x00U);
      //   OPENBL_USART_SendByte(0x00U);

      //   /* Send NULL status size */
      //   OPENBL_USART_SendByte(0x00U);
      //   OPENBL_USART_SendByte(0x00U);
      // }
      // else if (SpecialCmd->CmdType == OPENBL_EXTENDED_SPECIAL_CMD)
      // {
      //   /* Send NULL status size */
      //   OPENBL_USART_SendByte(0x00U);
      //   OPENBL_USART_SendByte(0x00U);
      // }
      break;
  }
}
