#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "zemaphore.h"
#include "msgq.h"

// SEE Labs/GdbLldbLab for more information on lldb - lowlevel debugger

struct msgq *mq;
char* arr[3][100];
int* producers=0;
//
// Main threads
//
char *messages[] = { "msg1", "msg2", "hellomsg", "gustymsg" };

// sends msgs in messages[]
void *promtAndSend(void *arg) {
    for (int i = 0; i < sizeof(messages)/sizeof(char*); i++) {
        char response[80];
        printf("Send? ");
        scanf("%s", response);
        if (response[0] == 'y' || response[0] == 'Y') {
            printf("sending: %s\n", messages[i]);
            msgq_send(mq, messages[i]);
        }
    }
    return NULL;
}

// consume messges in msgq
void *recvMsgs(void *arg) {
    sleep(5);
    int msg_count = msgq_len(mq);
    printf("mq msg_count: %d\n", msg_count);
    for (int i = 0; i < msg_count; i++) {
        char *m = msgq_recv(mq);
        printf("recvMsgs: %s\n", m);
        free(m);
    }
    return NULL;
}

void *passiton(void *arg) {
    int me = (int) arg;
    while (1) {
        sleep(1);
        printf("passiton%d initial msgq_len: %d\n", me, msgq_len(mq));
        char *m = msgq_recv(mq);
        printf("passiton%d: %p %p %s\n", me, &m, m, m);
        printf("passiton%d after recv msgq_len: %d\n", me, msgq_len(mq));
        msgq_send(mq, m);
        printf("passiton%d after send msgq_len: %d\n", me, msgq_len(mq));
        free(m);
    }
    return NULL;
}


///////////

void *producer(void *arg) {
    int id = (int) arg;
    int i;
    for (i = 0; i<50; i++) {
        sleep(5);
        char *m=messages[i%4];
        msgq_send(mq, (char *) m);
        printf("Message:%d m:%s l:%d after 5 seconds\n", id, m,mq->size);
    }
    printf("Sent all %d from %d\n", i, id);
    producers-=1;
    return 0;
}

void *consumer(void *arg) {
    int me = (int) arg;
    int pos=me-3;
    int i = 0;
    while (producers!=0) {
        sleep(1);
        char *m = msgq_recv(mq);
        printf("Received:%s\n",m);
        arr[pos][i]=strdup(m);
        i++;
    }
    arr[me-3][i]=NULL;
    return 0;
}

///////////


#define MSGQLEN 4

int main(int argc, char *argv[]) {
    pthread_t p1, p2, p3, p4, p5;
    mq = msgq_init(MSGQLEN);
    char test = '1';
    if (argc == 2)
        test = argv[1][0];
    switch (test) {
    case '1':
        printf("test fill and empty msgq\n");
        pthread_create(&p1, NULL, promtAndSend, NULL);
        pthread_join(p1, NULL);
        printf("msgq_show() after filling for test 1:\n");
        msgq_show(mq);
        pthread_create(&p2, NULL, recvMsgs, NULL);
        pthread_join(p2, NULL);
        printf("msgq_show() after all consumed by test 1:\n");
        msgq_show(mq);
        break;
    case '2':
        printf("test fill msgs and pass it on\n");
        pthread_create(&p1, NULL, promtAndSend, NULL);
        pthread_join(p1, NULL);
        printf("msgq_show() after filling for test 2:\n");
        msgq_show(mq);
        pthread_create(&p1, NULL, passiton, (void *)1);
        pthread_create(&p2, NULL, passiton, (void *)2);
        pthread_join(p1, NULL);
        pthread_join(p2, NULL);
        break;
    case '3':
        printf("Sending messages\n");
        producers+=2;
        pthread_create(&p1, NULL, producer, (void *)1);
        pthread_create(&p2, NULL, producer, (void *)2);
        pthread_create(&p3, NULL, consumer, (void *)3);
        pthread_create(&p4, NULL, consumer, (void *)4);
        pthread_create(&p5, NULL, consumer, (void *)5); 
        pthread_join(p1, NULL);
        pthread_join(p2, NULL);
        sleep(5);
        printf("msgq_show() after all consumed:\n");
        msgq_show(mq);
        printf("Here are the arrays:\n");
        int tot=0;
        int i=0;
        int j=0;
        while (i<3){
            printf("\nArray for Consumer %d:\n",i+3);
            while (arr[i][j]!=NULL){
                printf("%s ",arr[i][j]);
                j+=1;
            }
            printf("\n%d words in Consumer %d\n",j,i+3);
            tot+=j;
            j=0;
            i+=1;
        }
        printf("\n%d messages received\n",tot);
        break;
    default:
        printf("invalid test selection!\n");
        break;
    }
    return 0;
}

