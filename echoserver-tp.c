#include "csapp.h"
#include "sbuf.h"

#define NTHREADS 4
#define SBUFSIZE 16

void echo_cnt(int connfd);
void *thread(void *vargp);

sbuf_t sbuf; /* Shared buffer of connected descriptors */

static int byte_cnt; /* Byte counter */
static sem_t mutex;  /* and the mutex that protects it */

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[1]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    Sem_init(&mutex, 0, 1);
    sbuf_init(&sbuf, SBUFSIZE);
    byte_cnt = 0;

    /* Create worker threads */
    for (int i = 0; i < NTHREADS; ++i)
        Pthread_create(&tid, NULL, thread, NULL);

    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }
}

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    while (1)
    {
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
        echo_cnt(connfd);                /* Service client */
        Close(connfd);
    }
}

void echo_cnt(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        P(&mutex);
        byte_cnt += n;
        printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
        V(&mutex);
        Rio_writen(connfd, buf, n);
    }
}