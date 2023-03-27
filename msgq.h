
typedef struct msgq{
    struct msgNode *msgHead;
    struct msgNode *msgTail;
    int *max;
}msgq;

typedef struct msgNode{
    char *msg;
    struct msgNode *next;
}msgNode;


msgq* msgq_init(int nums_msgq);
int msgq_send(msgq *mq,char *msg);
char *msgq_recv(msgq *mq);
int msgq_len(msgq *mq);
void msgq_show(msgq *mq);