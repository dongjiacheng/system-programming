/**
 * Networking
 * CS 241 - Spring 2017
 */
//#include <stdio.h>

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
//#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
//#include "client.c"
#include "vector.h"
#include "dictionary.h"
#include <sys/epoll.h>
#include "common.h"
#include "format.h"
//#include <utils.h>
#define MAX_CLIENTS 2000
#define MAXEVENTS 400
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
static volatile int endSession;

//static volatile int clientsCount;
//static volatile int clients[MAX_CLIENTS];



vector* name_list;
int sock_fd;
struct addrinfo hints, *result;
int epoll_fd;
dictionary* stateDIC;

char *err_bad_request1 =
    "Bad request\n"; // Error message to send in response to a malformed request
char *err_bad_file_size1 = "Bad file size\n"; // Error message to send if
                                                   // the client sends too
                                                   // little or too much data
char *err_no_such_file1 = "No such file\n"; // Error message if a client
                                                 // tries to GET or DELETE a non
                                                 // existent file

//static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void string_destructor(void *elem) { free(elem); }

void *string_default_constructor(void) { return calloc(1, sizeof(char*)); }

int getSIZE(int fd);

int getDATA(int fd);

int getVERB(int fd);

int getFILENAME(int fd);

int writeSIZE(int fd);

int writeDATA(int fd);

void *string_copy_constructor(void *elem) {
  char* temp = (char*)elem;
  char* copy = strdup(temp);
  return copy;
}

typedef struct information{
  
  int state; // 0 is just start to read
  verb VERB;
  int VERBoffset;
  int SIZEoffset;
  int DATAoffset;
  char* verbBuffer;
  size_t sizeBuffer;
  char* dataBuffer;
  char* filename;
  int FILENAMEoffset;
  int fdToWrite;
  int mt;
}info;

void element_free(info* I)
{
      if(I->verbBuffer != NULL)
      {
            free(I->verbBuffer);
            I->verbBuffer = NULL;
      }
      if(I->filename != NULL)
      {
            free(I->filename);
            I->filename = NULL;
      }
      if(I->dataBuffer != NULL)
      {
            free(I->dataBuffer);
            I->dataBuffer = NULL;
      }
}

void close_server() {
    endSession = 1;
    
    // add any additional flags here you want.
}

void handle_data(struct epoll_event *e);


void cleanup() {
        freeaddrinfo(result);
        vector* allkey = dictionary_keys(stateDIC);
        for(size_t i = 0; i < vector_size(allkey);i ++)
        {
                  int* value = (int*)vector_get(allkey,i);
                  info* neeedfree = dictionary_get(stateDIC,value);
                  dictionary_remove(stateDIC,value);
                  element_free(neeedfree);
                  free(neeedfree);
        }
      vector_destroy(name_list);
      vector_destroy(allkey);

      dictionary_destroy(stateDIC);
}

