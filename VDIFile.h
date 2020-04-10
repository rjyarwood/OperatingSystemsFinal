//
// Created by rj on 2/14/20.
//

#ifndef INC_5806FINAL_VDIFILE_H
#define INC_5806FINAL_VDIFILE_H

#include <cstdio>
#include <cstring>



struct VDIDiskGeometry{
    unsigned int
        cylinderCount,
        sectorCount,
        headCount,
        sectorSize;
};

struct VDIHeader{
    char
        fileInfo[64];
    //uint32_t
        //magic;
    uint16_t
        imageSignature,
        versionMajor;
        //versionMinor;
    uint32_t
        postHeaderSize,
        imageType,
        imageFlags;
    //struct VDIDiskGeometry
        //oldGeometry;
    uint32_t
        pageSize,
        extraPageSize,
        nPagesTotal,
        nPagesAllocated;
    struct UUID
        uuidCreate,
        uuidModify,
        uuidPrevImage,
        uuidPrevImageModify;
    struct VDIDiskGeometry
        diskGeometry;
};

struct VDIFile {
    int fileDescriptor;
    size_t cursor;
    unsigned int* map;
    struct VDIHeader VDIHeader;
    int fileSize;
    //const int READ_HEADER = 1;
};


    struct VDIFile *vdiOpen(char *fn);

    void vdiClose(struct VDIFile *f);

    ssize_t vdiRead(struct VDIFile *f, void *buf, size_t count);

    ssize_t vdiWrite(struct VDIFile *f, void *buf, size_t count);

    off_t vdiSeek(VDIFile *f, off_t offset, int anchor);



#endif //INC_5806FINAL_VDIFILE_H
