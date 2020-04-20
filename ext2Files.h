//
// Created by rjyarwood on 4/19/20.
//

#ifndef INC_5806FINAL_EXT2FILES_H
#define INC_5806FINAL_EXT2FILES_H

#include <cstdint>


const unsigned char LINUX_PARTITION = 1;


struct SuperBlock{
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
    struct SuperBlock
            superBlock;
    struct BlockGroup
            blockGroup;
};

struct Ext2File *ext2Open(char *fn, int32_t pNum);


void ext2Close(struct Ext2File *f);
//Close the file whose pointer is given. Deallocate any dynamically created memory regions.

#endif //INC_5806FINAL_EXT2FILES_H
