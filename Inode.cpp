//
// Created by rjyarwood on 4/24/20.
//

#include <cstring>
#include "Inode.h"


//TODO add security to functions to ensure no crashes and correct returns

// Grabs an iNode from the Ext2File and reads it into the Inode buffer
int32_t fetchInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf){

    // Inode starts index at 1 so decrement index
    iNum--;

    // To find the right block group get the inodes per group from super block and find descriptor number with that info
    int blockGroupIndex = iNum / f->superBlock.s_inodes_per_group;
    // Find Index of Inode within the group containing the inode
    int inodeIndexWithinGroup = iNum % f->superBlock.s_inodes_per_group;
    // Find how many inodes are within each block
    int inodesPerBlock = f->blockSize / f->superBlock.s_inode_size;
    // Find what block inode is in
    int inodeBlock = inodeIndexWithinGroup / inodesPerBlock;
    // Find index within block
    int inodeWithinBlock = inodeIndexWithinGroup % inodesPerBlock;


    // allocate a temporary buf to store the fetched block in
    char* temp = new char[f->blockSize];
    //Add inode table to start at the right block
    fetchBlock(f, f->blockGroupTable[blockGroupIndex].bg_inode_table + inodeBlock, temp);

    *buf = ((Inode *)temp)[inodeWithinBlock];

    delete [] temp;

    return 0;
}

Inode writeInode(struct Ext2File *f, uint32_t iNum, struct Inode *buf){

    // Inode starts index at 1 so decrement index
    iNum--;

    // To find the write block group get the inodes per group from super block and find descriptor number with that info
    int blockGroupIndex = iNum / f->superBlock.s_inodes_per_group;
    // Find Index of Inode within the group containing the inode
    int inodeIndexWithinGroup = iNum % f->superBlock.s_inodes_per_group;
    // Fetch that block
    char* temp = new char[f->blockSize];
    fetchBlock(f, inodeIndexWithinGroup/f->blockSize, temp); //I Think it need to add how many blocks in the group is too

    int inodesPerBlock = f->blockSize / f->superBlock.s_inode_size;

    memcpy(temp + (inodeIndexWithinGroup % inodesPerBlock), buf, f->superBlock.s_inode_size);

    return reinterpret_cast<Inode &&>(temp);
}

int32_t inodeInUse(struct Ext2File *f,uint32_t iNum){
    iNum--;

    auto* bgdt = new Ext2BlockGroupDescriptor;

    // To find the write block group get the inodes per group from super block and find descriptor number with that info
    int blockGroupIndex = iNum / f->superBlock.s_inodes_per_group;
    fetchBGDT(f, blockGroupIndex, bgdt);
    // Find Index of Inode within the group containing the inode
    int inodeIndexWithinGroup = iNum % f->superBlock.s_inodes_per_group;
    // Find find how many blocks into the bitmap it will be and it's index in that block
    int inodesPerBlock = f->blockSize / f->superBlock.s_inode_size;
    int blockindex = inodeIndexWithinGroup / inodesPerBlock;
    int indexInBlock = inodeIndexWithinGroup % inodesPerBlock;

    //Allocate buf to store entire block
    char* temp = new char[f->blockSize];
    //Add inode table to start at the right block
    fetchBlock(f, bgdt->bg_inode_table + blockindex, temp);

    return temp[indexInBlock];
}

uint32_t allocateInode(struct Ext2File *f,int32_t group){
    auto* bgdt = new Ext2BlockGroupDescriptor;

    // Get block group descriptor Table
    fetchBGDT(f, group, bgdt);

    //Allocate buf to store entire block
    char* temp = new char[f->blockSize];

    // Read start of inode table
    fetchBlock(f, bgdt->bg_inode_table, temp);

    int index = 0;
    //Find unallocated inode
    for(int i=0; i<sizeof(f->blockSize); i++){
        if(temp[i] == 0){
            temp[i] ==1;
            index = i;
            break;
        }
    }

    //Write block back
    writeBlock(f, bgdt->bg_inode_table, temp);

    return index;
}

int32_t freeInode(struct Ext2File *f,uint32_t iNum){

    iNum--;

    auto* bgdt = new Ext2BlockGroupDescriptor;

    // To find the write block group get the inodes per group from super block and find descriptor number with that info
    int blockGroupIndex = iNum / f->superBlock.s_inodes_per_group;
    fetchBGDT(f, blockGroupIndex, bgdt);
    // Find Index of Inode within the group containing the inode
    int inodeIndexWithinGroup = iNum % f->superBlock.s_inodes_per_group;
    // Find find how many blocks into the bitmap it will be and it's index in that block
    int inodesPerBlock = f->blockSize / f->superBlock.s_inode_size;
    int blockindex = inodeIndexWithinGroup / inodesPerBlock;
    int indexInBlock = inodeIndexWithinGroup % inodesPerBlock;

    //Allocate buf to store entire block
    char* temp = new char[f->blockSize];
    //Add inode table to start at the right block
    fetchBlock(f, bgdt->bg_inode_table + blockindex, temp);

    //Mark as free
    temp[indexInBlock] = 0;

    return 0;
}
