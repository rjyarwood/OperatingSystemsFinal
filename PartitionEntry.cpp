//
// Created by rj on 3/2/20.
//
#include <cstdlib>
#include "PartitionEntry.h"

struct PartitionEntry *buildPartitionEntry(VDIFile *f){

    PartitionEntry *partitionEntry = new PartitionEntry;

    unsigned char *buf = reinterpret_cast<unsigned char *>(static_cast<char *>(malloc(16)));
    vdiRead(f,buf,16);

    unsigned char status = buf[0];

    partitionEntry->CHSofFirstSect.header = buf[1];
    partitionEntry->CHSofFirstSect.sectorbits = buf[2];
    partitionEntry->CHSofFirstSect.cylinderbits = buf[3];

    unsigned char PartitionType = buf[4];

    partitionEntry->CHSofLastSect.header = buf[5];
    partitionEntry->CHSofLastSect.sectorbits = buf[6];
    partitionEntry->CHSofLastSect.cylinderbits = buf[7];

    int LBAofFirstSect;
    int LBASectorCount;

    for(int i=0; i<4; i++) {
        LBAofFirstSect += (int)buf[8 + i] << (8*i);
        LBASectorCount += (int)buf[12 + i] << (8*i);
    }

    return partitionEntry;
}


struct PartitionEntry *buildPartitionEntry(unsigned char buf[]){

    PartitionEntry *partitionEntry = new PartitionEntry;


    unsigned char status = buf[0];

    partitionEntry->CHSofFirstSect.header = buf[1];
    partitionEntry->CHSofFirstSect.sectorbits = buf[2];
    partitionEntry->CHSofFirstSect.cylinderbits = buf[3];

    unsigned char PartitionType = buf[4];

    partitionEntry->CHSofLastSect.header = buf[5];
    partitionEntry->CHSofLastSect.sectorbits = buf[6];
    partitionEntry->CHSofLastSect.cylinderbits = buf[7];

    int LBAofFirstSect;
    int LBASectorCount;

    for(int i=0; i<4; i++) {
        LBAofFirstSect += (int)buf[8 + i] << (8*i);
        LBASectorCount += (int)buf[12 + i] << (8*i);
    }

    return partitionEntry;
}

