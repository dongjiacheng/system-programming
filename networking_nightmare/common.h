/**
 * Networking
 * CS 241 - Spring 2017
 */
#pragma once
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
//#define MSG_SIZE (256)


#define LOG(...)                                                               \
  do {                                                                         \
    fprintf(stderr, __VA_ARGS__);                                              \
    fprintf(stderr, "\n");                                                     \
  } while (0);

typedef enum { GET, PUT, DELETE, LIST } verb;

char *create_message(char *name, char *message);

ssize_t get_message_size(int socket);

ssize_t write_message_size(size_t size, int socket);

ssize_t read_all_from_socket(int socket, char *buffer, size_t count);

ssize_t write_all_to_socket(int socket, const char *buffer, size_t count);

ssize_t read_all_from_socket_server(int socket, char *buffer, size_t count);