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
    __uint32_t
        magic;
    __uint16_t
        versionMajor,
        versionMinor;
    __uint32_t
        postHeaderSize,
        imageType,
        imageFlags;
    char
        comment[256];
    __uint32_t
        mapOffset,
        dataOffset;
    struct VDIDiskGeometry
        oldGeometry;
    __uint32_t
        unused;
    __uint64_t
        diskSize;
    __uint32_t
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
