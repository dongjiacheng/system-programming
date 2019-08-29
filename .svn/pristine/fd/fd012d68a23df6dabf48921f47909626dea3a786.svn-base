/**
 * Machine Problem: Vector
 * CS 241 - Spring 2017
 */

#include "document.h"
#include <stdio.h>
int main(int argc, char *argv[]) {
  // Write your test cases here.
	if(argc != 3)
	{
		perror("two file name");
		exit(1);
	}

  document* Doc = document_create();
  char* A = "this is a test";
  char* B = "To check whether it is OK";
  char* C = "There may be a lot of Problem";
  char* D = "I would try to fix it";
  document_insert_line(Doc,1,D);
  document_insert_line(Doc,1,C);
  document_insert_line(Doc,1,B);
  document_insert_line(Doc,1,A);
  document_write_to_file(Doc,argv[1]);
  puts("1");
  document* D2 = document_create_from_file(argv[1]);
  puts("2");
  document_delete_line(D2,2);
  document_write_to_file(D2,argv[2]);
  document_destroy(Doc);
  document_destroy(D2);
  return 0;
}
