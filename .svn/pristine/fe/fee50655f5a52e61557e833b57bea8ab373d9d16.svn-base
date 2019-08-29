/**
 * Mad Mad Access Pattern
 * CS 241 - Spring 2017
 */
#include "tree.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup1 <data_file> <word> [<word> ...]
*/
void findword(FILE* file, char *Target, BinaryTreeNode* root, uint32_t offset, char* rootName);

int main(int argc, char **argv) { 

	if(argc <= 2)
	{
		printArgumentUsage();
		exit(1);
	}
	//argv[1]file name, argv[2]...keywords array
	FILE* file = fopen(argv[1],"r");
	if(file == NULL)
	{
		openFail(argv[1]);
		exit(2);
	}
	char formatcheker[5];
	size_t result = fread(&formatcheker,  4, 1, file);
	if(result != 1)
	{
		fprintf(stderr, "file not long enough\n");
		exit(2);
	}
	formatcheker[4] = '\0';

	//fprintf(stderr, "%s\n", formatcheker);
	if(strcmp(formatcheker,"BTRE\0") != 0)
	{
		formatFail(argv[1]);
		exit(2);
	}

	fseek(file, 4, SEEK_SET);
//	int h = (int)ftell(file);
	//ftell()
//	fprintf(stderr, "%d\n", h);

	BinaryTreeNode* root = calloc(1,sizeof(BinaryTreeNode)+8);//need free
	
	result = fread(root, sizeof(BinaryTreeNode), 1, file);

	fseek(file, sizeof(BinaryTreeNode), SEEK_CUR);

	while((char)fgetc(file) != '\0')
	{
		fseek(file, 1, SEEK_CUR);
	}

	long buf_length = ftell(file) - 4;

	char* buf = calloc(1,buf_length);
	fseek(file, 4 + sizeof(BinaryTreeNode), SEEK_SET);
	fread(buf, buf_length, 1, file);

	//fprintf(stderr, "%lu\n", sizeof(BinaryTreeNode));
	//fprintf(stderr, "something here %s\n", buf);
	//write(1,(char*)root,16);
	for(int i = 2; i < argc; i ++)
	{
		findword(file,argv[i],root,4,buf);
	}
	free(buf);
	free(root);
	fclose(file);
	return 0; 
	}

void findword(FILE* file, char *Target, BinaryTreeNode* root, uint32_t offset, char* rootName)
{
	//fprintf(stderr, "Target is %s\n", Target);
	//fprintf(stderr, "root is %s\n", rootName);
	int compare = strcmp(rootName,Target);
	//fprintf(stderr, "=======%d\n", offset);
	if(compare == 0)
	{
		printFound(rootName, root->count,root->price);
		return;
	}
	else if(compare < 0)
	{
		if(root->right_child == 0)
		{
			printNotFound(Target);
			return;
		}

		fseek(file, root->right_child, SEEK_SET);
	
		BinaryTreeNode* right_child = calloc(1,sizeof(BinaryTreeNode));//need free
	
		fread(right_child,  sizeof(BinaryTreeNode), 1, file);

		fseek(file, sizeof(BinaryTreeNode), SEEK_CUR);

		while((char)fgetc(file) != '\0')
		{
		fseek(file, 1, SEEK_CUR);
		}

		long buf_length = ftell(file) - offset;

		char* buf = calloc(1,buf_length);
		fseek(file, root->right_child + sizeof(BinaryTreeNode), SEEK_SET);
		fread(buf, buf_length, 1, file);

		findword(file,Target,right_child,root->right_child,buf);

		free(buf);
		free(right_child);

		return;
	}
	else
		{

			if(root->left_child == 0)
		{
			printNotFound(Target);
			return;
		}

		fseek(file, root->left_child, SEEK_SET);
	
		BinaryTreeNode* left_child = calloc(1,sizeof(BinaryTreeNode));//need free
	
		fread(left_child,  sizeof(BinaryTreeNode), 1, file);

		fseek(file, sizeof(BinaryTreeNode), SEEK_CUR);

		while((char)fgetc(file) != '\0')
		{
		fseek(file, 1, SEEK_CUR);
		}

		long buf_length = ftell(file) - offset;

		char* buf = calloc(1,buf_length);
		fseek(file, root->left_child + sizeof(BinaryTreeNode), SEEK_SET);
		fread(buf, buf_length, 1, file);

		findword(file,Target,left_child, root->left_child, buf);

		free(buf);

		free(left_child);

		return;

		}	

}





