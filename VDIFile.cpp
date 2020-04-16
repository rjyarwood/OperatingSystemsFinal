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

        read(vdiFile->fileDescriptor, &vdiFile->VDIHeader,sizeof(VDIHeader));

        vdiFile->map = new unsigned int[vdiFile->VDIHeader.nPagesTotal];

        //sends to display buffer and frees the buffer
        lseek(vdiFile->fileDescriptor,vdiFile->VDIHeader.mapOffset,SEEK_SET);
        read(vdiFile->fileDescriptor,vdiFile->map,vdiFile->VDIHeader.nPagesTotal * sizeof(int));

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


        if(f->cursor + count > f->VDIHeader.diskSize){
            count = f->VDIHeader.diskSize - f->cursor;
        }

        while(count > 0){

            bytesToRead = f->VDIHeader.pageSize - (f->cursor % f->VDIHeader.pageSize);

            if(bytesToRead > count){
                bytesToRead = count;
            }

            //Translate Page
            pageNumber = f->cursor / f->VDIHeader.pageSize;
            offset = f->cursor % f->VDIHeader.pageSize;

            pageNumber = f->map[pageNumber];

            if(pageNumber < 0){
                memset((char*)buf + bytesRead, 0, bytesRead);
                bytesRead += bytesToRead;
            }
            else{
                lseek(f->fileDescriptor, f->VDIHeader.dataOffset + (pageNumber * f->VDIHeader.pageSize) + offset, SEEK_SET);
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

        if(f->cursor + count > f->VDIHeader.diskSize){
            count = f->VDIHeader.diskSize - f->cursor;
        }

        while(count > 0){

            bytesToWrite = f->VDIHeader.pageSize - (f->cursor % f->VDIHeader.pageSize);

            if(bytesToWrite > count){
                bytesToWrite = count;
            }

            //Translate Page
            pageNumber = f->cursor / f->VDIHeader.pageSize;
            offset = f->cursor % f->VDIHeader.pageSize;

            pageNumber = f->map[pageNumber];

            if(pageNumber < 0){

                tmp = new char[f->VDIHeader.pageSize];
                if(tmp == nullptr){
                    break;
                }

                memset(tmp, 0, f->VDIHeader.pageSize);
                lseek(f->fileDescriptor, 0, SEEK_SET);
                n = write(f->fileDescriptor,tmp,f->VDIHeader);

                delete[] tmp;

                f->map[pageNumber] = f->VDIHeader.nPagesAllocated;

                pageNumber = f->VDIHeader.nPagesAllocated;

                f->VDIHeader.nPagesAllocated++;

                lseek(f->fileDescriptor,0,SEEK_SET);
                write(f->fileDescriptor, &f->VDIHeader, sizeof(f->VDIHeader));

                lseek(f->fileDescriptor,f->VDIHeader.mapOffset,SEEK_SET);
                write(f->fileDescriptor,f->map,f->VDIHeader.nPagesTotal * sizeof(int));

            }

            lseek(f->fileDescriptor, f->VDIHeader.dataOffset + (pageNumber * f->VDIHeader.pageSize) + offset, SEEK_SET);
            write(f->fileDescriptor, (char*)buf + bytesWritten, bytesToWrite);
            bytesWritten += bytesToWrite;

            //Write out VDIHeader & map back to file


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
            newLocation = f->VDIHeader.diskSize - offset;
        }

        if(newLocation >= 0 && newLocation < f->VDIHeader.diskSize){
            f->cursor = newLocation;
        }


        return f->cursor;
    }

#pragma clang diagnostic pop

