/**
 * Mad Mad Access Pattern
 * CS 241 - Spring 2017
 */
#include "tree.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses mmap to access the data.

  ./lookup2 <data_file> <word> [<word> ...]
*/
void findword(char* addr, char *Target, BinaryTreeNode* root);
int main(int argc, char **argv) {
	
	if(argc <= 2)
	{
		printArgumentUsage();
		exit(1);
	}


struct stat buf;
if(stat(argv[1], &buf) == -1){
	exit(2);
}
int fd = open(argv[1], O_RDONLY);
if(fd == -1)
{
	openFail(argv[1]);
		exit(2);
}
ssize_t SiZe =  (ssize_t)buf.st_size;
//fprintf(stderr, "size is %zd\n", SiZe);

char* addr = (char*)mmap(NULL, SiZe, PROT_READ, MAP_SHARED, fd, 0);

if((void*)-1 == (void*)addr)
{
	mmapFail(argv[1]);
	exit(2);
}
//printf("address of addr is %p\n", addr);
char formatcheker[5]; 
for(int i = 0; i< 4; i++)
{
	formatcheker[i] = addr[i];
	//fprintf(stderr, "%c\n", formatcheker[i]);
}
formatcheker[4] = '\0';
if(strcmp(formatcheker,"BTRE\0") != 0)
	{
		formatFail(argv[1]);
		exit(2);
	}

BinaryTreeNode* root = (BinaryTreeNode*)((char*)addr+4);	

for(int i = 2; i < argc; i ++)
	{
		findword((char*)addr,argv[i],root);
	}
munmap(addr,SiZe);
close(fd);

 return 0; }


 void findword(char* addr, char *Target, BinaryTreeNode* root)
{
	//fprintf(stderr, "Target is %s\n", Target);
	//fprintf(stderr, "root is %s\n", root->word);
	int compare = strcmp(root->word,Target);
	//fprintf(stderr, "compare is %d\n", compare);
	if(compare == 0)
	{
		printFound(root->word, root->count,root->price);
		return;
	}
	else if(compare < 0)
	{
		if(root->right_child == 0)
		{
			printNotFound(Target);
			return;
		}
	
		BinaryTreeNode* right_child = (BinaryTreeNode*)(addr+root->right_child);//need free

		findword(addr,Target,right_child);

		return;
	}
	else
		{

			if(root->left_child == 0)
		{
			printNotFound(Target);
			return;
		}

	
		BinaryTreeNode* left_child = (BinaryTreeNode*)(addr+root->left_child);

		findword(addr,Target,left_child);

		return;

		}	

}


