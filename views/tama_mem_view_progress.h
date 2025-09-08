#pragma once
#include <gui/view.h>

typedef struct SPIMemProgressView SPIMemProgressView;
typedef void (*SPIMemProgressViewCallback)(void* context);

View* tama_mem_view_progress_get_view(SPIMemProgressView* app);
SPIMemProgressView* tama_mem_view_progress_alloc();
void tama_mem_view_progress_free(SPIMemProgressView* app);
void tama_mem_view_progress_set_read_callback(
    SPIMemProgressView* app,
    SPIMemProgressViewCallback callback,
    void* cb_ctx);
void tama_mem_view_progress_set_verify_callback(
    SPIMemProgressView* app,
    SPIMemProgressViewCallback callback,
    void* cb_ctx);
void tama_mem_view_progress_set_write_callback(
    SPIMemProgressView* app,
    SPIMemProgressViewCallback callback,
    void* cb_ctx);
void tama_mem_view_progress_set_chip_size(SPIMemProgressView* app, size_t chip_size);
void tama_mem_view_progress_set_file_size(SPIMemProgressView* app, size_t file_size);
void tama_mem_view_progress_set_block_size(SPIMemProgressView* app, size_t block_size);
void tama_mem_view_progress_inc_progress(SPIMemProgressView* app);
void tama_mem_view_progress_reset(SPIMemProgressView* app);