void accept_connections(struct epoll_event *e, int epoll_fd){
      while(endSession == 0)
      {

            struct sockaddr_in new_addr;
            socklen_t new_len = sizeof(new_addr);
            int new_fd = accept(e->data.fd,(struct sockaddr*)&new_addr, &new_len);


            if(new_fd == -1)
            {
                  if(errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                  else
                  {
                        perror("accpet");
                        exit(1);
                  }
            }

            int flags = fcntl(new_fd, F_GETFL, 0);
            fcntl(new_fd, F_SETFL, flags | O_NONBLOCK);

                          struct epoll_event event;
                          event.data.fd = new_fd;
                  event.events = EPOLLIN | EPOLLET | EPOLLOUT;
                  if(epoll_ctl (epoll_fd, EPOLL_CTL_ADD, new_fd, &event) == -1)
                   {
                      perror ("epoll_ctl");
                      exit(1);
                    }
                  info* current_info = calloc(1,sizeof(info));
                        current_info->verbBuffer = calloc(1,7);
                        current_info->VERBoffset = 0;
                        current_info->SIZEoffset = 0;
                        current_info->DATAoffset = 0;
                        current_info->mt = 0;
                        //current_info->sizeBuffer = calloc(1,sizeof(size_t));
                        current_info->dataBuffer = NULL;
                        current_info->state = 0;
                        current_info->filename = calloc(1,256);
                        dictionary_set(stateDIC, &new_fd, current_info);

      }

}

void run_server(char *port) {
    
    //for(int i = 0; i < MAX_CLIENTS; i ++)
    //{
    //    clients[i] = -1;
    //}

      signal(SIGPIPE, SIG_IGN);

    int s;
    sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, port, &hints, &result);
     if (s != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            exit(1);
    }
    int optval = 1;
    
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT|SO_REUSEADDR, &optval, sizeof(optval));
    //??this one??

      if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
        perror("bind()");
        exit(1);
    }

    if (listen(sock_fd, -1) != 0) {
        perror("listen()");
        exit(1);
    }

    struct epoll_event event;
    struct epoll_event *events;

    epoll_fd = epoll_create(1);
    event.data.fd = sock_fd;
    event.events = EPOLLIN | EPOLLET; // needs change flag here
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &event))
    {
      perror("epoll_clt");
      exit(1);
    }

    events = calloc (MAXEVENTS, sizeof(event));


    while(endSession == 0)
    {
      //struct epoll_event new_event;
      int n;
      if((n = epoll_wait(epoll_fd, events, MAXEVENTS, -1)) > 0)
      {
            for (int i = 0; i < n; i++)
            {
                  if ((events[i].events & EPOLLERR) ||
              (events[i].events & EPOLLHUP))
                        //add EPOOLOUT right or wrong
                        {
              /* An error has occured on this fd, or the socket is not
                 ready for reading (why were we notified then?) */
                        fprintf (stderr, "epoll error\n");
                        close (events[i].data.fd);
                        continue;
                  }
                  else if(sock_fd ==  events[i].data.fd)
                  accept_connections(&events[i],epoll_fd);
                  else
                  {
                  //fprintf(stderr, "here??\n");
                  handle_data(&events[i]);      //data_handler
                  }
            }
      }
    
    }
    close(epoll_fd);//Close fd here
    free(events);
       

 }

