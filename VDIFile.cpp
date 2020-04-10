//
// Created by rj on 2/12/20.
//


#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include "displayBuffers.cpp.h"
#include "uuid2ascii.cpp"
#include "VDIFile.h"


    struct VDIFile *vdiOpen(char *fn) {
        VDIHeader *vdiHead = new VDIHeader;
        VDIFile *vdiFile = new VDIFile;


        //Opening the VDIFile and saving file descriptor
        vdiFile->fileDescriptor = open(fn, O_CREAT);
        //std::cout<<vdiFile->fileDescriptor << std::endl;

        // If file does not exist
        if(vdiFile->fileDescriptor < 1)
            std::cout << strerror(errno) << std::endl;

        //Finding size of file using lseek
        vdiFile->fileSize = lseek(vdiFile->fileDescriptor,0,SEEK_END);

        //Setting cursor to 0
        vdiFile->cursor = 0;

        //Creating and reading file Header into buffer
        unsigned char *buf = reinterpret_cast<unsigned char *>(static_cast<char *>(malloc(444)));
        pread(vdiFile->fileDescriptor, buf,444,0);
        lseek(vdiFile->fileDescriptor,0,0);

        for(int i=0; i<64; i++) {
            vdiHead->fileInfo[i] = buf[i];
        }



        for(int i=0; i<4; i++) {

            vdiHead->imageSignature += (int)buf[0x40 + i] << (8*i);
            vdiHead->versionMajor += (int)buf[0x44 + i] << (8*i);
            vdiHead->postHeaderSize += (int)buf[0x48 +i] << (8*i);
            vdiHead->imageType += (int)buf[0x4c + i] << (8*i);
            vdiHead->imageFlags += (int)buf[0x50 + i] << (8*i);
            vdiHead->pageSize += (int)buf[0x178 + i] << (8*i);
            vdiHead->extraPageSize += (int)buf[0x17c + i] << (8*i);
            vdiHead->nPagesTotal += (int)buf[0x180 + i] << (8*i);
            vdiHead->nPagesAllocated += (int)buf[0x184 + i] << (8*i);

            vdiHead->uuidCreate.timeLow += (int)buf[0x188 + i] << (8*i);
            vdiHead->uuidModify.timeLow += (int)buf[0x198 + i] << (8*i);
            vdiHead->uuidPrevImage.timeLow += (int)buf[0x1a8 + i] << (8*i);
            vdiHead->uuidPrevImageModify.timeLow += (int)buf[0x1b8 + i] << (8*i);


            vdiHead->diskGeometry.cylinderCount += (int)buf[0x15c +i] << (8*i);
            vdiHead->diskGeometry.headCount += (int)buf[0x160 + i] << (8*i);
            vdiHead->diskGeometry.sectorCount += (int)buf[0x164 + i] << (8*i);
            vdiHead->diskGeometry.sectorSize += (int)buf[0x168 + i] << (8*i);

        }

        for(int i = 0; i<2; i++){
            vdiHead->uuidCreate.timeMid += (int)buf[0x18c +i] << (8*i);
            vdiHead->uuidCreate.timeHigh += (int)buf[0x18e +i] << (8*i);
            vdiHead->uuidCreate.clock += (int)buf[0x190 +i] << (8*i);

            vdiHead->uuidModify.timeMid += (int)buf[0x19c +i] << (8*i);
            vdiHead->uuidModify.timeHigh += (int)buf[0x19e +i] << (8*i);
            vdiHead->uuidModify.clock += (int)buf[0x1a0 +i] << (8*i);

            vdiHead->uuidPrevImage.timeMid += (int)buf[0x1ac +i] << (8*i);
            vdiHead->uuidPrevImage.timeHigh += (int)buf[0x1ae +i] << (8*i);
            vdiHead->uuidPrevImage.clock += (int)buf[0x1b0 +i] << (8*i);

            vdiHead->uuidPrevImageModify.timeMid += (int)buf[0x1bc +i] << (8*i);
            vdiHead->uuidPrevImageModify.timeHigh += (int)buf[0x1abe +i] << (8*i);
            vdiHead->uuidPrevImageModify.clock += (int)buf[0x1c0 +i] << (8*i);


        }

        for(int i = 0; i<6; i++){
            vdiHead->uuidCreate.node[i] = (int)buf[0x192 + i];
            vdiHead->uuidModify.node[i] = (int)buf[0x1a2 + i];
            vdiHead->uuidPrevImage.node[i] = (int)buf[0x1b2 + i];
            vdiHead->uuidPrevImageModify.node[i] = (int)buf[0x1c2 + i];

        }


        //Placing vdiHeader into VDIFile?
        vdiFile->VDIHeader = reinterpret_cast<VDIHeader &&>(vdiHead);

        //sends to display buffer and frees the buffer
        displayvdiHeader(vdiFile);
        free(buf);

        return vdiFile;

    }


    void vdiClose(struct VDIFile *f) {

        close(f->fileDescriptor);

    }

    ssize_t vdiRead(VDIFile *f, void *buf, size_t count) {
        //std::cout << "Made it into vdiRead" << std::endl;

        off_t offset = f->cursor + f->headerSize;

        count = pread(f->fileDescriptor,buf,count,offset);
        vdiSeek(f,offset,count);

        //std::cout<< f->size << std::endl;
        //std::cout << f->fileDescriptor << std::endl;
        //std::cout << "Allocated buf" << std::endl;
        //std::cout << strerror(errno) << std::endl;
        //displayBuffer(reinterpret_cast<uint8_t *>(buf), count, 0);

        return count;
    }

    ssize_t vdiWrite(struct VDIFile *f, void *buf, size_t count) {

        vdiSeek(f,0,0);
        unsigned char* header = reinterpret_cast<unsigned char *>(static_cast<char *>(malloc(444)));
        header = f->header;

        write(f->fileDescriptor,header,444);


        return write(f->fileDescriptor,buf,count);
    }

    off_t vdiSeek(VDIFile *f, off_t offset, int anchor) {
        if(offset * anchor < 0 || offset * anchor > f->diskSize)
            return f->cursor;

        f->cursor = offset * anchor;
        return f->cursor;
    }


