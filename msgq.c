#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "zemaphore.h"
#include "msgq.h"

struct __zem_t *s;

msgq* msgq_init(int nums_msgq){
    msgq *new=malloc(sizeof(msgq));
    new->msgHead=NULL;
    new->msgTail=NULL; 
    new->max=(int *)nums_msgq;   
    zem_init((struct __zem_t *)&s,nums_msgq);
    return new;
}


int msgq_send(msgq *mq, char *msg){
    msgNode *in=malloc(sizeof(msgNode));
    in->msg=malloc(sizeof(msg));
    in->next=NULL;
    strcpy(in->msg,msg);
    zem_wait((struct __zem_t *)&s);
    if (mq->msgTail==NULL){
        mq->msgHead=in;
        mq->msgTail=in;
    } else {
        mq->msgTail->next=in;
        mq->msgTail=in;
        return 1;
    }
    return -1; 
}

char *msgq_recv(msgq *mq){
    while (&s->value>=mq->max){
        //do nothing
    }
    char *ret = mq->msgHead->msg;
    mq->msgHead=mq->msgHead->next;
    zem_post((struct __zem_t *)&s);
    return ret;
}

int msgq_len(msgq *mq){
    int i=0;
    msgNode *curr=mq->msgHead;
    if (curr==NULL){
        return 0;
    }
    while (curr->next!=NULL){
        i+=1;
        curr=curr->next;
    }
    return i+1;
}

void msgq_show(msgq *mq){
    msgNode *curr=mq->msgHead;
    while (curr->next!=NULL){
        printf("%s\n",curr->msg);
        curr=curr->next;
    }
    printf("%s\n",curr->msg);
}
