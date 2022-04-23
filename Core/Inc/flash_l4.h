/*
 * flash_l4.h
 *
 *  Created on: Apr 22, 2022
 *      Author: domin
 */

#ifndef INC_FLASH_L4_H_
#define INC_FLASH_L4_H_

#include "main.h"
//#include "flash.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int FLASH_write_at(uint32_t address, uint64_t *pData, uint32_t len_bytes);
void Flash_Read_Data (uint32_t StartPageAddress,size_t const amount,__IO uint8_t * DATA_8);
uint32_t Flash_Write_Data (uint32_t StartPageAddress, size_t const amount, uint8_t const* const bytes);

#endif /* INC_FLASH_L4_H_ */
