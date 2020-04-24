#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include "VDIFile.h"
#include "displayBuffers.cpp.h"
#include "PartitionFile.h"
#include "PartitionEntry.h"
#include "ext2Files.h"
#include "Inode.h"






int main(int argc, char *argv[]) {
    /**
    char *buf = nullptr;
    FILE *fp = fopen("/home/rj/Documents/OS/Test-fixed-1k.vdi", "r");
    if (fp != nullptr) {
        /* Go to the end of the file. */
    /**
    std::cout<<1<<std::endl;
    if (fseek(fp, 0L, SEEK_END) == 0) {
        /* Get the size of the file. */

    //long bufsize = ftell(fp);
    //if (bufsize == -1) { /* Error */ }

    /* Allocate our buffer to that size. */
    // buf = static_cast<char *>(malloc(sizeof(char) * (bufsize + 1)));

    /* Go back to the start of the file. */
    //if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

    /* Read the entire file into memory. */
    //size_t newLen = fread(buf, sizeof(char), bufsize, fp);
    //if ( ferror( fp ) != 0 ) {
    //    fputs("Error reading file", stderr);
    //} else {
    //    buf[newLen++] = '\0'; /* Just to be safe. */
    //}
    //}
    //fclose(fp);
    const char* fileName = "/home/rj/Documents/OS/Test-fixed-1k.vdi";
    const char* newName = "/home/rj/Documents/OS/test.vdi";
    int countToRead = 0x540;
    //VDIFile *newvdi = new VDIFile;

    //std::cout << "Created obj" << std::endl;
    //vdiOpen(const_cast<char *>(newName));
    struct VDIFile *vdi = vdiOpen(const_cast<char *>(fileName));
    //std::cout << "Opened File" << std::endl;
    char *buf = static_cast<char *>(malloc(vdi->size));
    //displayBuffer(reinterpret_cast<uint8_t *>(buf), 265, 0);
    //std::cout<< "Created buffer" << std::endl;
    ssize_t count = vdiRead(vdi, buf, countToRead);
    //std::cout << "Read File" << std::endl;
    displayBuffer(reinterpret_cast<uint8_t *>(buf), countToRead, 0);
    //vdiWrite(newvdi,buf,count);
    vdiClose(vdi);
    //vdiClose(newvdi);

    char buffer[64];
    //struct VDIFile *f = vdiOpen(const_cast<char *>(fileName));
    //vdiSeek(f,446,SEEK_SET);
    //vdiRead(f,buffer,64);
    //displayBufferPage(reinterpret_cast<uint8_t *>(buffer), 64, 190, 256);

    free(buf);

    return 0;
}


