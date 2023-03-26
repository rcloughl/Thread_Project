
struct msgq{
    struct msgNode *msgHead;
    struct msgNode *msgTail;
};

struct msgNode{
    char *msg;
    struct msgNode *next;
    struct msgNode *prev;
};


struct msgq* msgq_init(int nums_msgq);
int msgq_send(struct msgq *mq,char *msg);
char *msgq_recv(struct msgq *mq);
int msgq_len(struct msgq *mq);
void msgq_show(struct msgq *mq);