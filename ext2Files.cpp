//
// Created by rjyarwood on 4/19/20.
//


#include <cstdint>
#include <unistd.h>
#include "VDIFile.h"
#include "PartitionFile.h"
#include "ext2Files.h"
#include "exceptions.cpp"

/*
 * Use vdiOpen() and partitionOpen() to open the given VDI file and use partition
 * number pNum. Populate all of the fields of your structure and return a pointer to it.
 * Pro tip: Write your code so that pNum= −1 uses the first partition with a Linux file type.
 */
struct Ext2File *ext2Open(char *fn,int32_t pNum){

    //Declaring objects for later use
    VDIFile *vdiFile = vdiOpen(fn);
    PartitionTable *partitionTable = fillPartitionTable(vdiFile);
    PartitionEntry partitionEntry;


    //If pNum is -1 we should search through partitions in vdiFile to find first one of Linux, otherwise just go to the
    //partition number given
    if(pNum == -1) {
        for (int i = 0; i < sizeof(partitionTable) / sizeof(partitionTable->partitionEntries[1]); i++) {
            if (partitionTable->partitionEntries[i].partitionType == LINUX_PARTITION) {
                partitionEntry = partitionTable->partitionEntries[i];
                break;
            }
        }
    }
    else{
        partitionEntry = partitionTable->partitionEntries[pNum];
    }

    //Opening the partition and storing it in a local object
    PartitionFile *partitionFile = partitionOpen(vdiFile, partitionEntry);

    //Creating Ext2File object to be returned later
    Ext2File *ext2File = new Ext2File;
    ext2File->partitionFile = partitionFile;
    ext2File->blockSize = 1024 << ext2File->superBlock.logBlockSize;
    ext2File->blockGroupCount = (ext2File->superBlock.blocksCount + ext2File->superBlock.blocksPerGroup -1) / ext2File->superBlock.blocksPerGroup;

    //Seeking to start of superblock and reading
    partitionSeek(ext2File->partitionFile, 1024, SEEK_SET);
    partitionRead(ext2File->partitionFile, &ext2File->superBlock, sizeof(ext2File->superBlock));

    //Seeking to next block to read group descriptor table
    partitionSeek(ext2File->partitionFile, (ext2File->superBlock.firstDataBlock + 1) * ext2File->blockSize, SEEK_SET);
    partitionRead(ext2File->partitionFile, &ext2File->blockGroup, sizeof(ext2File->blockGroup));


    return ext2File;
}

//Close the file whose pointer is given. Deallocate any dynamically created memory regions.
void ext2Close(struct Ext2File *f){

    partitionClose(f->partitionFile);
    delete f;

}

int32_t fetchBlock(struct Ext2File *f, uint32_t blockNum, void *buf){

    //First ensure the blockNum is valid
    if(blockNum < 0 || blockNum >= f->superBlock.blocksCount)
        throw badIndex;

    try {
        //If valid index seek to block location
        partitionSeek(f->partitionFile, blockNum * f->blockSize, f->partitionFile->startLoc);
        partitionRead(f->partitionFile, buf, f->blockSize);

        return 0;
    }
    catch(int errorNo)
    {
        std::cout << "Exception N. " << errorNo << "was thrown in fetchBlock" << std::endl;
        return errorNo;
    }
}

int32_t writeBlock(struct Ext2File *f, uint32_t blockNum, void *buf){

    //First ensure the blockNum is valid
    if(blockNum < 0 || blockNum > f->superBlock.blocksCount)
        throw badIndex;


    try {

        //If valid index seek to block location
        partitionSeek(f->partitionFile, blockNum * f->blockSize, f->partitionFile->startLoc);
        partitionWrite(f->partitionFile, buf, f->blockSize);

        return 0;
    }
    catch(int errorNo)
    {
        std::cout << "Exception N. " << errorNo << "was thrown in fetchBlock" << std::endl;
        return errorNo;
    }
}

int32_t fetchSuperblock(struct Ext2File *f,uint32_t blockNum, struct Ext2SuperBlock *sb){

    unsigned char *tempBlock = new unsigned char[f->blockSize];

    //First ensure the blockNum is valid
    if(blockNum < 0 || blockNum > f->superBlock.blocksCount)
        throw badIndex;

    try {

        //If valid index seek to block location
        partitionSeek(f->partitionFile, blockNum*f->blockSize, f->partitionFile->startLoc);
        fetchBlock(f, blockNum, tempBlock);

        memcpy(sb, tempBlock, 1024);

        //Checking to see if a valid superblock
        if(sb->magic == 0xef53){
            return 0;
        }
        else{
            throw invalidSuperBlock;
        }

    }
    catch(int errorNo)
    {
        std::cout << "Exception N. " << errorNo << "was thrown in fetchBlock" << std::endl;
        return errorNo;
    }

}

