/**
 * Networking
 * CS 241 - Spring 2017
 */
#include "common.h"
#include "format.h"
#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <errno.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <signal.h>


static volatile int serverSocket;
struct addrinfo hints, *result;
//File* output_file;

char **parse_args(int argc, char **argv);
verb check_args(char **args);


int connect_to_server(const char *host, const char *port);




int main(int argc, char **argv) {
    // Good luck!
      signal(SIGPIPE, SIG_IGN);


    char **args = parse_args(argc,argv);
    verb command = check_args(args);
    
    serverSocket = connect_to_server(args[0], args[1]);
    
    if(command == LIST)
    {
        //write_all_to_socket(serverSocket, (char*)&command, sizeof(verb));
        char* t = "LIST\n";
        write_all_to_socket(serverSocket, t, 5);
        shutdown(serverSocket,SHUT_WR);
        
        ssize_t retval = 1;
        //char *buffer = NULL;
        char buffer[4];
        read_all_from_socket(serverSocket, buffer, 3);
        buffer[3] = '\0';
        //write_all_to_socket(1, buf, retval);
        if(strcmp(buffer,"OK\n")==0)
        {
            //fprintf(stdout, "STATUS_OK\n");
            retval = get_message_size(serverSocket);
            if(retval>0)
            {
                char buf[retval];
                
                //fprintf(stderr, "%zd\n", retval);
                if (retval > 0) {
                    
                    
                    ssize_t actual = read_all_from_socket(serverSocket, buf, retval);
                    if(actual == retval)
                    {
                        char hu[3];
                        if(!read_all_from_socket(serverSocket, hu, 1))
                            retval = actual;
                        else
                        {
                            retval = -1;
                            print_recieved_too_much_data();
                        }
                        
                    }
                    else
                    {
                        if(actual < retval)
                            print_too_little_data();
                        else
                            print_recieved_too_much_data();
                        retval = -1;
                    }
                    
                }
                if (retval > 0)
                {
                    write_all_to_socket(1, buf, retval);
                }
                
            }
            
        }
        else
        {
            char buf;
            int bytesread;
            
            int startwrite = 0;
            while ((bytesread = read(serverSocket, &buf, 1)) > 0 ) {
                if(startwrite)
                    write(1,&buf,1);
                if(buf == '\n')
                {
                    if(startwrite)
                        break;
                    startwrite = !startwrite;
                    
                }
            }
        }
        
        
        
        //fprintf(stderr, "getin\n");
        return 0;
        
    }
    else if(command == DELETE)
    {
        char* t = "DELETE ";
        write_all_to_socket(serverSocket, t, strlen(t));
        write_all_to_socket(serverSocket, args[3], strlen(args[3]));
        char* tt = "\n";
        write_all_to_socket(serverSocket, tt, 1);
        
        
        shutdown(serverSocket,SHUT_WR);
        char buffer[4];
        read_all_from_socket(serverSocket, buffer, 3);
        buffer[3] = '\0';
        if(strcmp(buffer,"OK\n")==0)
        {
            print_success();
        }
        else
        {
            char buf;
            int bytesread;
            //  int count = 0;
            // read one byte at a time.
            //fprintf(stderr, "the pipe it read%d\n", i);
            int startwrite = 0;
            while ((bytesread = read(serverSocket, &buf, 1)) > 0 ) {
                if(startwrite)
                    write(1,&buf,1);
                if(buf == '\n')
                {
                    if(startwrite)
                        break;
                    startwrite = !startwrite;
                    
                }
            }
            
        }
        
        return 0;
    }
    else if(command == GET)
    {
        char* t = "GET ";
        write_all_to_socket(serverSocket, t, strlen(t));
        write_all_to_socket(serverSocket, args[3], strlen(args[3]));
        char* tt = "\n";
        write_all_to_socket(serverSocket, tt, 1);
        
        
        shutdown(serverSocket,SHUT_WR);
        char buffer[4];
        read_all_from_socket(serverSocket, buffer, 3);
        //sleep(2);
        buffer[3] = '\0';
        if(strcmp(buffer,"OK\n")==0)
        {
            //fprintf(stdout, "STATUS_OK\n");
            ssize_t retval = 1;
            
            
            retval = get_message_size(serverSocket);
            if(retval>0)
            {
                char* buf = calloc(1,retval);
                
                //fprintf(stderr, "%zd\n", retval);
                if (retval > 0) {
                    
                   // sleep(1);
                    ssize_t actual = read_all_from_socket(serverSocket, buf, retval);
                    if(actual == retval)
                    {
                        char hu[3];
                        if(!read_all_from_socket(serverSocket, hu, 1))
                            retval = actual;
                        else
                        {
                            retval = -1;
                            print_recieved_too_much_data();
                        }
                        
                    }
                    else
                    {
                        if(actual < retval)
                            print_too_little_data();
                        else
                            print_recieved_too_much_data();
                        retval = -1;
                    }
                }
                if (retval > 0)
                {
                    
                    
                    
                    int out = open(args[4],O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR| S_IWUSR|S_IRWXO|S_IXUSR|S_IRWXG);
                    if (out == -1) {
                        perror("create_file() failed to open output file");
                        shutdown(serverSocket,SHUT_RDWR);
                        exit(1);
                    }
                    write_all_to_socket(out, buf, retval);
                    close(out);
                    return 0;
                }
                
                free(buf);
                
            }
        }
        else
        {
            
            char buf;
            int bytesread;
            
            int startwrite = 0;
            while ((bytesread = read(serverSocket, &buf, 1)) > 0 ) {
                if(startwrite)
                    write(1,&buf,1);
                if(buf == '\n')
                {
                    if(startwrite)
                        break;
                    startwrite = !startwrite;
                    
                }
            }
            
            
        }
    }
    else if(command == PUT)
    {
        int inputfd =  open(args[4],O_RDONLY);
        
        
        if(inputfd == -1)
        {
            perror("no such file");
            shutdown(serverSocket,SHUT_RDWR);
            exit(1);
        }
        char* t = "PUT ";
        //sleep(1);
        write_all_to_socket(serverSocket, t, strlen(t));
        // sleep(1);

        write_all_to_socket(serverSocket, args[3], strlen(args[3]));
           //     sleep(1);

        char* tt = "\n";
       //         sleep(1);

        write_all_to_socket(serverSocket, tt, 1);
        //sleep(1);
        size_t ssize = lseek(inputfd, 0, SEEK_END); // seek to end of file
        //size_t ssize = (size_t)ltell(inputfd); // get current file pointer
        //size_t ssize = 5;
        lseek(inputfd, 0, SEEK_SET); // seek back to beginning of file
        // fprintf(stderr, "start at beginning?%zu\n", m);
        //ssize = ssize-1;
        // fprintf(stderr, "fd is %d\n", inputfd);
        // size_t hsize = 999;
        
        //sleep(3);
        ssize_t retval = write_message_size(ssize, serverSocket);
        //fprintf(stderr, "retval is%zu\n", retval);
        
        char* tmbuf = calloc(1,ssize);
        if (retval > 0) {
            // fprintf(stderr, "size is %zu\n", ssize);
            //fprintf(stderr, "%zu\n",read(inputfd,tmbuf,ssize));
            retval = read_all_from_socket(inputfd,tmbuf,ssize);
            //           fprintf(stderr, "read is %zd\n", retval);
            if (retval > 0)
            {
                //sleep(4);
                write_all_to_socket(serverSocket, tmbuf, ssize);
                //sleep(5);
                // write_all_to_socket(serverSocket, tmbuf+(ssize - 3), 3);
            }
            
        }
        //sleep(2);
        free(tmbuf);
        
        shutdown(serverSocket,SHUT_WR);
        close(inputfd);
        char buffer[4];
        read_all_from_socket(serverSocket, buffer, 3);
        buffer[3] = '\0';
        if(strcmp(buffer,"OK\n")==0)
        {
            print_success();
        }
        else
        {
            char buf;
            int bytesread;
            
            int startwrite = 0;
            while ((bytesread = read(serverSocket, &buf, 1)) > 0 ) {
                if(startwrite)
                    write(1,&buf,1);
                if(buf == '\n')
                {
                    if(startwrite)
                        break;
                    startwrite = !startwrite;
                    
                }
            }
        }
        
        
    }
    free(args);
    
}

