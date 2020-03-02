//
// Created by rj on 2/28/20.
//

#ifndef INC_5806FINAL_PARTITIONFILE_H
#define INC_5806FINAL_PARTITIONFILE_H

#include "VDIFile.h"

struct PartitionFile{
    VDIFile *f;
    int startLoc;
    int sizePart;
};

struct PartitionFile *partitionOpen(struct VDIFile *, struct PartitionEntry *);
void partitionClose(struct PartitionFile *f);
ssize_t partitionWrite(struct PartitionFile *f, void *buf, size_t count);
off_t partitionSeek(struct PartitionFile *f, off_t offset, int anchor);

#endif //INC_5806FINAL_PARTITIONFILE_H
