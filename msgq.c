#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "zemaphore.h"
#include "msgq.h"

struct __zem_t *full;
struct __zem_t *empty;

msgq* msgq_init(int nums_msgq){
    msgq *new=malloc(sizeof(msgq));
    new->msgHead=NULL;
    new->msgTail=NULL; 
    new->max=nums_msgq;
    new->size=0;
    zem_init((struct __zem_t *)&full,1);
    zem_init((struct __zem_t *)&empty,1);
    return new;
}


int msgq_send(msgq *mq, char *msg){
    while (mq->size>=4){
        //hol up
    }
    msgNode *in=malloc(sizeof(msgNode));
    in->msg=malloc(sizeof(msg));
    in->next=NULL;
    strcpy(in->msg,msg);
    zem_wait((struct __zem_t *)&full);
    if (mq->msgHead==NULL){
        mq->msgHead=in;
        mq->msgTail=in;
        mq->size++;
        zem_post((struct __zem_t *)&full);
        return 1;
    } else {
        mq->msgTail->next=in;
        mq->msgTail=in;
        mq->size++;
        zem_post((struct __zem_t *)&full);
        return 1;
    }
    return -1;
}

char *msgq_recv(msgq *mq){
    while (mq->size==0){
        //hol up
    }
    zem_wait((struct __zem_t *)&empty);
    char *ret = mq->msgHead->msg;
    mq->msgHead=mq->msgHead->next;
    mq->size--;
    zem_post((struct __zem_t *)&empty);
    return ret;
}


int msgq_len(msgq *mq){
    int i=0;
    msgNode *curr=mq->msgHead;
    while (curr!=NULL){
        i+=1;
        curr=curr->next;
    }
    return i;
}

void msgq_show(msgq *mq){
    if (mq->msgHead==NULL){
        printf("Empty\n");
    } else {
        msgNode *curr=mq->msgHead;
        while (curr!=NULL){
            printf("%s\n",curr->msg);
            curr=curr->next;
        }
    }
}