void handle_data(struct epoll_event *e)
{
      int current_fd = e->data.fd;
      //fprintf(stderr, "226\n");
      //fprintf(stderr, "fdNB%d\n",current_fd);
      //int* ttd = calloc(1,sizeof(int));
      //*ttd = current_fd;
      info* current_info = dictionary_get(stateDIC,&current_fd);
      if(current_info == NULL)
                  {
                        current_info = calloc(1,sizeof(info));
                        current_info->verbBuffer = calloc(1,7);
                        current_info->VERBoffset = 0;
                        current_info->SIZEoffset = 0;
                        current_info->DATAoffset = 0;
                        //current_info->sizeBuffer = calloc(1,sizeof(size_t));
                        current_info->dataBuffer = NULL;
                        current_info->state = 0;
                        current_info->filename = calloc(1,256);
                        dictionary_set(stateDIC, &current_fd, current_info);
                  }

      int sttate = current_info->state;
      //int out = -1;
      int s = 1;
      switch(sttate){   
      case 0 :
      s = getVERB(current_fd);
      if(s == -1)
            {
                 // fprintf(stderr, "fd closed due to get verb error\n");
                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }
                  write_all_to_socket(current_fd, err_bad_request, strlen(err_bad_request));
                  
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                  perror("reading problem totally wrong");
                  return;
            }


      if(s == 0)
      {
            return;
      }
      case 1:
      current_info = dictionary_get(stateDIC,&current_fd);
      //fprif(stderr, "258\n");
      if(current_info->VERB == LIST)
      {
            current_info->state = -1;
            dictionary_set(stateDIC, &current_fd, current_info);
            break;
      }
      else
      {
            s = getFILENAME(current_fd);
            if(s == -1)
            {
                  fprintf(stderr, "fd closed due to get FILENAME error\n");
                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                 // fprintf(stderr, "response write%d\n", h);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                  fprintf(stderr, "socket closed\n");
                              }
                              fprintf(stderr, "strange error%d\n", errno);
                              return;
                  }
                  write_all_to_socket(current_fd, err_bad_request, strlen(err_bad_request));
                  //fprintf(stderr, "write value %d\n", rtval);
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  //sleep(1);
                  shutdown(current_fd,SHUT_WR);
                  //sleep(1);
                  close(e->data.fd);
                  perror("reading  FILENAME  problem totally wrong");
                  return;
            }
            if(s == 0)
            {
                  return;
            }

      }
      //    fprintf(stderr, "296\n");

      case 2:
      current_info = dictionary_get(stateDIC,&current_fd);
      if(current_info->VERB == PUT)
      {
            s = getSIZE(current_fd);
            //    fprintf(stderr, "303\n");
            //    fprintf(stderr, "s value is%d\n", s);

            if(s == -1)
            {
                 // fprintf(stderr, "fd closed due to get size error\n");
                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }
                  write_all_to_socket(current_fd, err_bad_request, strlen(err_bad_request));
                  
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                  perror("reading  SIZE  problem totally wrong");
                  return;
            }
            if(s == 0)
            {
                  return;
            }

      }
      else
      {
            char ttttth;
            if(read(current_fd,&ttttth,1) == 1)
            {
                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }
                  write_all_to_socket(current_fd, err_bad_request, strlen(err_bad_request));
                  
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                  perror("reading  SIZE  problem totally wrong");
                  return;
            }
            if(current_info->VERB == GET)
            {
                  //fprintf(stderr, "getting\n");
                  struct stat bufed;
                  if(stat(current_info->filename, &bufed) == -1)
                  {
                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }

                  write_all_to_socket(current_fd, err_no_such_file, strlen(err_no_such_file));
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(current_fd);
                  return;
                  }
                  else
                  {
                        char response[] = "OK\n";
                        int h = write_all_to_socket(current_fd, response, 3);
                        if(h == -1)
                        {
                         if (errno == EPIPE)
                              {
                  dictionary_remove(stateDIC, &current_fd);
                        element_free(current_info);
                        free(current_info);
                        shutdown(e->data.fd,SHUT_WR);
                        close(e->data.fd);
                              
                              }
                              return;
                        }
                  
                  }

                  //struct epoll_event event;
                  //      event.data.fd = current_fd;
              //    event.events = EPOLLOUT | EPOLLET;

            //epoll_ctl(epoll_fd, EPOLL_CTL_MOD, current_fd, &event);
            size_t size =  (size_t)bufed.st_size;
            //fprintf(stderr, "size of the file is %zu\n",size);
            current_info->sizeBuffer = size;
            int readFile = open(current_info->filename,O_RDONLY);
            current_info->fdToWrite = readFile;
           
            if(readFile == -1)
            {
            char response[] = "ERROR\n";
                  write_all_to_socket(current_fd, response, 6);
                  write_all_to_socket(current_fd, err_no_such_file, strlen(err_no_such_file));
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(current_fd);
                  return;     
            }

            //current_info->dataBuffer = calloc(1,size);
            //current_info->SIZEoffset = 0;

            //int retval = read_all_from_socket(readFile,current_info->dataBuffer,size);
            //if(retval == -1)
            //{
            //    perror("read wrong");
            //    exit(2);
            //}
            //close(readFile);
                  current_info->state = 3;
                  dictionary_set(stateDIC, &current_fd, current_info);
            }
            else
            {
            //fprintf(stderr, "set state -1 500\n");    
            current_info->state = -1;
            dictionary_set(stateDIC, &current_fd, current_info);
            break;
            }
      }
      //fprintf(stderr, "318\n");
      case 3:
      current_info = dictionary_get(stateDIC,&current_fd);
      if(current_info->VERB == PUT)
      {
            
            //s = getDATA(current_fd);
            //fprintf(stderr, "gedata value is %d\n", s);
            int out = -1;
            if(access(current_info->filename,F_OK) != 0)
                  {
                  //size_t size = 0;
                  out = open(current_info->filename,O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR| S_IWUSR|S_IRWXO|S_IXUSR|S_IRWXG );
                  if(out == -1)
                  {
                       // fprintf(stderr, "open / create fail%d\n", errno);
                        dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                        
                        return;
                  }
                  vector_push_back(name_list, current_info->filename);

                  }
                  else
                  {
                        out = open(current_info->filename,O_WRONLY|O_TRUNC);
                        if(out == -1)
                        {
                       // fprintf(stderr, "open fail\n");
                        dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                        return;
                        }

                  }
                  
            current_info->fdToWrite = out;      
            current_info->state = -1;
            dictionary_set(stateDIC, &current_fd, current_info);
            break;            
      //if(s == 0)
      //    return;
      
      }
      else if(current_info->VERB == GET)
      {
            s = writeSIZE(current_fd);
            //    fprintf(stderr, "303\n");
            //    fprintf(stderr, "s value is%d\n", s);
            
            if(s == -1)
            {
                 // fprintf(stderr, "fd closed due to write size error\n");
                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }
                  //write_all_to_socket(current_fd, err_bad_request, sizeof(err_bad_request));
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                  perror("writing  SIZE  problem totally wrong");
                  return;
            }
            if(s == 0)
            {
                  return;
            }
            
          //  fprintf(stderr, "pass 628\n");
      }
      else
      {
            break;
      }


            }
      //fprintf(stderr, "330\n");
      current_info = dictionary_get(stateDIC,&current_fd);
      //fprintf(stderr, "%s\n", current_info->verbBuffer);


      if(current_info->VERB == LIST)
      {
            //fprintf(stderr, "read LIST\n");

            char response[] = "OK\n";
            int h = write_all_to_socket(current_fd, response, 3);
            if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }
            size_t size = 0;
            //fprintf(stderr, "vector size %zu\n", vector_size(name_list));
            for(size_t i = 0; i < vector_size(name_list); i++)
            {
                  char* cur_filename = (char*)vector_get(name_list, i);
                  //fprintf(stderr, "%s\n", cur_filename);
                  size+= strlen(cur_filename);
                  size++;
                  //free(cur_filename);
            }
            if(size != 0)
            size--;
            //fprintf(stderr, "size to write%zu\n",size);
            //ssize_t retval = 
            write_message_size(size, current_fd);

            for(size_t i = 0; i < vector_size(name_list); i++)
            {
                  char* cur_filename = (char*)vector_get(name_list, i);
                  //fprintf(stderr, "%s\n", cur_filename);
                  //int tmpsize =  strlen(cur_filename);
                  //fprintf(stderr, "size I write%d\n", tmpsize);
                  write_all_to_socket(current_fd, cur_filename, strlen(cur_filename));
                  //fprintf(stderr, "retval%zd\n",retval );
                  if(i != vector_size(name_list)-1)
                  {
                        char n = '\n';
                        write_all_to_socket(current_fd,&n,1);
                  }
                  //free(cur_filename);
            }

            current_info = dictionary_get(stateDIC,&current_fd);
            
            dictionary_remove(stateDIC, &current_fd);
            element_free(current_info);
            free(current_info);
           // fprintf(stderr, "fd closed after LIST\n");
            shutdown(e->data.fd,SHUT_WR);
            close(current_fd);
            return;
      }

      if(current_info->VERB == PUT)
      {
            s = getDATA(current_fd);
            if(s == 0)
                  return;

            current_info = dictionary_get(stateDIC,&current_fd);
            close(current_info->fdToWrite);     
            if(s < 0)
            {
                              
                  unlink(current_info->filename);
                  for(size_t i = 0; i < vector_size(name_list); i++)
                  {
                        char* tpn = vector_get(name_list,i);
                        if(strcmp(current_info->filename,tpn)==0)
                        {
                              vector_erase(name_list,i);
                              break;
                        }
                  }

                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }

                  if(s == -1)
                        fprintf(stderr, "error happened\n");
                  else if(s == -2)
                        write_all_to_socket(current_fd, err_bad_file_size, strlen(err_bad_file_size));
                  else if(s == -3)
                        write_all_to_socket(current_fd, err_bad_file_size, strlen(err_bad_file_size));

                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(current_fd);
                  return;
            }
            else if(s)
            {

                  char response[] = "OK\n";
                  int h = write_all_to_socket(current_fd, response, 3);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              return;
                              }
                              
                  }
                  dictionary_remove(stateDIC, &current_fd);
                  
                  element_free(current_info);
                  free(current_info);
                  //fprintf(stderr, "fd closed after PUT\n");
                  shutdown(e->data.fd,SHUT_WR);
                  close(current_fd);
                  
                  //close(current_fd);
                  return;
            }
            return;
      }

      if(current_info->VERB == DELETE)
      {
            if(access(current_info->filename,F_OK) == 0)
            {
                  if(unlink(current_info->filename) == 0)
                  {
                  char response[] = "OK\n";
                  write_all_to_socket(current_fd, response, 3);
                  for(size_t i = 0; i < vector_size(name_list); i++)
                  {
                        char* tpn = vector_get(name_list,i);
                        if(strcmp(current_info->filename,tpn)==0)
                        {
                              vector_erase(name_list,i);
                              break;
                        }
                  }
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(current_fd);
                  return;
                  }
            }
                  //fprintf(stderr, "==================================\n");
                  char response[] = "ERROR\n";

                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }
                  write_all_to_socket(current_fd, err_no_such_file, strlen(err_no_such_file));
                  fprintf(stderr, "=========%lu============\n", strlen(err_no_such_file));
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(current_fd);
                  return;

      }

      if(current_info->VERB == GET)
      {
            s = writeDATA(current_fd);
            if(s == 0)
                  return;
            current_info = dictionary_get(stateDIC,&current_fd);

            close(current_info->fdToWrite);
            if(s == 1)
            {
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  close(current_fd);
                  return;
            }
            else
            {
                  char response[] = "ERROR\n";
                  int h = write_all_to_socket(current_fd, response, 6);
                  if(h == -1)
                  {
                         if (errno == EPIPE)
                              {
            dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(e->data.fd);
                              }
                              return;
                  }
                  write_all_to_socket(current_fd, err_no_such_file, strlen(err_no_such_file));
                  dictionary_remove(stateDIC, &current_fd);
                  element_free(current_info);
                  free(current_info);
                  shutdown(e->data.fd,SHUT_WR);
                  close(current_fd);
                  return;

            }

      }


}

