#pragma once
#include "tama_mem_app.h"

bool tama_mem_file_select(SPIMemApp* app);
bool tama_mem_file_create(SPIMemApp* app, const char* file_name);
bool tama_mem_file_delete(SPIMemApp* app);
bool tama_mem_file_create_open(SPIMemApp* app);
bool tama_mem_file_open(SPIMemApp* app);
bool tama_mem_file_write_block(SPIMemApp* app, uint8_t* data, size_t size);
bool tama_mem_file_read_block(SPIMemApp* app, uint8_t* data, size_t size);
void tama_mem_file_close(SPIMemApp* app);
void tama_mem_file_show_storage_error(SPIMemApp* app, const char* error_text);
size_t tama_mem_file_get_size(SPIMemApp* app);

