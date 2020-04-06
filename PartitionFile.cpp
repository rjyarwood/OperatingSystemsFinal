//
// Created by rj on 2/28/20.
//

#include "PartitionFile.h"


/*
 * @param vdi: The VDIFile that will be placed in the new PartitionFile struct
 * @param pe: The PartitionEntry to be placed in the PartitionFile struct
 */
PartitionFile* partitionOpen(struct VDIFile *vdi, struct PartitionEntry* pe){

    /*
     * This should just take the inputs and plug them directly into a PartitionFile struct and return the pointer.
     */

    PartitionFile* partitionFile = new PartitionFile{vdi, pe};
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
    lseek()
}




