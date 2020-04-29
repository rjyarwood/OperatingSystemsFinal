//
// Created by rjyarwood on 4/29/20.
//

#ifndef INC_5806FINAL_FILEACCESS_H
#define INC_5806FINAL_FILEACCESS_H

#include <cstdint>
#include "Inode.h"

int32_t fetchBlockFromFile(struct Inode *, uint32_t,void *);
int32_t writeBlockToFile(struct Inode *, uint32_t,void *);

#endif //INC_5806FINAL_FILEACCESS_H