int main(int argc, char **argv) {
  // Good luck!
      if (argc != 2) {
        fprintf(stderr, "./server <port>\n");
        return -1;
    }
    size_t temp = strlen(argv[1]);
    //puts("1");
    for(size_t i = 0; i < temp; i ++)
    {
      //fprintf(stderr, "%c\n", *(argv[1]+i));
      if(isdigit(*(argv[1]+i)) == 0)
      {
            printf("Usage : ./server <port> \n");
            return -1;
      }
    }

     struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = close_server;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        return 1;
    }

  char template[] = "storageXXXXXX";
  char *tmp_dirname = mkdtemp (template);

  if(tmp_dirname == NULL)
  {
     perror ("tempdir: error: Could not create tmp directory");
     exit (EXIT_FAILURE);
  }
    print_temp_directory(tmp_dirname);
    if (chdir (tmp_dirname) == -1)
      {
     perror ("tempdir: error: ");
     exit (EXIT_FAILURE);
       }

      name_list =  vector_create(string_copy_constructor,string_destructor,string_default_constructor);

    stateDIC =  dictionary_create(int_hash_function, int_compare,
                              int_copy_constructor,
                              int_destructor,
                              NULL,
                              NULL);

      run_server(argv[1]);
      for(size_t i = 0 ; i < vector_size(name_list); i ++)
      {
      char* cur_filename = (char*)vector_get(name_list, i);
      if(unlink(cur_filename)!=0)
      {
            fprintf(stderr, "==========name is %s\n", cur_filename);
      }//???
      //free(cur_filename);
      
      }

      if (chdir ("..") == -1)
      {
     perror ("tempdir: error: ");
     exit (EXIT_FAILURE);
       }

      if(rmdir(tmp_dirname) != 0)
            fprintf(stderr, "dose not remove\n");


       cleanup();

}

