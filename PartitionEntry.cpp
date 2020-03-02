//
// Created by rj on 3/2/20.
//
#include <cstdlib>
#include "PartitionEntry.h"

struct PartitionEntry *buildPartitionEntry(VDIFile *f){

    unsigned char *buf = reinterpret_cast<unsigned char *>(static_cast<char *>(malloc(16)));
    vdiRead(f,buf,16);

    unsigned char status = buf[0];

    CHS * CHSofFirstSec = new CHS{buf[1], buf[2], buf[3]};

    unsigned char PartitionType = buf[4];

    CHS * CHSofLastSec = new CHS{buf[5], buf[6], buf[7]};

    int LBAofFirstSect;
    int LBASectorCount;

    for(int i=0; i<4; i++) {
        LBAofFirstSect += (int)buf[8 + i] << (8*i);
        LBASectorCount += (int)buf[12 + i] << (8*i);
    }

    return PartitionEntry{status, CHSofFirstSec, PartitionType, CHSofLastSec};
}


struct PartitionEntry *buildPartitionEntry(unsigned char buf[]){

    unsigned char status = buf[0];

    CHS * CHSofFirstSec = new CHS{buf[1], buf[2], buf[3]};

    unsigned char PartitionType = buf[4];

    CHS * CHSofLastSec = new CHS{buf[5], buf[6], buf[7]};

    int LBAofFirstSect;
    int LBASectorCount;

    for(int i=0; i<4; i++) {
        LBAofFirstSect += (int)buf[8 + i] << (8*i);
        LBASectorCount += (int)buf[12 + i] << (8*i);
    }

    return PartitionEntry{status, CHSofFirstSec, PartitionType, CHSofLastSec};
}

