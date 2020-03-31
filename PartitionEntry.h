//
// Created by rj on 3/2/20.
//

#ifndef INC_5806FINAL_PARTITIONENTRY_H
#define INC_5806FINAL_PARTITIONENTRY_H

#include "VDIFile.h"

struct CHS{
    unsigned char header;
    unsigned char sectorbits;
    unsigned char cylinderbits;
};

struct PartitionEntry{
    unsigned char status;
    struct CHS CHSofFirstSec;
    struct CHS CHSofLastSect;
    unsigned char partitionType;
    int LBAofFirstSect;
    int LBASectorCount;
};


/*
struct CHS *buildCHS(void*);
struct CHS *buildCHS(VDIFile *);
*/

struct PartitionEntry *buildPartitionEntry(unsigned char);
struct PartitionEntry *buildPartitionEntry(VDIFile *);

#endif //INC_5806FINAL_PARTITIONENTRY_H
