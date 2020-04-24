//
// Created by rjyarwood on 4/24/20.
//

#include "Inode.h"

// Grabs an iNode from the Ext2File and reads it into the Inode buffer
int32_t fetchInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf){

    // Inode starts index at 1 so decrement index
    iNum--;

    // To find the write block group get the inodes per group from super block and find descriptor number with that info
    int blockGroupIndex = iNum / f->superBlock.s_inodes_per_group;
    // Find Index of Inode within the given block by finding how many inodes were skipped getting to the right block
    int inodeIndexWithinBlock = iNum - (f->superBlock.s_inodes_per_group * blockGroupIndex);

    Ext2BlockGroupDescriptor *bgdt = new Ext2BlockGroupDescriptor [];

    int status = fetchBGDT(f, blockGroupIndex, bgdt);


}