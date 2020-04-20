//
// Created by rj on 2/14/20.
//

#ifndef INC_5806FINAL_DISPLAYBUFFERS_CPP_H
#define INC_5806FINAL_DISPLAYBUFFERS_CPP_H

#include <cstdint>
#include "PartitionEntry.h"

void displayBufferPage(uint8_t *buf, uint32_t count, uint32_t start, uint64_t offset);

void displayBuffer(uint8_t *buf,int32_t count,uint64_t offset);
void displayvdiHeader(struct VDIFile *f);
void displayPartitionEnrty(struct PartitionEntry);
void displayCHS(CHS);

#endif //INC_5806FINAL_DISPLAYBUFFERS_CPP_H
