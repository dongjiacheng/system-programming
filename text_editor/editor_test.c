/**
 * Machine Problem: Text Editor
 * CS 241 - Spring 2017
 */
#include "document.h"
#include "editor.h"
#include "format.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * You can programatically test your text editor.
*/
int main() {
  // Setting up a docment based on the file named 'filename'.
  char *filename = "test.txt";
  Document *document = Document_create_from_file(filename);
  handle_save_command(document,"test0.txt");
  handle_insert_command(document,(location){1, 0},"one ");
  location loc;
  loc = handle_search_command(document,(location){1, 0},"Second");
  handle_split_line(document,loc);
  handle_insert_command(document,(location){1, 4},"two");
  handle_insert_command(document,(location){1,7},"three");
//  handle_display_command(document, 1, 5);
  handle_delete_command(document,(location){1, 4},21);
  handle_insert_command(document,(location){1, 0},"I want to 3");
  handle_insert_command(document,(location){2, 12},"I want to 4");
  //  handle_display_command(document, 1, 2);
  handle_save_command(document,"test2.txt");
  handle_delete_line(document,1);
//   print the first 5 lines
//  handle_display_command(document, 1, 5);
  handle_merge_line(document,1);
  handle_cleanup(document);
//  handle_save_command(document,"test2.txt");
}
