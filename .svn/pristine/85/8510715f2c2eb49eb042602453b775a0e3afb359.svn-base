/**
 * Machine Problem: Text Editor
 * CS 241 - Spring 2017
 */
#include "document.h"
#include "editor.h"
#include "format.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *get_filename(int argc, char *argv[]) {
  if(argc == 1)
  {
     print_usage_error();
     return NULL;
  }
  return argv[1];
  // TODO implement get_filename
  // take a look at editor_main.c to see what this is used for
  //return NULL;
}

Document *handle_create_document(char *path_to_file) {
  Document * doc = Document_create_from_file(path_to_file);
  // TODO create the document
  return doc;
}

void handle_cleanup(Document *document) {
  Document_destroy(document);
  // TODO destroy the document
}

void handle_display_command(Document *document, size_t start_line,
                            size_t max_lines) {
//  if(start_line <= 0)
 // {
//   print_usage_error();
 //  return; 
//  }
 size_t lineNumber = Document_size(document);
 if(lineNumber == 0)
 {
  print_document_empty_error();
  return;
 }

if(start_line>lineNumber)
 {
  print_document_empty_error();
  return; 
 }

if(start_line <= 0)
  {
   print_usage_error();
   return;
  }

 
 for(size_t i = start_line; i <= fmin(lineNumber,start_line+max_lines-1);i++)
 {
  print_line(document, i);
 }
  return;// TODO implement handle_display_command
}

void handle_insert_command(Document *document, location loc, char *line) {
   //Document_insert_line(document,loc,line);
  size_t lineLoc = loc.line_no;
  size_t charLoc = loc.idx;
  size_t lineNumber = Document_size(document);
  if(lineLoc > lineNumber)
  {
    Document_insert_line(document,lineLoc,line);
    return;
  }

  const char* originLine = Document_get_line(document,lineLoc);
  if(strlen(originLine) == 0)
  {
    Document_set_line(document,lineLoc,line);
    return;
  }
  char newline[strlen(originLine)+strlen(line)+1];
  if(charLoc == 0)
  {
    strcpy(newline,line);
    strcat(newline,originLine);
    Document_set_line(document,lineLoc,newline);
    return;
  }
  else
  {
    strncpy(newline,originLine,charLoc);
    newline[charLoc] = '\0';
    strncat(newline,line,strlen(line));
    strncat(newline,originLine+charLoc,strlen(originLine)-charLoc);
    Document_set_line(document,lineLoc,newline);
    return;
  }
  // TODO implement handle_insert_command
}

void handle_delete_command(Document *document, location loc, size_t num_chars) {
  size_t lineLoc = loc.line_no;
  size_t charLoc = loc.idx;
  size_t lineNumber = Document_size(document);
  if(lineLoc>lineNumber || lineLoc < 1)
  {
    print_usage_error();
    return;
  }
  //change...

  const char* originLine = Document_get_line(document,lineLoc);
  char newline[strlen(originLine)];
  strncpy(newline,originLine,charLoc);
  newline[charLoc] = '\0';
  size_t numChars = fmin(num_chars,strlen(originLine+charLoc));
  strcat(newline,originLine+charLoc+numChars);

//strncpy(&newline,originLine,(strlen(originLine) - num_chars));
//newline[(strlen(originLine) - num_chars)] = '\0';
  Document_set_line(document,lineLoc,newline);
  return;
  // TODO implement handle_delete_command
}

void handle_delete_line(Document *document, size_t line_no) {
  size_t lineNumber = Document_size(document);
  if(line_no>lineNumber || line_no < 1)
  {
    print_usage_error();
    return;
  }
  Document_delete_line(document,line_no);// TODO implement handle_delete_line
}

location handle_search_command(Document *document, location loc,
                               const char *search_str) {
   size_t lineLoc = loc.line_no;
   size_t charLoc = loc.idx;
   size_t lineNumber = Document_size(document);
  if(lineLoc>lineNumber || lineLoc < 1)
  {
    print_usage_error();
    return (location){0, 0};
  }
   int argum = 1;
   size_t lineLocCurrN = lineLoc + 1;
   int times = 0;
   while(argum)
   {
   const char* originLine = Document_get_line(document,lineLoc);
   char* temp = strstr(originLine+charLoc,search_str);
   if(temp == NULL)
   {
    lineLoc ++;
    if(lineLoc == lineLocCurrN && times)
      return (location){0, 0};

    if(lineLoc > Document_size(document))
     { lineLoc = 1;
        times  = 1;
      }
    charLoc = 0;
   }
   else
   {
    return (location){lineLoc,(size_t)((temp - originLine)/sizeof(char))};
   }
  }
// TODO implement handle_search_command
  return (location){0, 0};
}

void handle_merge_line(Document *document, size_t line_no) {
  if(line_no >= Document_size(document) || line_no < 1)
  {
    print_usage_error();
    return;
  }
  const char* originLine = Document_get_line(document,line_no);
  const char* nextLine = Document_get_line(document,line_no+1);
  char output[strlen(originLine)+strlen(nextLine)+1];
  strcpy(output,originLine);
  strcat(output,nextLine);
  Document_set_line(document,line_no,output);
  Document_delete_line(document,line_no+1);
  // TODO implement handle_merge_command
}

void handle_split_line(Document *document, location loc) {
  size_t lineLoc = loc.line_no;
  size_t charLoc = loc.idx;
  size_t lineNumber = Document_size(document);
  if(lineLoc>lineNumber || lineLoc < 1)
  {
    print_usage_error();
    return;
  }


  const char* originLine = Document_get_line(document,lineLoc);
  char newOrigin[charLoc+1];
  strncpy(newOrigin,originLine,charLoc);
  newOrigin[charLoc] = '\0';
  char newNext[strlen(originLine)-charLoc+1];
  strcpy(newNext,originLine+charLoc);
  Document_set_line(document,lineLoc,newOrigin);
  Document_insert_line(document,lineLoc + 1,newNext);
  // TODO implement handle_split_line
}

void handle_save_command(Document *document, const char *filename) {
  
  Document_write_to_file(document,filename);// TODO implement handle_save_command
}
