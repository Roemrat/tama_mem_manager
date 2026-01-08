#pragma once

#include "tama_mem_chip.h"

#define tama_mem_SPI_TIMEOUT 1000
#define tama_mem_MAX_BLOCK_SIZE 512
#define tama_mem_FILE_BUFFER_SIZE 8192

bool tama_mem_tools_read_chip_info(SPIMemChip* chip);
bool tama_mem_tools_read_block(SPIMemChip* chip, size_t offset, uint8_t* data, size_t block_size);
size_t tama_mem_tools_get_file_max_block_size(SPIMemChip* chip);
SPIMemChipStatus tama_mem_tools_get_chip_status(SPIMemChip* chip);
bool tama_mem_tools_erase_chip(SPIMemChip* chip);
bool tama_mem_tools_write_bytes(SPIMemChip* chip, size_t offset, uint8_t* data, size_t block_size);

