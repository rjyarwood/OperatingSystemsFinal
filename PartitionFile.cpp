//
// Created by rj on 2/28/20.
//

#include "PartitionFile.h"


/*
 * @param vdi: The VDIFile that will be placed in the new PartitionFile struct
 * @param pe: The PartitionEntry to be placed in the PartitionFile struct
 */
PartitionFile* partitionOpen(struct VDIFile *vdi, PartitionTable pt){

    /*
     * This should just take the inputs and plug them directly into a PartitionFile struct and return the pointer.
     */

    PartitionFile* partitionFile = new PartitionFile;

    partitionFile->vdif = vdi;
    partitionFile->partitionTable = pt;

    return partitionFile;
}


/*
 * @param f: The PartitionFile whose VDI file is to be closed
 */
void partitionClose(struct PartionFile *f){
    /*
     * This should call vdiClose to close the vdi file associated with this PartitionFile
     */
    VDIFile *vdiFile = f.vdif;


    vdiClose(vdiFile);
}

/*
 * @param f: The PartitionFile to read
 * @param buf: The buffer to read to
 * @param count: How much to read
 */
ssize_t partitionRead(struct PartitionFile *f, void *buf, size_t count){



}

/*
 * @param f: The PartitionFile to write to.
 * @param buf: The buffer to be read from
 * @param count: The amount of characters to be written
 */
ssize_t partitionWrite(struct PartitionFile *f, void *buf, size_t count){

}

/*
 * @param f: The PartitionFile whose cursor will be set
 * @param offset: How much to move by
 * @param anchor: Where the offset is in reference to
 */
off_t partitionSeek(struct PartitionFile *f, off_t offset, int anchor){

}

struct PartitionFile *fillPartitionTable(VDIFile *vdi, PartitionEntry[] entries){

    unsigned char *buf = reinterpret_cast<unsigned char *>(static_cast<char *>(malloc(64)));

    vdiSeek(vdi, 446, 0);
    vdiRead(vdi, buf, 64);

    PartitionTable partitionTable = new PartitionTable;

    PartitionEntry tempEntry = new PartitionEntry;

    for(int i=0; i<4; i++){
        tempEntry.status = buf[(i*16)+0];
        tempEntry.CHSofFirstSect = new CHS {buf[(i*16)+1], buf[(i*16)+2], buf[(i*16)+3]};
        tempEntry.partitionType = buf[(i*16)+4];
        tempEntry.CHSofLastSect = new CHS {buf[(i*16)+5], buf[(i*16)+6], buf[(i*16)+7]};
        tempEntry.LBAofFirstSect = buf[(i*16)+8] + buf[(i*16)+9] << 4 + buf[(i*16)+10] << 8 + buf[(i*16)+11] << 12;
        tempEntry.LBAofFirstSect = buf[(i*16)+12] + buf[(i*16)+13] << 4 + buf[(i*16)+14] << 8 + buf[(i*16)+15] << 12;

        partitionTable.partitionEntries[i] = tempEntry;
    }

    return new PartitionFile{vdi, partitionTable};

}



