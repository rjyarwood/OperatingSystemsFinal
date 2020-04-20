//
// Created by rjyarwood on 4/19/20.
//


#include <cstdint>
#include "VDIFile.h"
#include "PartitionFile.h"
#include "ext2Files.h"

/*
 * Use vdiOpen() and partitionOpen() to open the given VDI file and use partition
 * number pNum. Populate all of the fields of your structure and return a pointer to it.
 * Pro tip: Write your code so that pNum= −1 uses the first partition with a Linux file type.
 */
struct Ext2File *ext2Open(char *fn,int32_t pNum){

    VDIFile *vdiFile = vdiOpen(fn);
    PartitionEntry entries[4];
    PartitionTable *partitionTable = fillPartitionTable(vdiFile, entries);
    PartitionEntry partitionEntry;


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



}

//Close the file whose pointer is given. Deallocate any dynamically created memory regions.
void ext2Close(struct Ext2File *f){


}
