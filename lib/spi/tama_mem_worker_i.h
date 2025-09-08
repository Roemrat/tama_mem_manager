#pragma once

#include "tama_mem_worker.h"

typedef enum {
    SPIMemWorkerModeIdle,
    SPIMemWorkerModeChipDetect,
    SPIMemWorkerModeRead,
    SPIMemWorkerModeVerify,
    SPIMemWorkerModeErase,
    SPIMemWorkerModeWrite
} SPIMemWorkerMode;

struct SPIMemWorker {
    SPIMemChip* chip_info;
    found_chips_t* found_chips;
    SPIMemWorkerMode mode_index;
    SPIMemWorkerCallback callback;
    void* cb_ctx;
    FuriThread* thread;
    FuriString* file_name;
};

extern const SPIMemWorkerModeType tama_mem_worker_modes[];

