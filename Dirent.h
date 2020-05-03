//
// Created by ryarw on 4/30/2020.
//

#ifndef INC_5806FINAL_DIRENT_H
#define INC_5806FINAL_DIRENT_H

#include <cstdint>
#include <cstring>
#include "Inode.h"
#include "ext2Files.h"
#include "FileAccess.h"

struct Dirent {
    uint32_t
        iNum;
    uint16_t
        recLen;
    uint8_t
        nameLen,
        fileType,
        name[1];
};

struct Directory {
    uint32_t
        cursor;
    uint8_t
        *block;
    uint32_t
        inodeNum;
    Inode
        *inode;
    Dirent
        *dirent;
};

struct Directory *openDir(Ext2File *, uint32_t);
bool getNextDirent(struct Ext2File *, struct Directory *, uint32_t &, char *);
void rewindDir(struct Directory *);
void closeDir(struct Directory *);
uint32_t searchDir(struct Ext2File *f, uint32_t iNum, char *target);
uint32_t traversePath(Ext2File *f, char *path);

#endif //INC_5806FINAL_DIRENT_H