int getVERB(int fd_VERB){
                  //int* tfd = calloc(1,sizeof(int));
                  //*tfd = fd_VERB;
                  info* current_info = dictionary_get(stateDIC,&fd_VERB);
                  if(current_info == NULL)
                  {
                        current_info = calloc(1,sizeof(info));
                        current_info->verbBuffer = calloc(1,8);
                        current_info->VERBoffset = 0;
                  }

                  current_info->state = 0;


                  //char* buffer = calloc(1,7);
            //char buf;
            int bytesread = 0;
            int total_read = 0;
            //int startwrite = 0;
            while ((bytesread = read(fd_VERB, (current_info->verbBuffer+current_info->VERBoffset+total_read), 1)) > 0 ) {
                if(*(current_info->verbBuffer+current_info->VERBoffset+total_read) == '\n' || *(current_info->verbBuffer+current_info->VERBoffset+total_read) == ' ')
                  {

                  *(current_info->verbBuffer+current_info->VERBoffset+total_read + 1) = '\0';
                    total_read++;
                    break;
                  }
                 total_read++;
                 if(total_read > 8)
                  break;

               }


               //total_read++;
            if(bytesread == -1)
            {
                  if(errno != EAGAIN && errno != EWOULDBLOCK) //error exit
                  {
                        perror("read error");
                        printf("Closed connection on descriptor with error\n");
                        
                        return -1;
                  }
                  
                  current_info->state = 0;
                  current_info->VERBoffset += total_read;
                  dictionary_set(stateDIC, &fd_VERB, current_info);
                  if(current_info->VERBoffset >= 8)
                  {
                        //perror("read error");
                              //printf("Closed connection on descriptor with error\n");
                              print_error_message(err_bad_request1);
                              return -1;
                  }
                  return 0;
            }
            else
            {
                  current_info->state = 1;
                  current_info->VERBoffset += total_read;

                  if(current_info->VERBoffset >= 8)
                  {
                        //perror("read error");
                              //printf("Closed connection on descriptor with error\n");
                              print_error_message(err_bad_request1);
                              return -1;
                  }

                  if(strcmp("PUT \0",current_info->verbBuffer) == 0)
                  current_info->VERB = PUT;
                  else if(strcmp("DELETE \0",current_info->verbBuffer) == 0)
                  current_info->VERB = DELETE;
                  else if(strcmp("LIST\n\0",current_info->verbBuffer) == 0)
                  {
                    char akk;
                  if(read(fd_VERB,&akk,1) == 1)
                    return -1;
                  current_info->VERB = LIST;
                  }
                  else if(strcmp("GET \0",current_info->verbBuffer) == 0)
                  current_info->VERB = GET;
                  else
                  {
                        print_error_message(err_bad_request1);
                        return -1;
                  }

                  dictionary_set(stateDIC, &fd_VERB, current_info);
                  return 1;
            }

            return 1;

}

