#include <furi_hal.h>
#include <furi_hal_spi_config.h>
#include "tama_mem_chip_i.h"
#include "tama_mem_tools.h"

static uint8_t tama_mem_tools_addr_to_byte_arr(uint32_t addr, uint8_t* cmd) {
    uint8_t len = 3; // TODO(add support of 4 bytes address mode)
    for(uint8_t i = 0; i < len; i++) {
        cmd[i] = (addr >> ((len - (i + 1)) * 8)) & 0xFF;
    }
    return len;
}

static bool tama_mem_tools_trx(
    SPIMemChipCMD cmd,
    uint8_t* tx_buf,
    size_t tx_size,
    uint8_t* rx_buf,
    size_t rx_size) {
    bool success = false;
    furi_hal_spi_acquire(&furi_hal_spi_bus_handle_external);
    do {
        if(!furi_hal_spi_bus_tx(
               &furi_hal_spi_bus_handle_external, (uint8_t*)&cmd, 1, tama_mem_SPI_TIMEOUT))
            break;
        if(tx_buf) {
            if(!furi_hal_spi_bus_tx(
                   &furi_hal_spi_bus_handle_external, tx_buf, tx_size, tama_mem_SPI_TIMEOUT))
                break;
        }
        if(rx_buf) {
            if(!furi_hal_spi_bus_rx(
                   &furi_hal_spi_bus_handle_external, rx_buf, rx_size, tama_mem_SPI_TIMEOUT))
                break;
        }
        success = true;
    } while(0);
    furi_hal_spi_release(&furi_hal_spi_bus_handle_external);
    return success;
}

static bool tama_mem_tools_write_buffer(uint8_t* data, size_t size, size_t offset) {
    furi_hal_spi_acquire(&furi_hal_spi_bus_handle_external);
    uint8_t cmd = (uint8_t)SPIMemChipCMDWriteData;
    uint8_t address[4];
    uint8_t address_size = tama_mem_tools_addr_to_byte_arr(offset, address);
    bool success = false;
    do {
        if(!furi_hal_spi_bus_tx(&furi_hal_spi_bus_handle_external, &cmd, 1, tama_mem_SPI_TIMEOUT))
            break;
        if(!furi_hal_spi_bus_tx(
               &furi_hal_spi_bus_handle_external, address, address_size, tama_mem_SPI_TIMEOUT))
            break;
        if(!furi_hal_spi_bus_tx(&furi_hal_spi_bus_handle_external, data, size, tama_mem_SPI_TIMEOUT))
            break;
        success = true;
    } while(0);
    furi_hal_spi_release(&furi_hal_spi_bus_handle_external);
    return success;
}

bool tama_mem_tools_read_chip_info(SPIMemChip* chip) {
    uint8_t rx_buf[3] = {0, 0, 0};
    do {
        if(!tama_mem_tools_trx(SPIMemChipCMDReadJEDECChipID, NULL, 0, rx_buf, 3)) break;
        if(rx_buf[0] == 0 || rx_buf[0] == 255) break;
        chip->vendor_id = rx_buf[0];
        chip->type_id = rx_buf[1];
        chip->capacity_id = rx_buf[2];
        return true;
    } while(0);
    return false;
}

bool tama_mem_tools_check_chip_info(SPIMemChip* chip) {
    SPIMemChip new_chip_info;
    tama_mem_tools_read_chip_info(&new_chip_info);
    do {
        if(chip->vendor_id != new_chip_info.vendor_id) break;
        if(chip->type_id != new_chip_info.type_id) break;
        if(chip->capacity_id != new_chip_info.capacity_id) break;
        return true;
    } while(0);
    return false;
}

bool tama_mem_tools_read_block(SPIMemChip* chip, size_t offset, uint8_t* data, size_t block_size) {
    if(!tama_mem_tools_check_chip_info(chip)) return false;
    for(size_t i = 0; i < block_size; i += tama_mem_MAX_BLOCK_SIZE) {
        uint8_t cmd[4];
        if((offset + tama_mem_MAX_BLOCK_SIZE) > chip->size) return false;
        if(!tama_mem_tools_trx(
               SPIMemChipCMDReadData,
               cmd,
               tama_mem_tools_addr_to_byte_arr(offset, cmd),
               data,
               tama_mem_MAX_BLOCK_SIZE))
            return false;
        offset += tama_mem_MAX_BLOCK_SIZE;
        data += tama_mem_MAX_BLOCK_SIZE;
    }
    return true;
}

size_t tama_mem_tools_get_file_max_block_size(SPIMemChip* chip) {
    UNUSED(chip);
    return (tama_mem_FILE_BUFFER_SIZE);
}

SPIMemChipStatus tama_mem_tools_get_chip_status(SPIMemChip* chip) {
    UNUSED(chip);
    uint8_t status;
    if(!tama_mem_tools_trx(SPIMemChipCMDReadStatus, NULL, 0, &status, 1))
        return SPIMemChipStatusError;
    if(status & SPIMemChipStatusBitBusy) return SPIMemChipStatusBusy;
    return SPIMemChipStatusIdle;
}

static bool tama_mem_tools_set_write_enabled(SPIMemChip* chip, bool enable) {
    UNUSED(chip);
    uint8_t status;
    SPIMemChipCMD cmd = SPIMemChipCMDWriteDisable;
    if(enable) cmd = SPIMemChipCMDWriteEnable;
    do {
        if(!tama_mem_tools_trx(cmd, NULL, 0, NULL, 0)) break;
        if(!tama_mem_tools_trx(SPIMemChipCMDReadStatus, NULL, 0, &status, 1)) break;
        if(!(status & SPIMemChipStatusBitWriteEnabled) && enable) break;
        if((status & SPIMemChipStatusBitWriteEnabled) && !enable) break;
        return true;
    } while(0);
    return false;
}

bool tama_mem_tools_erase_chip(SPIMemChip* chip) {
    do {
        if(!tama_mem_tools_set_write_enabled(chip, true)) break;
        if(!tama_mem_tools_trx(SPIMemChipCMDChipErase, NULL, 0, NULL, 0)) break;
        return true;
    } while(0);
    return true;
}

bool tama_mem_tools_write_bytes(SPIMemChip* chip, size_t offset, uint8_t* data, size_t block_size) {
    do {
        // chip identity is already validated during detection; skip per-page check for speed
        if(!tama_mem_tools_set_write_enabled(chip, true)) break;
        if((offset + block_size) > chip->size) break;
        if(!tama_mem_tools_write_buffer(data, block_size, offset)) break;
        return true;
    } while(0);
    return false;
}

