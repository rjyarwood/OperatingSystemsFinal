//
// Created by rj on 2/12/20.
//


/*
 * CHANGES TO BE MADE
 *
 */


#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include "uuid2ascii.cpp"
#include "VDIFile.h"


    struct VDIFile *vdiOpen(char *fn) {
        auto *vdiFile = new VDIFile;


        //Opening the VDIFile and saving file descriptor
        vdiFile->fileDescriptor = open(fn, O_RDWR);
        //std::cout<<vdiFile->fileDescriptor << std::endl;

        // If file does not exist
        if(vdiFile->fileDescriptor < 1)
            std::cout << strerror(errno) << std::endl;

        //Finding size of file using lseek
        vdiFile->fileSize = lseek(vdiFile->fileDescriptor,0,SEEK_END);

        //Setting cursor to 0
        vdiFile->cursor = 0;

        read(vdiFile->fileDescriptor, &vdiFile->vdiHeader,sizeof(VDIHeader));

        vdiFile->map = new unsigned int[vdiFile->vdiHeader.nPagesTotal];

        //sends to display buffer and frees the buffer
        lseek(vdiFile->fileDescriptor,vdiFile->vdiHeader.mapOffset,SEEK_SET);
        read(vdiFile->fileDescriptor,vdiFile->map,vdiFile->vdiHeader.nPagesTotal * sizeof(int));

        return vdiFile;

    }

    void vdiClose(struct VDIFile *f) {

        close(f->fileDescriptor);

        delete[] f->map;

    }

    ssize_t vdiRead(VDIFile *f, void *buf, size_t count) {

        uint64_t
            bytesToRead,
            bytesRead = 0;
        uint32_t
            pageNumber,
            offset;


        if(f->cursor + count > f->vdiHeader.diskSize){
            count = f->vdiHeader.diskSize - f->cursor;
        }

        while(count > 0){

            bytesToRead = f->vdiHeader.pageSize - (f->cursor % f->vdiHeader.pageSize);

            if(bytesToRead > count){
                bytesToRead = count;
            }

            //Translate Page
            pageNumber = f->cursor / f->vdiHeader.pageSize;
            offset = f->cursor % f->vdiHeader.pageSize;

            pageNumber = f->map[pageNumber];

            if(pageNumber < 0){
                memset((char*)buf + bytesRead, 0, bytesRead);
                bytesRead += bytesToRead;
            }
            else{
                lseek(f->fileDescriptor, f->vdiHeader.dataOffset + (pageNumber * f->vdiHeader.pageSize) + offset, SEEK_SET);
                read(f->fileDescriptor, (char*)buf + bytesRead, bytesToRead);
                bytesRead += bytesToRead;
            }

            count -= bytesToRead;
        }

        return bytesRead;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored (OC Unused Global Declaration Inspection)
    ssize_t vdiWrite(struct VDIFile *f, void *buf, size_t count) {

        uint64_t
                bytesToWrite,
                bytesWritten = 0;
        uint32_t
                pageNumber,
                offset,
                n;
        char*
                tmp;

        if(f->cursor + count > f->vdiHeader.diskSize){
            count = f->vdiHeader.diskSize - f->cursor;
        }

        while(count > 0){

            bytesToWrite = f->vdiHeader.pageSize - (f->cursor % f->vdiHeader.pageSize);

            if(bytesToWrite > count){
                bytesToWrite = count;
            }

            //Translate Page
            pageNumber = f->cursor / f->vdiHeader.pageSize;
            offset = f->cursor % f->vdiHeader.pageSize;

            pageNumber = f->map[pageNumber];

            if(pageNumber < 0){

                tmp = new char[f->vdiHeader.pageSize];
                if(tmp == nullptr){
                    break;
                }

                memset(tmp, 0, f->vdiHeader.pageSize);
                lseek(f->fileDescriptor, 0, SEEK_SET);
                n = write(f->fileDescriptor,tmp, f->vdiHeader.pageSize);

                delete[] tmp;

                f->map[pageNumber] = f->vdiHeader.nPagesAllocated;

                pageNumber = f->vdiHeader.nPagesAllocated;

                f->vdiHeader.nPagesAllocated++;

                lseek(f->fileDescriptor,0,SEEK_SET);
                write(f->fileDescriptor, &f->vdiHeader, sizeof(f->vdiHeader));

                lseek(f->fileDescriptor,f->vdiHeader.mapOffset,SEEK_SET);
                write(f->fileDescriptor,f->map,f->vdiHeader.nPagesTotal * sizeof(int));

            }

            lseek(f->fileDescriptor, f->vdiHeader.dataOffset + (pageNumber * f->vdiHeader.pageSize) + offset, SEEK_SET);
            write(f->fileDescriptor, (char*)buf + bytesWritten, bytesToWrite);
            bytesWritten += bytesToWrite;

            /*
            //Write out VDIHeader & map back to file
            lseek(f->fileDescriptor,0,0);
            write(f->fileDescriptor,&f->vdiHeader, sizeof(f->vdiHeader));

            lseek(f->fileDescriptor,f->vdiHeader.mapOffset,0);
            write(f->fileDescriptor,f->map, sizeof(f->map));
            */

            count -= bytesToWrite;
        }

        return bytesWritten;
    }

    off_t vdiSeek(VDIFile *f, off_t offset, int anchor) {
        size_t newLocation;

        if(anchor == SEEK_SET){
            newLocation = offset;
        }
        else if(anchor == SEEK_CUR){
            newLocation = f->cursor + offset;
        }
        else{
            newLocation = f->vdiHeader.diskSize - offset;
        }

        if(newLocation >= 0 && newLocation < f->vdiHeader.diskSize){
            f->cursor = newLocation;
        }


        return f->cursor;
    }

#pragma clang diagnostic pop

