//
// Created by rj on 2/14/20.
//

#ifndef INC_5806FINAL_DISPLAYBUFFERS_H
#define INC_5806FINAL_DISPLAYBUFFERS_H

#include <cstdint>
#include "PartitionEntry.h"
#include "ext2Files.h"
#include "Inode.h"

#include <iostream>
#include <iomanip>
#include "VDIFile.h"
#include "ext2Files.h"


void displayBufferPage(uint8_t *buf, uint32_t count, uint32_t start, uint64_t offset);

void displayBuffer(uint8_t *buf,int32_t count,uint64_t offset);
void displayvdiHeader(struct VDIFile *f);
void displayPartitionEnrty(struct PartitionEntry *);
void displayCHS(CHS);
void displaySuperblockContent(struct Ext2File *);
void displaySuperblockContent(struct Ext2SuperBlock *);
void displayBlockGroupDescriptorTable(struct Ext2BlockGroupDescriptor *);
void displayInode(struct Inode *);

#endif //INC_5806FINAL_DISPLAYBUFFERS_H806FINAL_DISPLAYBUFFERS_H