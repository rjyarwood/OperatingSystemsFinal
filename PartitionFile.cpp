//
// Created by rj on 2/28/20.
//

#include "PartitionFile.h"


/*
 * @param vdi: The VDIFile that will be placed in the new PartitionFile struct
 * @param pe: The PartitionEntry info to be placed in the PartitionFile struct
 */
PartitionFile* partitionOpen(struct VDIFile *vdi, PartitionEntry pe){

    /*
     * This should just take the given vdi file and place it in the PartitionFile struct and find the start location
     * and size of the given partition from the PartitionEntry struct
     */

    PartitionFile* partitionFile = new PartitionFile;

    partitionFile->vdi = vdi;
    partitionFile->startLoc = pe.LBAofFirstSect * 512;
    partitionFile->partitionSize = pe.LBASectorCount * 512;
    partitionFile->cursor = SEEK_SET;

    return partitionFile;
}


/*
 * @param f: The PartitionFile whose VDI file is to be closed
 */
void partitionClose(struct PartionFile *f){
    /*
     * This should call vdiClose to close the vdi file associated with this PartitionFile
     */
    VDIFile *vdiFile = f->vdif;


    vdiClose(vdiFile);
    delete f;
}

/*
 * @param f: The PartitionFile to read
 * @param buf: The buffer to read to
 * @param count: How much to read
 */
ssize_t partitionRead(struct PartitionFile *f, void *buf, size_t count){

    //If the given count would read beyond the partition, set the count to just read to the end
    if(f->cursor + count > f->partitionSize){
        count = f->partitionSize - f->cursor;
    }

    //Calls VDI Seek to set the cursor to the location of the partition and then read count bytes from there
    vdiSeek(f->vdi, f->cursor, SEEK_SET);
    vdiRead(f->vdi, buf, count);


    return count;

}

/*
 * @param f: The PartitionFile to write to.
 * @param buf: The buffer to be read from
 * @param count: The amount of characters to be written
 */
ssize_t partitionWrite(struct PartitionFile *f, void *buf, size_t count){

    //If the count would write beyond partition then restrict count to only write to the end
    if(f->cursor + count > f->partitionSize){
        count = f->partitionSize - f->cursor;
    }

    //Call VDISeek to set the cursor and VDIWrite to write to the file
    vdiSeek(f->vdi, f->cursor, SEEK_SET);
    vdiWrite(f->vdi,buf,count);

    return count;

}

/*
 * @param f: The PartitionFile whose cursor will be set
 * @param offset: How much to move by
 * @param anchor: Where the offset is in reference to
 */
off_t partitionSeek(struct PartitionFile *f, off_t offset, int anchor){

    size_t newLocation;

    if(anchor == SEEK_SET){
        newLocation = offset;
    }
    else if(anchor == SEEK_CUR){
        newLocation = f->cursor + offset;
    }
    else{
        newLocation = f->partitionSize - offset;
    }

    if(newLocation >= 0 && newLocation < f->partitionSize){
        f->cursor = newLocation;
    }


    return f->cursor;

}

struct PartitionTable *fillPartitionTable(VDIFile *vdi){

    PartitionTable partitionTable = new PartitionTable;


    vdiSeek(vdi, 446, SEEK_SET);
    vdiRead(vdi, partitionTable, 64);

    return new partitionTable;
}



