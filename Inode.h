//
// Created by rjyarwood on 4/24/20.
//

#ifndef INC_5806FINAL_INODE_H
#define INC_5806FINAL_INODE_H

#include <cstdint>
#include "ext2Files.h"


struct Inode {
    uint16_t
            i_mode,
            i_uid;
    uint32_t
            i_size,
            i_atime,
            i_ctime,
            i_mtime,
            i_dtime;
    uint16_t
            i_gid,
            i_links_count;
    uint32_t
            i_blocks,
            i_flags,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            i_osd1,
#pragma clang diagnostic pop
            i_block[15],
            i_generation,
            i_file_acl,
            i_sizeHigh,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            i_faddr;
#pragma clang diagnostic pop
    uint16_t
            i_blocksHigh,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            reserved16,
#pragma clang diagnostic pop
            i_uidHigh,
            i_gidHigh;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
    uint32_t
            reserved32;
#pragma clang diagnostic pop
};

int32_t fetchInode(struct Ext2File*, uint32_t, struct Inode *);
Inode writeInode(struct Ext2File*, uint32_t, struct Inode *);
int32_t inodeInUse(struct Ext2File*,uint32_t);
uint32_t allocateInode(struct Ext2File*,int32_t);
int32_t freeInode(struct Ext2File*,uint32_t);

#endif //INC_5806FINAL_INODE_H
