#include "csapp.h"

void echo(int connfd);

/* Reap all terminated children to prevent zombies (non-blocking) */
void sigchld_handler(int sig)
{
    /* Reap any child that has finished without blocking the parent */
    while (waitpid(-1, 0, WNOHANG) > 0)
        ;
    return;
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Validate arguments: expect a single port number */
    if (argc != 2)
    {
        fprintf(stderr, "useage: %s <port>\n", argv[0]);
        exit(0);
    }

    /* Install SIGCHLD handler to reap terminated children (avoid zombies) */
    Signal(SIGCHLD, sigchld_handler);

    /* Create a listening socket bound to the given port (string form) */
    listenfd = Open_listenfd(argv[1]);

    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);

        /* Block waiting for the next incoming connection */
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        /* Fork a child to handle this client so the parent can keep accepting */
        if (Fork() == 0)
        {
            /* Child process: no need for the listening socket */
            Close(listenfd);

            /* Serve this client connection (echo until EOF) */
            echo(connfd);

            /* Close the connected socket and exit child cleanly */
            Close(connfd);
            exit(0);
        }

        /* Parent process: close connected socket (child owns it now) */
        Close(connfd);
    }
}