//
// Created by rj on 2/14/20.
//

#ifndef INC_5806FINAL_VDIFILE_H
#define INC_5806FINAL_VDIFILE_H

#include <cstdio>
#include <cstring>


struct VDIFile {
    int fileDescriptor;
    unsigned long size;
    std::string imageName;
    unsigned int imageSignature;
    unsigned int version;
    unsigned int headerSize;
    unsigned int imageType;
    unsigned int imageFlags;
    std::string imageDescription;
    unsigned int offsetBlocks;
    unsigned int offsetData;
    unsigned int cylinderCount;
    unsigned int sectorCount;
    unsigned int headCount;
    unsigned int sectorSize;
    unsigned long long int diskSize;
    unsigned int blockSize;
    unsigned int extraBlockData;
    unsigned int blockCountinHDD;
    unsigned int blocksAllocated;
    char *UUID;
    char *snapUUID;
    char *UUIDLink;
    char *parentUUID;
    size_t cursor;
    unsigned char* header;
    //const int READ_HEADER = 1;
};

    struct VDIFile *vdiOpen(char *fn);

    void vdiClose(struct VDIFile *f);

    ssize_t vdiRead(struct VDIFile *f, void *buf, size_t count);

    ssize_t vdiWrite(struct VDIFile *f, void *buf, size_t count);

    off_t vdiSeek(VDIFile *f, off_t offset, int anchor);



#endif //INC_5806FINAL_VDIFILE_H
