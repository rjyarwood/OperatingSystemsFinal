//
// Created by rjyarwood on 4/19/20.
//

#ifndef INC_5806FINAL_EXT2FILES_H
#define INC_5806FINAL_EXT2FILES_H

#include <cstdint>


const unsigned char LINUX_PARTITION = 1;


struct Ext2SuperBlock{
    __uint32_t
        inodesCount,
        blocksCount,
        rBlocksCount,
        freeBlocksCount,
        freeInodesCount,
        firstDataBlock,
        logBlockSize,
        logFragSize,
        blocksPerGroup,
        fragsPerGroup,
        inodesPerGroup,
        mtime,
        wtime;
    __uint8_t
        mntCount,
        maxmntCount,
        magic,
        state,
        errors,
        minorRevLevel;
    __uint32_t
        lastCheck,
        checkInterval,
        creatorOS,
        revLevel;
    __uint16_t
        defResuid,
        defResgid;
};

struct BlockGroup{
    __uint32_t
        blockBitmap,
        inodeBitmap,
        inodeTable;
    __uint16_t
        freeBlocksCount,
        freeInodesCount,
        usedDirsCount,
        pad;
};

struct Ext2File{
    int fileDescriptor;
    struct Ext2SuperBlock
            superBlock;
    struct BlockGroup
            blockGroup;
    PartitionFile
            *partitionFile;
};

struct Ext2File *ext2Open(char *fn, int32_t pNum);
void ext2Close(struct Ext2File *f);

int32_t fetchBlock(struct Ext2File*, uint32_t, void *);
int32_t writeBlock(struct Ext2File*, uint32_t, void *);

int32_t fetchSuperblock(struct Ext2File*,uint32_t, struct Ext2SuperBlock*);
int32_t writeSuperblock(struct Ext2File*,uint32_t, struct Ext2SuperBlock*);

#endif //INC_5806FINAL_EXT2FILES_H