int getSIZE(int fd_SIZE){

                  info* current_info = dictionary_get(stateDIC,&fd_SIZE);
       
            int bytesread;
            int total_read = 0;
           

              while( (bytesread = read(fd_SIZE,((char*)&current_info->sizeBuffer)+current_info->SIZEoffset+total_read, sizeof(size_t) - current_info->SIZEoffset - total_read) ) > 0 )
              {

                  if(bytesread == -1)
                  {
                        if(errno == EINTR)
                                     continue;
                        else
                                    break;
                                     
                  }
                  else if(bytesread == 0)
                        {
                              break;
                        }
   
                        total_read += bytesread;
                        if(total_read == (sizeof(size_t)-current_info->SIZEoffset))
                        break;

              }
            
              current_info->SIZEoffset += total_read;
            
            if(bytesread == -1)
            {
                  if(errno != EAGAIN && errno != EWOULDBLOCK) //error exit
                        {
                        perror("read error");
                        printf("Closed connection on descriptor with error\n");
                        
                        //return -1;
                  
                  fprintf(stderr, "ero is %d\n", errno);
                  return -1;
                  }
                  dictionary_set(stateDIC, &fd_SIZE, current_info);
                  return 0;
            }
    

           // fprintf(stderr, "bytesread%d\n", bytesread);
            if(current_info->SIZEoffset == sizeof(size_t))
            {
                  current_info->state = 3;
                  current_info->SIZEoffset = 8;
                  dictionary_set(stateDIC, &fd_SIZE, current_info);
                  return 1;
            }
            else
            {
                  if(bytesread == 0 && current_info->SIZEoffset!= sizeof(size_t))
                  {
                        perror("not enought size data");
                        return -1;
                  }
                  //current_info->SIZEoffset += bytesread;
                  dictionary_set(stateDIC, &fd_SIZE, current_info);
                  return 0;
            }

      }

