/**
 * Chatroom Lab
 * CS 241 - Spring 2017
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "utils.h"





static const size_t MESSAGE_SIZE_DIGITS = 4;

char *create_message(char *name, char *message) {
    int name_len = strlen(name);
    int msg_len = strlen(message);
    char *msg = calloc(1, msg_len + name_len + 4);
    sprintf(msg, "%s: %s", name, message);

    return msg;
}

ssize_t get_message_size(int socket) {
    int32_t size;
    ssize_t read_bytes =
        read_all_from_socket(socket, (char *)&size, MESSAGE_SIZE_DIGITS);
    if (read_bytes == 0 || read_bytes == -1)
        return read_bytes;

    return (ssize_t)ntohl(size);
}

// You may assume size won't be larger than a 4 byte integer
ssize_t write_message_size(size_t size, int socket) {
    // Your code here
    int32_t shize = htonl((uint32_t)size);
    //char tp[MESSAGE_SIZE_DIGITS];
    //sprintf(tp,"%zu",size);
    ssize_t write_bytes =  
        write_all_to_socket(socket, (char*)&shize, MESSAGE_SIZE_DIGITS);
    if (write_bytes == 0 || write_bytes == -1)
        return write_bytes;

    return write_bytes;

   // return 9001;
}

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
    
size_t totalNumber = 0;
//int len = read(socket, buffer, count);
int len;
while ((len = read(socket, buffer+totalNumber, count- totalNumber)) > -10)
{
   // fprintf(stderr, "len %d\n", len);
    //if(len > 0)
    //{
      //  totalNumber += len;
       // return totalNumber;
    //}
    if(len == -1)
    {

        if(errno == EINTR)
            continue;
        else
           {
            
            return -1;
        }
    }
    else if(len == 0)
    {
        break;
    }
    totalNumber += len;
    if(totalNumber == count)
        break;

}



return totalNumber;

    // Your Code Here
   // return 9001;
}

ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
    // Your Code Here
   size_t totalNumber = 0;

   
//int len = read(socket, buffer, count);
int len;
while ((len = write(socket, buffer+totalNumber, count - totalNumber)) > -10)
{
       // fprintf(stderr, "wirtelen %d\n", len);

    //if(len > 0)
    //{
      //  totalNumber+=len;
       // return totalNumber;
    //}
    if(len == -1)
    {
        if(errno == EINTR)
            continue;
        else
        {

          //  fprintf(stderr, "errononotset\n");
            return -1;
        }
    }
    else if(len == 0)
    {
        break;
    }
    totalNumber += len;
    if(count == totalNumber)
        break;

}


return totalNumber;

}
