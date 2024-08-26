#ifndef DMA_H
#define DMA_H

#include <common.h>
#include <bus.h>

void dma_start(u8 start);
void dma_tick();

bool dma_transferring();

#endif