int getFILENAME(int fd_FILENAME)
{

                  info* current_info = dictionary_get(stateDIC,&fd_FILENAME);

            int bytesread = 0;
            int total_read = 0;

            if(current_info->FILENAMEoffset == 0)
              current_info->mt = 0;
           //int mt = 0;
            //int startwrite = 0;
            while ((bytesread = read(fd_FILENAME, (current_info->filename+current_info->FILENAMEoffset+total_read), 1)) > 0 ) {
                if(*(current_info->filename+current_info->FILENAMEoffset+total_read) == '\n' )
                  {

                  *(current_info->filename+current_info->FILENAMEoffset+total_read) = '\0';
                    total_read++;
                    current_info->mt = 1;
                    break;
                  }
                 total_read++;
                 if(total_read + current_info->FILENAMEoffset > 256)
                  break;

               }

              

                    
             current_info->FILENAMEoffset += total_read;

              if(current_info->FILENAMEoffset > 255)
              {
                fprintf(stderr, "too much filename\n");
                  return -1;
              }

              // total_read++;
            if(bytesread == -1)
            {
                  if(errno != EAGAIN && errno != EWOULDBLOCK) //error exit
                  {
                        perror("read error");
                        printf("Closed connection on descriptor with error\n");
                        
                        return -1;
                  }

                  
                  current_info->state = 1;
                  dictionary_set(stateDIC, &fd_FILENAME, current_info);
                  return 0;
            }
            else
            {
                  // if(mt == 0)
                    //    return -1;

                  if(current_info->FILENAMEoffset == 0)
                        return -1;

                  if(current_info->mt == 0)
                    return -1;

                 // fprintf(stderr, "good??? %dn", current_info->mt);

                  current_info->state = 2;
                  
                  current_info->FILENAMEoffset += total_read;
                  dictionary_set(stateDIC, &fd_FILENAME, current_info);
                  return 1;
            }

            return 1;




}

int getDATA(int fd_DATA)
{

                  info* current_info = dictionary_get(stateDIC,&fd_DATA);
                  //if(current_info->dataBuffer == NULL)
                  //{
                  //    current_info->dataBuffer = calloc(1,1024);
                  //    current_info->DATAoffset = 0;
                  //}
                  //int out = open(current_info->filename,O_WRONLY);
                  //lseek(out, current_info->DATAoffset, SEEK_SET);
                  int out = current_info->fdToWrite;
                  char* buffer = calloc(1,4096);
                  if(out < 0)
                  {
                        fprintf(stderr, "out is eeror value is : %d\n",out);
                        return -1;
                  }
                  int bytesread = 0;
            size_t total_read = 0;
            size_t size = current_info->sizeBuffer;
            int offset = current_info->DATAoffset;

            while(1)
            {
                  bytesread = read(fd_DATA, buffer, 4096);
                 // fprintf(stderr, "==============put %d\n", bytesread );
                  if(bytesread == -1)
                  {
                        if(errno == EINTR)
                                     continue;
                        else
                                    break;
                                     
                  }

                  if(bytesread == 0)
                        break;
                  
                  total_read+= bytesread;

                  //fprintf(stderr, "%lu\n", total_read % 4096);

                  int writed = write_all_to_socket(out, buffer ,bytesread);
                  if(writed != bytesread)
                  {
                    perror("does not write leaglly\n"); 

                  }

                  if(total_read >= size - offset)
                        break;

            }

          //  fprintf(stderr, "ofsset is %d\n", offset);

            //close(out);

            current_info->DATAoffset += total_read;
            dictionary_set(stateDIC, &fd_DATA, current_info);
            free(buffer);

            if(bytesread == -1)
            {
                  if(errno != EAGAIN && errno != EWOULDBLOCK) //error exit
                  {
                        perror("read error");
                        printf("Closed connection on descriptor with error\n");
                        
                        return -1;
                  }
                  
                  if(current_info->DATAoffset > (int)size)
                        return -1;
                  
                  //current_info->DATAoffset += total_read;
                  dictionary_set(stateDIC, &fd_DATA, current_info);
                  return 0;
            }
            else
            {
                  if(current_info->DATAoffset < (int)size)
                        {
                              print_too_little_data();
                              print_error_message(err_bad_file_size1);
                              return -2;
                        }
                  else
                  {
                        //char tmmp;
                  if(current_info->DATAoffset > (int)size)
                  {
                        print_recieved_too_much_data();
                        print_error_message(err_bad_file_size1);
                        return -3;
                  }

                  current_info->state = -1;
                 // fprintf(stderr, "finnal offset%d\n", current_info->DATAoffset);
                  //current_info->DATAoffset += total_read;
                  dictionary_set(stateDIC, &fd_DATA, current_info);
                  return 1;
                  
                  }


            }





}

