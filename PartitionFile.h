//
// Created by rj on 2/28/20.
//

#ifndef INC_5806FINAL_PARTITIONFILE_H
#define INC_5806FINAL_PARTITIONFILE_H


#include <sys/types.h>
#include "VDIFile.h"
#include "PartitionEntry.h"

struct PartitionTable{
    PartitionEntry partitionEntries[4];
};

struct PartitionFile{
    struct VDIFile *vdi;
    off_t startLoc;
    int partitionSize;
    size_t cursor;
};

struct PartitionFile *partitionOpen(struct VDIFile *, PartitionEntry);
void partitionClose(struct PartitionFile *f);
ssize_t partitionRead(struct PartitionFile *f, void *buf, size_t count);
ssize_t partitionWrite(struct PartitionFile *f, void *buf, size_t count);
off_t partitionSeek(struct PartitionFile *f, off_t offset, int anchor);
struct PartitionTable *fillPartitionTable(VDIFile *, PartitionEntry[]);

#endif //INC_5806FINAL_PARTITIONFILE_H
