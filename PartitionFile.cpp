//
// Created by rj on 2/28/20.
//

#include "PartitionFile.h"


PartitionFile* partitionOpen(struct VDIFile *vdi, struct PartitionEntry* pe){

    /*
     *
     */



    PartitionFile* partitionFile = new PartitionFile{vdi, startloc, size};
    return partitionFile;
}

