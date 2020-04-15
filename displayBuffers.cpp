//
// Created by rj on 2/14/20.
//

#include <cstdint>
#include <iostream>
#include <iomanip>
#include "VDIFile.h"
#include "PartitionEntry.h"

void displayCHS(CHS chs);

void displayBufferPage(uint8_t *buf, uint32_t count, uint32_t start, uint64_t offset){
    std::cout << "Offset: 0x" << std::hex << offset << std::endl;
    std::cout << "    00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f     0...4...8...c..." << std::endl;
    std::cout << "+..................................................+    +..............+" << std::endl;
    for(int y = 0; y < 16; y++) {
        std::cout <<std::hex << y << "0| ";
        if (start / 16 > y) {
            std::cout << "                                         " << std::endl;
            continue;
        }
        for (int x = 0; x < 16; x++) {
            if (y * 16 + x < start || y * 16 + x >= start + count) {
                std::cout << "   ";
                continue;
            }
//            if((int)buf[offset+(y*16) + x] < 16)
//                std::cout<<" ";
            std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) buf[offset + (y * 16) + x] << " ";


        }
        std::cout << "|" << std::hex << y << "0|";
        for (int x = 0; x < 16; x++) {
            if (y * 16 + x < start || y * 16 + x >= start + count) {
                std::cout << "   ";
                if(x==15)
                    std::cout<<"\n";
                continue;
            }
            if (isprint(buf[offset + (y * 16 + x)])) {
                putchar(buf[offset + (y * 16 + x)]);
            }
            else
                std::cout << "  ";
            if(x==15)
                std::cout<<"\n";

        }
    }

    std::cout << "+..................................................+    +..............+" << std::endl;
    std::cout << std::endl;
}

void displayBuffer(uint8_t *buf,int32_t count,uint64_t offset){

    while (count > 0){
        displayBufferPage(buf, count,0,offset);
        offset += 0x100;
        count -= 256;
    }



}

void displayvdiHeader(struct VDIFile*f){
    std::cout << "Image Name: " << f->VDIHeader->fileInfo << std::endl;
    std::cout << "Signature: 0x" << std::hex << f->VDIHeader->imageSignature << std::endl;
    std::cout << "Version: " << f->VDIHeader->versionMajor << std::endl;
    std::cout << "Header Size: 0x" << std::hex << f->VDIHeader->postHeaderSize << std::endl;
    std::cout << "Image type: 0x" << std::hex << f->VDIHeader->imageType << std::endl;
    std::cout << "Flags: 0x" << std::hex << f->VDIHeader->imageFlags << std::endl;
    std::cout << "Frame offset: 0x" << std::hex << f->offsetBlocks << std::endl;
    std::cout << "Offset Data: 0x" << std::hex << f->offsetData << std::endl;
    std::cout << "Cylinder Count 0x" << std::hex << f->VDIHeader->diskGeometry.cylinderCount << std::endl;
    std::cout << "Head Count 0x" << std::hex << f->VDIHeader->diskGeometry.headCount << std::endl;
    std::cout << "Sector count: 0x" << std::hex << f->VDIHeader->diskGeometry.sectorCount << std::endl;
    std::cout << "Sector Size: 0x" << std::hex << f->VDIHeader->diskGeometry.sectorSize << std::endl;
    std::cout << "Disk Size: 0x" << std::hex << f->fileSize << std::endl;
    std::cout << "Frame Size: 0x" << std::hex << f->VDIHeader->pageSize << std::endl;
    std::cout << "Extra Frame Size: 0x" << std::hex << f->VDIHeader->extraPageSize << std::endl;
    std::cout << "Frames in HDD: 0x" << std::hex << f->VDIHeader.nPagesTotal << std::endl;
    std::cout << "Frames allocated: 0x" << std::hex << f->VDIHeader.nPagesAllocated << std::endl;
    std::cout << "UUID: ";

   /* for(int i=0;i<32;i++) {
     std::cout << f->UUID[i];
    }
    std::cout << "\nUUID of last SNAP: " << f->snapUUID << std::endl;
    std::cout << "Link UUID : " << f->UUIDLink << std::endl;
    std::cout << "Parent UUID: " << f->parentUUID << std::endl;
    */
    std::cout << "Image Description: " << f->fileDescriptor << std::endl;
}

void displayPartitionEntry(struct PartitionEntry p){
    std::cout << "Status: "  << p.status << std::endl;
    std::cout << "CHS of First Sector: 0x";
    displayCHS(p.CHSofFirstSect);
    std::cout << "Partition Type: " << p.partitionType << std::endl;
    std::cout << "CHS of Last Sector: 0x";
    displayCHS(p.CHSofLastSect);
    std::cout << "LBA of First Sector: 0x" << std::hex << p.LBAofFirstSect << std::endl;
    std::cout << "LBA Sector Count: 0x" << std::hex << p.LBASectorCount << std::endl;

}

void displayCHS(CHS c){
    std::cout <<  std::hex << c.header << c.sectorbits << c.cylinderbits << std::endl;

}

