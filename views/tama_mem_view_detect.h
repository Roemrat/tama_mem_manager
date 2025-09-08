#pragma once
#include <gui/view.h>

typedef struct SPIMemDetectView SPIMemDetectView;
typedef void (*SPIMemDetectViewCallback)(void* context);

View* tama_mem_view_detect_get_view(SPIMemDetectView* app);
SPIMemDetectView* tama_mem_view_detect_alloc();
void tama_mem_view_detect_free(SPIMemDetectView* app);

