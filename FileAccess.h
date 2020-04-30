//
// Created by rjyarwood on 4/29/20.
//

#ifndef INC_5806FINAL_FILEACCESS_H
#define INC_5806FINAL_FILEACCESS_H

#include <cstdint>
#include "Inode.h"

int32_t fetchBlockFromFile(struct Inode *, uint32_t,void *);
int32_t writeBlockToFile(struct Inode *, uint32_t,void *);

int32_t directBlockAccess(uint32_t blockList[], int b, void *buf);
int32_t singleIndirectAccess(uint32_t blockList[], int b, int k, void *buf);

int32_t directBlockWrite(uint32_t blockList[], int b, void *buf);
int32_t singleIndirectWrite(uint32_t blockList[], int b, int k, void *buf);

int32_t Allocate(Ext2File *f);

#endif //INC_5806FINAL_FILEACCESS_H
