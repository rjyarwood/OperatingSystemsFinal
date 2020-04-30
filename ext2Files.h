//
// Created by rjyarwood on 4/19/20.
//

#ifndef INC_5806FINAL_EXT2FILES_H
#define INC_5806FINAL_EXT2FILES_H

#include <cstdint>


const unsigned char LINUX_PARTITION = 0x83;


struct Ext2SuperBlock{
    uint32_t
        s_inodes_count,
        s_blocks_count,
        s_r_blocks_count,
        s_free_blocks_count,
        s_free_inodes_count,
        s_first_data_block,
        s_log_block_size,
        s_log_frag_size,
        s_blocks_per_group,
        s_frags_per_group,
        s_inodes_per_group,
        s_mtime,
        s_wtime;
    uint16_t
        s_mnt_count,
        s_max_mnt_count,
        s_magic,
        s_state,
        s_errors,
        s_minor_rev_level;
    uint32_t
        s_lastcheck,
        s_checkinterval,
        s_creator_os,
        s_rev_level;
    uint16_t
        s_def_resuid,
        s_def_resgid;
    uint32_t
        s_first_ino;
    uint16_t
        s_inode_size,
        s_block_group_nr;
    uint32_t
        s_feature_compat,
        s_feature_incompat,
        s_feature_ro_compat;
    struct UUID
        s_uuid;
    unsigned char
        s_volume_name[16],
        s_last_mounted[64];
    uint32_t
        s_algo_bitmap;
    unsigned char
        s_prealloc_blocks,
        s_prealloc_dir_blocks;
    uint16_t
        alignment;
    struct UUID
        s_journal_uuid;
    uint32_t
        s_journal_inum,
        s_journal_dev,
        s_last_orphan,
        s_hash_seed[4];
    unsigned char
        s_def_hash_version,
        padding[3];
    uint32_t
        s_default_mount_options,
        s_first_meta_bg;
    unsigned char
        unused[760];
};

struct Ext2BlockGroupDescriptor{
    uint32_t
        bg_block_bitmap,
        bg_inode_bitmap,
        bg_inode_table;
    uint16_t
        bg_free_blocks_count,
        bg_free_inodes_count,
        bg_used_dirs_count,
        bg_pad;
    unsigned char
        bg_reserved[12];
};

struct Ext2File{
    struct Ext2SuperBlock
            superBlock;
    unsigned int
            blockGroupCount;
    struct Ext2BlockGroupDescriptor
            *blockGroupTable;
    PartitionFile
            *partitionFile;
    int
            blockSize;
};

struct Ext2File *ext2Open(char *fn, int32_t pNum);
void ext2Close(struct Ext2File *f);

int32_t fetchBlock(struct Ext2File*, uint32_t, void *);
int32_t writeBlock(struct Ext2File*, uint32_t, void *);

int32_t fetchSuperblock(struct Ext2File*,uint32_t, struct Ext2SuperBlock*);
int32_t writeSuperblock(struct Ext2File*,uint32_t, struct Ext2SuperBlock*);

int32_t fetchBGDT(struct Ext2File *, uint32_t, struct Ext2BlockGroupDescriptor*);
int32_t writeBGDT(struct Ext2File *,uint32_t,struct Ext2BlockGroupDescriptor*);

#endif //INC_5806FINAL_EXT2FILES_H
