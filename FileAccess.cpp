//
// Created by rjyarwood on 4/29/20.
//

#include <cstring>
#include <cmath>
#include "FileAccess.h"


// TODO: ask about Allocate Function
// TODO: where does ext2File struct come from?


int32_t fetchBlockFromFile(Ext2File *f, struct Inode *i, uint32_t bNum, void *buf){

    const int blockSize = 512;
    int k = blockSize / 4;
    //Creating a temporary array so i_block does not get changed
    uint32_t blockList[15];
    unsigned int index;

    // If block number is a directly accessible block
    if(bNum < 12){

        // Copying i_block to temp array
        memcpy(blockList, i->i_block, sizeof(i->i_block));

        goto direct;
    }

    else if(bNum < (12 + k)){
        if(i->i_block[12] == 0){
            return false;
        }

        // Same as above
        fetchBlock(f,i->i_block[12], buf);

        // Copying buf to blockList
        memcpy(blockList, buf, sizeof(blockList));

        // adjust block number
        bNum = bNum - 12;

        goto direct;
    }

    else if(bNum < (12 + k + pow(k,2))){
        if(i->i_block[13] == 0){
            return false;
        }

        fetchBlock(f,i->i_block[13], buf);

        memcpy(blockList, buf, sizeof(blockList));

        bNum = bNum - 12 - k;

        goto single;

    }

    else{
        if(i->i_block[14] == 0){
            return false;
        }

        fetchBlock(f,i->i_block[14], buf);

        memcpy(blockList, buf, sizeof(blockList));

        bNum = bNum - 12 - k - pow(k, 2);
    }

    index = bNum/(int)(pow(k,2));
    bNum = bNum % (int)pow(k, 2);

    if(blockList[index] == 0){
        return false;
    }

    fetchBlock(f,blockList[index], buf);
    memcpy(blockList, buf, sizeof(blockList));


    single:

    index = bNum / k;
    bNum = bNum % k;

    if(blockList[index] == 0){
        return false;
    }

    fetchBlock(f, blockList[index], buf);
    memcpy(blockList, buf, sizeof(blockList));


    direct:

    if(blockList[bNum] == 0){
        return false;
    }

    fetchBlock(f,blockList[bNum], buf);

    return true;


}

int32_t writeBlockFromFile(Ext2File *f, struct Inode *i, uint32_t bNum, void *buf) {

    int k = 512 / 4;
    int b = bNum;\
    uint32_t blockList[15];
    auto *temp = new unsigned char [sizeof(blockList)];
    int index;

    if(b < 12){
        if(i->i_block[b] == 0){
            i->i_block[b] = Allocate(f);
            writeInode(f, i->i_block[b], i);
        }
        memcpy(blockList, i->i_block, sizeof(blockList));

        goto direct;
    }

    else if(b < (12 + k)){
        if(i->i_block[12] == 0){
            i->i_block[12] = Allocate(f);
            writeInode(f, i->i_block[12], i);
        }
        fetchBlock(f, i->i_block[12], temp);

        int ibNum = i->i_block[12];
        memcpy(blockList, temp, sizeof(blockList));
        b -= 12;

        goto direct;
    }

    else if(b < (12 + k + pow(k,2))){
        if(i->i_block[13] == 0){
            i->i_block[13] = Allocate(f);
            writeInode(f, i->i_block[13], i);
        }
        fetchBlock(f, i->i_block[13], temp);

        int ibNum = i->i_block[13];
        memcpy(blockList, temp, sizeof(blockList));
        b = b - 12 - k;

        goto single;
    }

    else{
        if(i->i_block[14] == 0){
            i->i_block[14] = Allocate(f);
            writeInode(f, i->i_block[14], i);
        }
        fetchBlock(f, i->i_block[14], temp);

        int ibNum = i->i_block[14];
        memcpy(blockList, temp, sizeof(blockList));
        b = b - 12 - k - pow(k, 2);
    }

    index = b / pow(k,2);
    b = b % (int)pow(k,2);

    if(i->i_block[index] == 0){
        i->i_block[index] = Allocate(f);
        writeBlock(f, i->i_block[index], blockList);
    }
    fetchBlock(f, i->i_block[index], temp);
    int ibNum = i->i_block[index];
    memcpy(blockList, temp, sizeof(blockList));

    single:

    index = b / k;
    b = b % k;

    if(i->i_block[index] == 0){
        i->i_block[index] = Allocate(f);
        writeBlock(f, i->i_block[index], blockList);
    }
    fetchBlock(f, i->i_block[index], temp);
    ibNum = i->i_block[index];
    memcpy(blockList, temp, sizeof(blockList));


    direct:

    if(i->i_block[b] == 0){
        i->i_block[b] = Allocate(f);
        writeBlock(f, i->i_block[b], blockList);
    }
   writeBlock(f, blockList[b], buf);
}

int32_t Allocate(Ext2File *f){

    // Used Later
    bool Found = false;
    unsigned int bitLocation;
    unsigned int byteLocation;
    int count = 0;

    // There are no free blocks
    if(f->superBlock.s_free_blocks_count == 0){
        return false;
    }

    if(f->blockGroupTable->bg_free_blocks_count == 0){
        return false;
    }

    //Temp buffer to store the block in
    auto *temp = new unsigned char[f->blockSize];

    do{
        fetchBlock(f, f->blockGroupTable->bg_block_bitmap + count, temp);

        for(unsigned int i = 0; i < sizeof(f->blockSize); i++){
            if(temp[i] == 0xFFF)
                continue;
            else{
                for(unsigned int j=0; j<8; j++){
                    if((temp[i] & 1<<j)){
                        bitLocation = j;
                        byteLocation = i;
                        temp[i] |= 1<<bitLocation;
                        Found = true;
                        break;
                    }
                }
            }
        }

        writeBlock(f, f->blockGroupTable->bg_block_bitmap + count, temp);


        count++;
    }while(!Found);

    f->superBlock.s_free_blocks_count--;
    f->blockGroupTable->bg_free_blocks_count--;

    //Write these back out

    return (byteLocation * 8) + bitLocation;


}