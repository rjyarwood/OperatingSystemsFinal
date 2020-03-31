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
        VDIFile *vdi = new VDIFile;

        vdi->fileDescriptor = open(fn, O_CREAT);
        std::cout<<vdi->fileDescriptor << std::endl;
        if(vdi->fileDescriptor < 1)
            std::cout << strerror(errno) << std::endl;
        vdi->size = lseek(vdi->fileDescriptor,0,SEEK_END);

        vdi->cursor = 0;
        unsigned char *buf = reinterpret_cast<unsigned char *>(static_cast<char *>(malloc(444)));
        pread(vdi->fileDescriptor, buf,444,0);
        lseek(vdi->fileDescriptor,0,0);
        vdi->header = buf;

        //std::cout << "back in vdiOpen" << std::endl;
        int i = -1;
        bool cont = true;
        while(cont) {
            i++;
            //std::cout << i << std::endl;
            if (isprint(buf[i])) {
                //std::cout<<"in if" << std::endl;
                vdi->imageName +=  buf[i];
            }
            else
                cont = false;
        }

        struct UUID *uuid = new struct UUID;
        struct UUID *snapuuid = new struct UUID;
        struct UUID *uuidlink = new struct UUID;
        struct UUID *parentuuid = new struct UUID;


        //displayBuffer(reinterpret_cast<uint8_t *>(header), 444, 0);

        for(int i=0; i<4; i++) {
                    vdi->imageSignature += (int)vdi->header[0x40 + i] << (8*i);
                    vdi->version += (int)vdi->header[0x44 + i] << (8*i);
                    vdi->headerSize += (int)vdi->header[0x48 +i] << (8*i);
                    vdi->imageType += (int)vdi->header[0x4c + i] << (8*i);
                    vdi->imageFlags += (int)vdi->header[0x50 + i] << (8*i);
                    vdi->offsetBlocks += (int)vdi->header[0x154 + i] << (8*i);
                    vdi->offsetData += (int)vdi->header[0x158 + i] << (8*i);
                    vdi->cylinderCount += (int)vdi->header[0x15c +i] << (8*i);
                    vdi->headCount += (int)vdi->header[0x160 + i] << (8*i);
                    vdi->sectorCount += (int)vdi->header[0x164 + i] << (8*i);
                    vdi->sectorSize += (int)vdi->header[0x168 + i] << (8*i);
                    vdi->blockSize += (int)vdi->header[0x178 + i] << (8*i);
                    vdi->extraBlockData += (int)vdi->header[0x17c + i] << (8*i);
                    vdi->blockCountinHDD += (int)vdi->header[0x180 + i] << (8*i);
                    vdi->blocksAllocated += (int)vdi->header[0x184 + i] << (8*i);

                    uuid->timeLow += (int)vdi->header[0x188 + i] << (8*i);
                    snapuuid->timeLow += (int)vdi->header[0x198 + i] << (8*i);
                    uuidlink->timeLow += (int)vdi->header[0x1a8 + i] << (8*i);
                    parentuuid->timeLow += (int)vdi->header[0x1b8 + i] << (8*i);
        }

        for(int i=0; i<8; i++){
                    vdi->diskSize += (int)vdi->header[0x170 +i] << (8*i);
        }

        for(int i = 0; i<2; i++){
                    uuid->timeMid += (int)vdi->header[0x18c +i] << (8*i);
                    uuid->timeHigh += (int)vdi->header[0x18e +i] << (8*i);
                    uuid->clock += (int)vdi->header[0x190 +i] << (8*i);

                    snapuuid->timeMid += (int)vdi->header[0x19c +i] << (8*i);
                    snapuuid->timeHigh += (int)vdi->header[0x19e +i] << (8*i);
                    snapuuid->clock += (int)vdi->header[0x1a0 +i] << (8*i);

                    uuidlink->timeMid += (int)vdi->header[0x1ac +i] << (8*i);
                    uuidlink->timeHigh += (int)vdi->header[0x1ae +i] << (8*i);
                    uuidlink->clock += (int)vdi->header[0x1b0 +i] << (8*i);

                    parentuuid->timeMid += (int)vdi->header[0x1bc +i] << (8*i);
                    parentuuid->timeHigh += (int)vdi->header[0x1abe +i] << (8*i);
                    parentuuid->clock += (int)vdi->header[0x1c0 +i] << (8*i);


        }

        for(int i = 0; i<6; i++){
                    uuid->node[i] = (int)vdi->header[0x192 + i];
                    //std::cout<< std::hex << (int)vdi->header[0x192 + i] << '\t' << std::hex << (int)uuid->node[i] << std::endl;
                    snapuuid->node[i] = (int)vdi->header[0x1a2 + i];
                    uuidlink->node[i] = (int)vdi->header[0x1b2 + i];
                    parentuuid->node[i] = (int)vdi->header[0x1c2 + i];

        }

        //std::cout << std::hex << (int)uuid->timeLow << "-" <<std::hex << (int)uuid->timeMid << "-"<< std::hex <<uuid->timeHigh << "-" << std::hex << uuid->clock << std::hex << uuid->node << std::endl;
        //printf("%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x\n",
        //        uuid->timeLow,uuid->timeMid,uuid->timeHigh,uuid->clock,
        //        uuid->node[0],uuid->node[1],uuid->node[2],uuid->node[3],
        //        uuid->node[4],uuid->node[5]);

        vdi->UUID = uuid2ascii(uuid);
        vdi->snapUUID = uuid2ascii(snapuuid);
        vdi->UUIDLink = uuid2ascii(uuidlink);
        vdi->parentUUID = uuid2ascii(parentuuid);

        //std::cout<< "\n0x" << std::hex<< (int)header[4*16 + 3] << (int)header[4*16+2] << (int)header[4*16+1]<< (int)header[4*16]<< std::endl;
        //std::cout << "0x" << std::hex << imageSignature << std::endl;

        displayvdiHeader(vdi);
        free(buf);

        return vdi;
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