int writeSIZE(int fd_SIZE){

                  info* current_info = dictionary_get(stateDIC,&fd_SIZE);
       
            int byteswrite;
            int total_write = 0;
           

              while( (byteswrite = write(fd_SIZE,((char*)&current_info->sizeBuffer)+current_info->SIZEoffset+total_write, sizeof(size_t) - current_info->SIZEoffset - total_write) ) > 0 )
              {

                  if(byteswrite == -1)
                  {
                        if(errno == EINTR)
                                     continue;

                        else if (errno == EPIPE)
                              return -1;
                        
                        else
                                    break;
                                     
                  }
                  else if(byteswrite == 0)
                        {
                              break;
                        }
   
                        total_write += byteswrite;
                        if(total_write == (sizeof(size_t)-current_info->SIZEoffset))
                        break;

              }
            
              current_info->SIZEoffset += total_write;
            
            if(total_write == -1)
            {
                  if(errno != EAGAIN && errno != EWOULDBLOCK) //error exit
                        {
                        perror("write error");
                        printf("Closed connection on descriptor with error\n");
                        
                        //return -1;
                  
                  fprintf(stderr, "ero is %d\n", errno);
                  return -1;
                  }
                  dictionary_set(stateDIC, &fd_SIZE, current_info);
                  return 0;
            }
    

           // fprintf(stderr, "byteswrite%d\n", byteswrite);
            if(current_info->SIZEoffset == sizeof(size_t))
            {
                  current_info->state = -1;
                  current_info->SIZEoffset = 8;
                  dictionary_set(stateDIC, &fd_SIZE, current_info);
                  return 1;
            }
            else
            {
                  if(byteswrite == 0 && current_info->SIZEoffset!= sizeof(size_t))
                  {
                        perror("not enought size data");
                        return -1;
                  }
                  dictionary_set(stateDIC, &fd_SIZE, current_info);
                  return 0;
            }

      }


int writeDATA(int fd_DATA)
{
      ///*
                  info* current_info = dictionary_get(stateDIC,&fd_DATA);

                  int readFile = current_info->fdToWrite;
                  
            int byteswrite = 0;
            size_t total_write = 0;
            //int startwrite = 0;
            
            if(current_info->dataBuffer == NULL)
            {
            current_info->dataBuffer = calloc(1, 4096);
            current_info->DATAoffset = 0;
            }
            size_t size = current_info->sizeBuffer;
            //fprintf(stderr, "size is %zu\n", size);
            int offset = current_info->DATAoffset;
           // fprintf(stderr, "offset is %d\n", offset);
            //int position = lseek(fd_DATA, 0, SEEK_CUR);
            //fprintf(stderr, "positno is %d\n", position);
            int tRead = 0;
           
            while(1)
            {
            
            if((size_t)offset == size)
                        break;
            
            tRead = read_all_from_socket_server(readFile,current_info->dataBuffer, 4096);

            if(tRead < 0)
              return -1;
                        //fprintf(stderr, "tRead is %d\n", tRead);
            byteswrite = write_all_to_socket(fd_DATA,current_info->dataBuffer, tRead); 
            if(byteswrite == -1)
                  {
                        //fprintf(stderr, "byteswrite eroor trgured \n");
                        if(errno == EINTR)
                        {
                                    //fprintf(stderr, "tRead in error %d\n", tRead);
                                     continue;
                        }

                        else if (errno == EPIPE)
                                    return -1;

                        else
                                    continue;
                                     
                  }
                  else if(byteswrite == 0)
                        {
                              break;
                        }
   
                        total_write += byteswrite;
                        offset+= byteswrite;
                        //fprintf(stderr, "offset is %d\n", offset);

            }

            current_info->DATAoffset = offset;
            dictionary_set(stateDIC, &fd_DATA, current_info);
            //fprintf(stderr, "offset is being %d\n", offset);
              // total_read++;
            if(byteswrite == -1)
            {
                  if(errno != EAGAIN && errno != EWOULDBLOCK) //error exit
                  {
                        perror("read error");
                        printf("Closed connection on descriptor with error\n");
                        
                        return -1;
                  }
                  
                  if(current_info->DATAoffset > (int)size)
                        return -1;
                  
                  //current_info->DATAoffset += total_read;
                  dictionary_set(stateDIC, &fd_DATA, current_info);
                  return 0;
            }
            else
            {
                //  fprintf(stderr, "set state -1 1343\n");

                  if(current_info->DATAoffset < (int)size)
                        return -1;

                  current_info->state = -1;
                  
                  //current_info->DATAoffset += total_read;
                  dictionary_set(stateDIC, &fd_DATA, current_info);
                  return 1;
            }

            return 1;
     
}      

//new

