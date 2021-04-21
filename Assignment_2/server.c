// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>

#define PORT 8086

int setup_server(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    char buffer[102] = {0};
    char *hello = "Hello from server";
    if(argc==1){
        int client_socket = setup_server();
        char child_sock_fd[25];
        sprintf(child_sock_fd, "%d", client_socket);
        pid_t process_id=fork();
        if(process_id==0)
        {
            char * child_args[3];
            child_args[0] = (char *)argv[0];
            child_args[1] = child_sock_fd;
            child_args[2] = NULL;
            execvp(argv[0], child_args);
        }
        int status_var=0;
        wait(&status_var);
    }else{
        int new_socket = 0, valread;
        uid_t nobody_user = 65534;
        sscanf(argv[1], "%d", &new_socket);
        setuid(nobody_user);
        printf("child id of nobodyuser %d \n",getuid());
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
    }
    
    
    return 0;
}
