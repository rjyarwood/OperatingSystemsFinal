//
// Created by ryarw on 4/30/2020.
//

#include <cstring>
#include "Dirent.h"
#include "FileAccess.h"

struct Directory *openDir(Ext2File *f, uint32_t iNum){

    auto *directory = new Directory;

    directory->cursor = 0;
    directory->block = new uint8_t[f->blockSize];
    directory->inodeNum = iNum;
    fetchInode(f, directory->inodeNum, reinterpret_cast<Inode *>(&(directory->inode)));

    return directory;
}

bool getNextDirent(struct Ext2File *f, struct Directory *d, uint32_t &iNum, char *name){
    Dirent *dirent;

    // This was in Discord and highlighted by Dr. Kramer
    while(d->cursor < d->inode->i_size){
        // Doing math to get block and offset within block Dirent is
        unsigned int blockNum = d->cursor / f->blockSize;
        unsigned int offset = d->cursor % f->blockSize;

        // We know we can do this because no Dirent spans 2 blocks
        fetchBlockFromFile(f, reinterpret_cast<Inode *>(&(d->inode)), blockNum, d->block);

        // Copy the block from the offset on into the dirent
        dirent = (Dirent *)(d->block + offset);
        //move cursor so the same file is not read again
        d->cursor += dirent->recLen;

        // There is something in the Dirent
        if(dirent->iNum != 0){
            // Sending iNum back
            iNum = dirent->iNum;

            // Copy name of Dirent to pointer name to be 'returned'
            for(int i = 0; i < dirent->nameLen; i++){
                name[i] = dirent->name[i];
            }
            //set end of name to 0
            name[dirent->nameLen] = 0;
            return true;
        }

    }

    return false;
}

void rewindDir(struct Directory *d){
    d->cursor = 0;
}

void closeDir(struct Directory *d){
    delete d;
}

uint32_t searchDir(struct Ext2File *f, uint32_t iNum, char *target){

    uint32_t tempiNum;
    char *tempName = nullptr;
    Directory *directory = openDir(f,iNum);

    while(getNextDirent(f,directory,tempiNum, tempName)){
        if(strcmp(tempName, target) == 0){
            closeDir(directory);
            return tempiNum;
        }
    }

    closeDir(directory);
    return 0;
}

uint32_t traversePath(Ext2File *f, char *path){
    int start = 1;
    int len = strlen(path);
    int iNum = 2;

    while(start<len && iNum != 0){
        int end = start +1;
        while(path[end] != 0 && path[end] != '/'){end++;}
        path[end] = 0;
        iNum = searchDir(f,iNum, path + start);
        start = end + 1;
    }

    return iNum;
}