/**
 * Given commandline argc and argv, parses argv.
 *
 * argc argc from main()
 * argv argv from main()
 *
 * Returns char* array in form of {host, port, method, remote, local, NULL}
 * where `method` is ALL CAPS
 */
char **parse_args(int argc, char **argv) {
    if (argc < 3) {
        return NULL;
    }
    
    char *host = strtok(argv[1], ":");
    char *port = strtok(NULL, ":");
    if (port == NULL) {
        return NULL;
    }
    
    char **args = calloc(1, (argc + 1) * sizeof(char *));
    args[0] = host;
    args[1] = port;
    args[2] = argv[2];
    char *temp = args[2];
    while (*temp) {
        *temp = toupper((unsigned char)*temp);
        temp++;
    }
    if (argc > 3) {
        args[3] = argv[3];
    } else {
        args[3] = NULL;
    }
    if (argc > 4) {
        args[4] = argv[4];
    } else {
        args[4] = NULL;
    }
    
    return args;
}

/**
 * Validates args to program.  If `args` are not valid, help information for the
 * program is printed.
 *
 * args     arguments to parse
 *
 * Returns a verb which corresponds to the request method
 */
verb check_args(char **args) {
    if (args == NULL) {
        print_client_usage();
        exit(1);
    }
    
    char *command = args[2];
    
    if (strcmp(command, "LIST") == 0) {
        return LIST;
    }
    
    if (strcmp(command, "GET") == 0) {
        if (args[3] != NULL && args[4] != NULL) {
            return GET;
        }
        print_client_help();
        exit(1);
    }
    
    if (strcmp(command, "DELETE") == 0) {
        if (args[3] != NULL) {
            return DELETE;
        }
        print_client_help();
        exit(1);
    }
    
    if (strcmp(command, "PUT") == 0) {
        if (args[3] == NULL || args[4] == NULL) {
            print_client_help();
            exit(1);
        }
        return PUT;
    }
    
    // Not a valid Method
    print_client_help();
    exit(1);
}

int connect_to_server(const char *host, const char *port) {
    
    
    int s;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);//why not directly assining serverSocket
    
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; /* IPv4 only */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    
    s = getaddrinfo(host, port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }
    
    if(connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1){
        perror("connect");
        exit(1);
    }
    freeaddrinfo(result);
    
    //may freed too early
    
    return sock_fd;
}

