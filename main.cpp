
#include <fcntl.h>
#include <zconf.h>
#include <cstring>
#include "ext2Files.h"
#include "Dirent.h"
#include "FileAccess.h"

int main(int argc, char *argv[]) {

    char localName[] = "   ";
    char vdiName[] = "   ";
    char fileToCopyPath[] = "   ";

    int localfd = open(localName, O_WRONLY | O_CREAT, 0666);

    auto ext2File = ext2Open(vdiName, -1);

    int iNum = traversePath(ext2File, fileToCopyPath);;

    auto directory = openDir(ext2File, iNum);

    char* name = nullptr;

    while(getNextDirent(ext2File, directory, reinterpret_cast<uint32_t &>(iNum), name)){
        if(strcmp(fileToCopyPath, name) == 0)
            break;
    }

    auto inode = new Inode;
    fetchInode(ext2File, iNum, inode);

    char* temp = new char[512];

    int bNum = 0;
    lseek(localfd, 0, SEEK_SET);

    while(fetchBlockFromFile(ext2File, inode, bNum, temp)){



        bNum++;
    }

    return 0;
}


