//
// Created by rjyarwood on 4/24/20.
//

#ifndef INC_5806FINAL_INODE_H
#define INC_5806FINAL_INODE_H

#include <stdint-gcc.h>
#include "ext2Files.h"

struct Inode{
    uint16_t
        i_mode,
        i_uid;
    uint32_t
        i_size,
        i_atime,
        i_ctime,
        i_mtime,
        i_dtime;
    int8_t
        i_gid,
        i_links_count;
    uint32_t
        i_blocks,
        i_flags,
        i_osd1,
        i_block[15],
        i_generation,
        i_file_acl,
        i_dir_acl,
        i_faddr;
    unsigned char
        i_osd2[12];
};

int32_t fetchInode(struct Ext2File, uint32_t, struct Inode);
int32_t writeInode(struct Ext2File, uint32_t, struct Inode);

#endif //INC_5806FINAL_INODE_H
