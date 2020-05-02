
#include <fcntl.h>
#include <zconf.h>
#include <cstring>
#include <string>
#include <vector>
#include "ext2Files.h"
#include "Dirent.h"
#include "FileAccess.h"

const char* findFileName(char* path){
    // Converting to string to make it easierto find filename at end
    std::string tempString(path);


    int stringLen = tempString.size();

    int i = 0;

    do{
        i++;
    }while(tempString[stringLen - i] != '/');

    std::string tempReturnString[i];

    int m = 0;
    for(int j = stringLen - i; j < stringLen; j++){
        tempReturnString[m] = tempString[j];
        m++;
    }

    return tempReturnString->c_str();

}

int main(int argc, char *argv[]) {

    char* vdiName = argv[0];
    char* localName = argv[1];
    char* fileToCopyPath = argv[2];

    const char* fileName = findFileName(fileToCopyPath);



    int localfd = open(localName, O_WRONLY | O_CREAT, 0666);

    auto ext2File = ext2Open(vdiName, -1);

    int iNum = traversePath(ext2File, fileToCopyPath);;

    auto directory = openDir(ext2File, iNum);

    char* name = nullptr;

    while(getNextDirent(ext2File, directory, reinterpret_cast<uint32_t &>(iNum), name)){
        if(strcmp(fileName, name) == 0)
            break;
    }

    auto inode = new Inode;
    fetchInode(ext2File, iNum, inode);

    char* temp = new char[512];

    int bNum = 0;
    lseek(localfd, 0, SEEK_SET);

    while(fetchBlockFromFile(ext2File, inode, bNum, temp)){

        write(localfd, temp, sizeof(temp));

        bNum++;
        lseek(localfd, 512 * bNum, SEEK_SET);
    }

    return 0;
}


