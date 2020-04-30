//
// Created by rj on 2/14/20.
//

#include "displayBuffers.h"
//#include "uuid2ascii.cpp"


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

void displayvdiHeader(struct VDIFile* f){
    std::cout << "Image Name: " << f->vdiHeader.fileInfo << std::endl;
    std::cout << "Signature: 0x" << std::hex << f->vdiHeader.magic << std::endl;
    std::cout << "Version: " << f->vdiHeader.versionMajor << "." << f->vdiHeader.versionMinor << std::endl;
    std::cout << "Header Size: 0x" << std::hex << f->vdiHeader.dataOffset << std::endl;
    std::cout << "Image type: 0x" << std::hex << f->vdiHeader.imageType << std::endl;
    std::cout << "Flags: 0x" << std::hex << f->vdiHeader.imageFlags << std::endl;
    std::cout << "Frame offset: 0x" << std::hex << f->vdiHeader.mapOffset << std::endl;
    std::cout << "Offset Data: 0x" << std::hex << f->map << std::endl;
    std::cout << "Cylinder Count 0x" << std::hex << f->vdiHeader.diskGeometry.cylinderCount << std::endl;
    std::cout << "Head Count 0x" << std::hex << f->vdiHeader.diskGeometry.headCount << std::endl;
    std::cout << "Sector count: 0x" << std::hex << f->vdiHeader.diskGeometry.sectorCount << std::endl;
    std::cout << "Sector Size: 0x" << std::hex << f->vdiHeader.diskGeometry.sectorSize << std::endl;
    std::cout << "Disk Size: 0x" << std::hex << f->fileSize << std::endl;
    std::cout << "Frame Size: 0x" << std::hex << f->vdiHeader.pageSize << std::endl;
    std::cout << "Extra Frame Size: 0x" << std::hex << f->vdiHeader.extraPageSize << std::endl;
    std::cout << "Frames in HDD: 0x" << std::hex << f->vdiHeader.nPagesTotal << std::endl;
    std::cout << "Frames allocated: 0x" << std::hex << f->vdiHeader.nPagesAllocated << std::endl;
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

void displayPartitionEntry(struct PartitionEntry *p){
    std::cout << "Status: "  << p->status << std::endl;
    std::cout << "CHS of First Sector: 0x";
    displayCHS(p->CHSofFirstSect);
    std::cout << "Partition Type: " << p->partitionType << std::endl;
    std::cout << "CHS of Last Sector: 0x";
    displayCHS(p->CHSofLastSect);
    std::cout << "LBA of First Sector: 0x" << std::hex << p->LBAofFirstSect << std::endl;
    std::cout << "LBA Sector Count: 0x" << std::hex << p->LBASectorCount << std::endl;

}

void displayCHS(CHS c){
    std::cout <<  std::hex << c.header << c.sectorbits << c.cylinderbits << std::endl;

}

void displaySuperblockContent(struct Ext2File *e) {
    std::cout << "Superblock contents:" << std::endl;
    std::cout << "Number of inodes: " << e->superBlock.s_inodes_count << std::endl;
    std::cout << "Number of blocks: " << e->superBlock.s_blocks_count << std::endl;
    std::cout << "Number of reserved blocks: " << e->superBlock.s_r_blocks_count << std::endl;
    std::cout << "Number of free blocks: " << e->superBlock.s_free_blocks_count << std::endl;
    std::cout << "Number of free inodes: " << e->superBlock.s_free_inodes_count << std::endl;
    std::cout << "First data block: " << e->superBlock.s_first_data_block << std::endl;
    std::cout << "Log block size: " << e->superBlock.s_log_block_size << std::endl;
    std::cout << "Log fragment size: " << e->superBlock.s_log_frag_size << std::endl;
    std::cout << "Blocks per group: " << e->superBlock.s_blocks_per_group << std::endl;
    std::cout << "Fragments per group: " << e->superBlock.s_frags_per_group << std::endl;
    std::cout << "Inodes per group: " << e->superBlock.s_inodes_per_group << std::endl;
    std::cout << "Last mount time: " << e->superBlock.s_mtime << std::endl;
    std::cout << "Last write time: " << e->superBlock.s_wtime << std::endl;
    std::cout << "Mount count: " << e->superBlock.s_mnt_count << std::endl;
    std::cout << "Max mount count: " << e->superBlock.s_max_mnt_count << std::endl;
    std::cout << "Magic number: " << e->superBlock.s_magic << std::endl;
    std::cout << "State: " << e->superBlock.s_state << std::endl;
    std::cout << "Error processing: " << e->superBlock.s_errors << std::endl;
    std::cout << "Revision level: " << e->superBlock.s_minor_rev_level << std::endl;
    std::cout << "Last system check: " << e->superBlock.s_lastcheck << std::endl;
    std::cout << "Check interval: " << e->superBlock.s_checkinterval << std::endl;
    std::cout << "OS creator: " << e->superBlock.s_creator_os << std::endl;
    std::cout << "Default reserve UID: " << e->superBlock.s_def_resuid << std::endl;
    std::cout << "Default reserve GID: " << e->superBlock.s_def_resgid << std::endl;
    std::cout << "First inode number: " << e->superBlock.s_first_ino << std::endl;
    std::cout << "Inode size: " << e->superBlock.s_inode_size << std::endl;
    std::cout << "Block group number: " << e->superBlock.s_block_group_nr << std::endl;
    std::cout << "Feature compatibility bits: " << e->superBlock.s_feature_compat << std::endl;
    std::cout << "Feature incompatibility bits: " << e->superBlock.s_feature_incompat << std::endl;
    std::cout << "Feature read/only compatibility bits: " << e->superBlock.s_feature_ro_compat << std::endl;
    //std::cout << "UUID: " << e->superBlock.s_uuid << std::endl;
    // WHAT TO DO HERE?
    std::cout << "Volume name: " << e->superBlock.s_volume_name << std::endl;
    std::cout << "Last mount point: " << e->superBlock.s_last_mounted << std::endl;
    std::cout << "Algorithm bitmap: " << e->superBlock.s_algo_bitmap << std::endl;
    std::cout << "Number of blocks to preallocate: " << e->superBlock.s_prealloc_blocks << std::endl;
    std::cout << "Number of blocks to preallocate for directories: " << e->superBlock.s_prealloc_dir_blocks << std::endl;
    //std::cout << "Journal UUID: " << e->superBlock.s_journal_uuid << std::endl;
    // WHAT TO DO HERE?
    std::cout << "Journal inode number: " << e->superBlock.s_journal_inum << std::endl;
    std::cout << "Journal device number: " << e->superBlock.s_journal_dev << std::endl;
    std::cout << "Journal last orphan inode number: " << e->superBlock.s_last_orphan << std::endl;
    std::cout << "Default hash version: " << e->superBlock.s_def_hash_version << std::endl;
    std::cout << "Default mount option bitmap: " << e->superBlock.s_default_mount_options << std::endl;
    std::cout << "First meta block group: " << e->superBlock.s_first_meta_bg << std::endl;

}

void displaySuperblockContent(struct Ext2SuperBlock *e) {
    std::cout << "Superblock contents:" << std::endl;
    std::cout << "Number of inodes: " << e->s_inodes_count << std::endl;
    std::cout << "Number of blocks: " << e->s_blocks_count << std::endl;
    std::cout << "Number of reserved blocks: " << e->s_r_blocks_count << std::endl;
    std::cout << "Number of free blocks: " << e->s_free_blocks_count << std::endl;
    std::cout << "Number of free inodes: " << e->s_free_inodes_count << std::endl;
    std::cout << "First data block: " << e->s_first_data_block << std::endl;
    std::cout << "Log block size: " << e->s_log_block_size << std::endl;
    std::cout << "Log fragment size: " << e->s_log_frag_size << std::endl;
    std::cout << "Blocks per group: " << e->s_blocks_per_group << std::endl;
    std::cout << "Fragments per group: " << e->s_frags_per_group << std::endl;
    std::cout << "Inodes per group: " << e->s_inodes_per_group << std::endl;
    std::cout << "Last mount time: " << e->s_mtime << std::endl;
    std::cout << "Last write time: " << e->s_wtime << std::endl;
    std::cout << "Mount count: " << e->s_mnt_count << std::endl;
    std::cout << "Max mount count: " << e->s_max_mnt_count << std::endl;
    std::cout << "Magic number: " << e->s_magic << std::endl;
    std::cout << "State: " << e->s_state << std::endl;
    std::cout << "Error processing: " << e->s_errors << std::endl;
    std::cout << "Revision level: " << e->s_minor_rev_level << std::endl;
    std::cout << "Last system check: " << e->s_lastcheck << std::endl;
    std::cout << "Check interval: " << e->s_checkinterval << std::endl;
    std::cout << "OS creator: " << e->s_creator_os << std::endl;
    std::cout << "Default reserve UID: " << e->s_def_resuid << std::endl;
    std::cout << "Default reserve GID: " << e->s_def_resgid << std::endl;
    std::cout << "First inode number: " << e->s_first_ino << std::endl;
    std::cout << "Inode size: " << e->s_inode_size << std::endl;
    std::cout << "Block group number: " << e->s_block_group_nr << std::endl;
    std::cout << "Feature compatibility bits: " << e->s_feature_compat << std::endl;
    std::cout << "Feature incompatibility bits: " << e->s_feature_incompat << std::endl;
    std::cout << "Feature read/only compatibility bits: " << e->s_feature_ro_compat << std::endl;
    //std::cout << "UUID: " << e->s_uuid << std::endl;
    // WHAT TO DO HERE?
    std::cout << "Volume name: " << e->s_volume_name << std::endl;
    std::cout << "Last mount point: " << e->s_last_mounted << std::endl;
    std::cout << "Algorithm bitmap: " << e->s_algo_bitmap << std::endl;
    std::cout << "Number of blocks to preallocate: " << e->s_prealloc_blocks << std::endl;
    std::cout << "Number of blocks to preallocate for directories: " << e->s_prealloc_dir_blocks << std::endl;
    //std::cout << "Journal UUID: " << e->superBlock.s_journal_uuid << std::endl;
    // WHAT TO DO HERE?
    std::cout << "Journal inode number: " << e->s_journal_inum << std::endl;
    std::cout << "Journal device number: " << e->s_journal_dev << std::endl;
    std::cout << "Journal last orphan inode number: " << e->s_last_orphan << std::endl;
    std::cout << "Default hash version: " << e->s_def_hash_version << std::endl;
    std::cout << "Default mount option bitmap: " << e->s_default_mount_options << std::endl;
    std::cout << "First meta block group: " << e->s_first_meta_bg << std::endl;

}

void displayBlockGroupDescriptorTable(struct Ext2BlockGroupDescriptor *e) {
    std::cout << "Block group descriptor table: " << std::endl;
    std::cout << "Block   Block   Inode   Inode   Free    Free    Used" << std::endl;
    std::cout << "Number  Bitmap  Bitmap  Table   Blocks  Inodes  Dirs" << std::endl;
    std::cout << "------  ------  ------  -----   ------  ------  ----" << std::endl;

    for (int i = 0; i < 16; i++) {
        std::cout << i << std::setw(4) << e->bg_block_bitmap << std::setw(4) << e->bg_inode_bitmap
                  << std::setw(4) << e->bg_inode_table << std::setw(4) << e->bg_free_blocks_count
                  << std::setw(4) << e->bg_free_inodes_count << std::setw(4) << e->bg_used_dirs_count << std::endl;
    }

}

void displayInode(struct Inode *i) {
    std::cout << "Mode: " << i->i_mode << std::endl;
    std::cout << "Size: " << i->i_size << std::endl;
    std::cout << "Blocks: " << i->i_blocks << std::endl;
    std::cout << "UID / GID: " << i->i_uid << " / " << i->i_gid << std::endl;
    std::cout << "Links: " << i->i_links_count << std::endl;
    std::cout << "Created: " << i->i_ctime << std::endl;
    std::cout << "Last access: " << i->i_atime << std::endl;
    std::cout << "Last modification: " << i->i_mtime << std::endl;
    std::cout << "Deleted: " << i->i_dtime << std::endl;
    std::cout << "Flags: " << i->i_flags << std::endl;
    std::cout << "File version: " << i->i_generation << std::endl; // IS THIS RIGHT?
    std::cout << "ACL block: " << i->i_file_acl << std::endl;
    std::cout << "Direct blocks: " << std::endl;
    std::cout << "0-3: " << i->i_block[0] << std::setw(3) << i->i_block[1] << std::setw(3)
              << i->i_block[2] << std::setw(3) << i->i_block[3] << std::endl;
    std::cout << "4-7: " << i->i_block[4] << std::setw(3) << i->i_block[5] << std::setw(3)
              << i->i_block[6] << std::setw(3) << i->i_block[7] << std::endl;
    std::cout << "8-11: " << i->i_block[8] << std::setw(3) << i->i_block[9] << std::setw(3)
              << i->i_block[10] << std::setw(3) << i->i_block[11] << std::endl;
    std::cout << "Single indirect block: " << "0" << std::endl; //Where is this?
    std::cout << "Double indirect block: " << "0" << std::endl; //And this?
    std::cout << "Triple indirect block: " << "0" << std::endl; //And this?

}
