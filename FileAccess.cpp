//
// Created by rjyarwood on 4/29/20.
//

#include <cstring>
#include "FileAccess.h"

int32_t fetchBlockFromFile(struct Inode *i, uint32_t bNum, void *buf){

    const int blockSize = 512 * 8;
    int k = blockSize / 4;

    // If block number is a directly accessible block
    if(bNum < 12){
        //Creating a temporary array so i_block does not get changed
        uint32_t blockList[15];
        memcpy(blockList, i->i_block, sizeof(uint32_t) * 15);

        //If the block is empty
        if(blockList[bNum] == 0){
            return false;
        }


    }


}




int32_t writeBlockToFile(struct Inode *i, uint32_t bNum,void *buf){



}