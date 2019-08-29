/**
* Finding Filesystems
* CS 241 - Spring 2017
*/
#include "format.h"
#include "fs.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

void cat_RC(file_system *fs, inode* I, size_t RemainningSize);
void ls_help(file_system *fs, inode* I, size_t Datas);


void fs_ls(file_system *fs, char *path) {
    // Arrrrrgh Matey
    inode* I = get_inode(fs,path);
    if(!I)
    {
    	print_no_file_or_directory();
    	return;
    }
    if(is_file(I))
    {
    	print_file(path);
    	return;
    }
    size_t Datas = (I->size/256);
    if(is_directory(I))
    {
    //read file name
   // size_t Datas = (I->size/256);

    for(size_t h = 0; h < NUM_DIRECT_INODES; h++)
    {
    	if(I->direct[h] == -1)
    		break;
    	data_block* BLOCK = &fs->data_root[I->direct[h]];

    for(size_t i = 0; i < min(sizeof(data_block)/256,Datas); i++)
    	{
    		//char buf[248];
    		//char node[8];
    		dirent* ptr = calloc(1,sizeof(dirent));
    		/*
    		for(size_t j = 0; j < 248; j++)
    		{
   				
    			buf[j] = BLOCK[j+i*256]];
       		}
    		for(size_t j = 248; j < 256; j++)
    		{
    			
    			node[j] = BLOCK[j+i*256]];
    		}
    		*/
    		make_dirent_from_string(&BLOCK->data[i*256], ptr);
    		if(is_file( &fs->inode_root[ptr->inode_num]))
					print_file(ptr->name);
			else if(is_directory(&fs->inode_root[ptr->inode_num]))
					print_directory(ptr->name);
    		free(ptr);
    	}
    	Datas -= sizeof(data_block)/256;
    
    	}
    }

    if(I->indirect != UNASSIGNED_NODE)
    {
    	ls_help(fs,&fs->inode_root[I->indirect],Datas);
    }	


}

void ls_help(file_system *fs, inode* I, size_t Datas){

	//fprintf(stderr, "ls_help\n");
	//fprintf(stderr, "size%zu\n",I->size);

	//size_t Datas = (I->size/256);

    for(size_t h = 0; h < NUM_DIRECT_INODES; h++)
    {
    	if(I->direct[h] == -1)
    		break;
    	data_block* BLOCK = &fs->data_root[I->direct[h]];

    	//fprintf(stderr, "data_block%zu\n",sizeof(data_block)/256);
    	//fprintf(stderr, "%zu\n",Datas);
    for(size_t i = 0; i < min(sizeof(data_block)/256,Datas); i++)
    	{
    		//char buf[248];
    		//char node[8];
    		dirent* ptr = calloc(1,sizeof(dirent));
    		/*
    		for(size_t j = 0; j < 248; j++)
    		{
   				
    			buf[j] = BLOCK[j+i*256]];
       		}
    		for(size_t j = 248; j < 256; j++)
    		{
    			
    			node[j] = BLOCK[j+i*256]];
    		}
    		*/
    		//fprintf(stderr, "for_loop\n");
    		make_dirent_from_string(&BLOCK->data[i*256], ptr);
    		if(is_file(&fs->inode_root[ptr->inode_num]))
					print_file(ptr->name);
			else if(is_directory(&fs->inode_root[ptr->inode_num]))
					print_directory(ptr->name);
    		free(ptr);
    	}
    	Datas -= sizeof(data_block)/256;
    
    	}

    	 if(I->indirect != UNASSIGNED_NODE)
    {
    	ls_help(fs,&fs->inode_root[I->indirect], Datas);
    }	

}

void fs_cat(file_system *fs, char *path) {
    // Shiver me inodes
    inode* I = get_inode(fs, path);
    if(!I)
    {
    	print_no_file_or_directory();
    	return;
    }

    size_t RemainningSize = I->size;
    
    for(size_t i = 0; i < NUM_DIRECT_INODES; i++)//size is not ok
    {
    	if(I->direct[i] == -1)
    		break;
    	data_block* current_block = &fs->data_root[I->direct[i]];
    	for(size_t j = 0; j < min(sizeof(data_block),RemainningSize); j++)
    	{
    		   write(1,&current_block->data[j],1);
		}
		RemainningSize -= sizeof(data_block);
    }

    if(I->indirect != UNASSIGNED_NODE)
    {
    	cat_RC(fs,&fs->inode_root[I->indirect],RemainningSize);
    }
}

void cat_RC(file_system *fs, inode* I,size_t RemainningSize) {

	//size_t RemainningSize = I->size;

	for(size_t i = 0; i < NUM_DIRECT_INODES; i++)//size is not ok
    {
    	if(I->direct[i] == -1)
    		break;
    	data_block* current_block = &fs->data_root[I->direct[i]];
    	for(size_t j = 0; j < min(sizeof(data_block),RemainningSize); j++)
    	{
    		   write(1,&current_block->data[j],1);
		}
		RemainningSize -= sizeof(data_block);
    }

    if(I->indirect != UNASSIGNED_NODE)
    {
    	cat_RC(fs,&fs->inode_root[I->indirect],RemainningSize);
    }
}


