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

    // If block number is a directly accessible block
    if(bNum < 12){

        // Copying i_block to temp array
        memcpy(blockList, i->i_block, sizeof(i->i_block));

        return directBlockAccess(blockList, bNum, buf);
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

        return directBlockAccess(blockList, bNum, buf);
    }

    else if(bNum < (12 + k + pow(k,2))){
        if(i->i_block[13] == 0){
            return false;
        }

        fetchBlock(f,i->i_block[13], buf);

        memcpy(blockList, buf, sizeof(blockList));

        bNum = bNum - 12 - k;

        return singleIndirectAccess(blockList, bNum, k, buf);

    }

    else{
        if(i->i_block[14] == 0){
            return false;
        }

        fetchBlock(f,i->i_block[14], buf);

        memcpy(blockList, buf, sizeof(blockList));

        bNum = bNum - 12 - k - pow(k, 2);
    }

    unsigned int index = bNum/(int)(pow(k,2));
    bNum = bNum % (int)pow(k, 2);

    if(blockList[index] == 0){
        return false;
    }

    fetchBlock(f,blockList[index], buf);

    memcpy(blockList, buf, sizeof(blockList));

    singleIndirectAccess(blockList, bNum, k, buf);

}




int32_t writeBlockToFile(Ext2File *f, struct Inode *i, uint32_t bNum,void *buf){

    //Creating a temporary array so i_block does not get changed
    uint32_t blockList[15];

    int k = 512 / 4;

    if(bNum < 12){
        if(i->i_block[bNum]){
            int iNum = Allocate(f);
            writeInode(f,i->i_block[bNum],i);
        }
        memcpy(blockList, i->i_block, sizeof(blockList));

        directBlockWrite(blockList,bNum,buf);
    }

}


int32_t directBlockAccess(Ext2File *f, uint32_t blockList[], int b, void *buf){

    if(blockList[b] == 0){
        return false;
    }

    fetchBlock(f,blockList[b], buf);

    return true;
}

int32_t singleIndirectAccess(Ext2File *f, uint32_t blockList[], int b, int k, void *buf){

    unsigned int index = b / k;
    b = b % k;

    if(blockList[index] == 0){
        return false;
    }

    fetchBlock(f, blockList[index], buf);

    // Is the & correct here?
    memcpy(blockList, buf, sizeof(&blockList));

    return directBlockAccess(blockList, b, buf);
}


int32_t directBlockWrite(Ext2File *f, struct Inode *i, uint32_t blockList[], int b, void *buf) {

    if(blockList[b] == 0){
        blockList[b] = Allocate(f);
        writeInode(f,blockList[b],blockList);
    }
    writeBlock(,blockList[b], buf);
}

int32_t singleIndirectWrite(Ext2File *f, struct Inode *i, uint32_t blockList[], int b, int k, void *buf){

    int ibNum;
    void *temp = malloc(512);

    unsigned int index = b / k;
    b = b % k;

    if(blockList[index] == 0){
        blockList[index] = Allocate(f);
        writeBlock(f,blockList[index], blockList);
    }

    ibNum = blockList[index];

    fetchBlock(,blockList[index], temp);

    memcpy(blockList, temp, sizeof(&blockList));

    directBlockWrite(blockList, b, buf);

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

    if(f->blockGroup->bg_free_blocks_count == 0){
        return false;
    }

    //Temp buffer to store the block in
    auto *temp = new unsigned char[f->blockSize];

    do{
        fetchBlock(f, f->blockGroup->bg_block_bitmap + count, temp);

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

        writeBlock(f, f->blockGroup->bg_block_bitmap + count, temp);


        count++;
    }while(!Found);

    f->superBlock.s_free_blocks_count--;
    f->blockGroup->bg_free_blocks_count--;

    //Write these back out

    return (byteLocation * 8) + bitLocation;


}