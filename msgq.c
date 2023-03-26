#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "zemaphore.h"
#include "msgq.h"

struct __zem_t *s;

struct msgq* msgq_init(int nums_msgq){
    struct msgq *new=malloc(sizeof(struct msgq));
    new->msgHead=NULL;
    new->msgTail=NULL;    
    zem_init(s,nums_msgq);
    return new;
}


int msgq_send(struct msgq *mq, char *msg){
    struct msgNode *in=malloc(sizeof(struct msgNode));
    in->msg=malloc(sizeof(msg));
    strcpy(in->msg,msg);
    if (mq->msgTail==NULL){
        zem_wait(s);
        mq->msgHead=in;
        mq->msgTail=in;
    } else {
        zem_wait(s);
        mq->msgTail->next=in;
        mq->msgTail=in;
        return 1;
    }
    return -1; 
}

char *msgq_recv(struct msgq *mq){
    char *ret = mq->msgHead->msg;
    if (mq->msgHead->next==NULL){
        zem_post(s);
    } else {
    mq->msgHead=mq->msgHead->next;
    zem_post(s);
    }
    return ret;
}

int msgq_len(struct msgq *mq){
    return s->value;
}

void msgq_show(struct msgq *mq){
    while (mq->msgHead!=NULL){
        printf("%s\n",msgq_recv(mq));
    }
}
