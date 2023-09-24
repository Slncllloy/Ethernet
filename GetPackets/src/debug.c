#include <string.h>         // strerror
#include <stdio.h>          // printf
#include <errno.h>          // error message
#include <netinet/ether.h>  // ethhdr
#include <netinet/ip.h>     // iphdr
#include <netinet/udp.h>    // udphdr
#include <arpa/inet.h>      // inet_ntoa
#include <stdbool.h>        // for bool

#define DEBUG true

//For all custom func error return is ERROR or SUCCESS
#define FAILED -35
#define SUCCESS 35

//Print error of func
inline static void DGetLastError(char* string)
{
    printf("%s %s\n",string,strerror(errno));
}

//Print of func ethernet information
inline static void DEthernetPrint(struct ethhdr* eth)
{
    printf("\t______________Ethernet Header________________\n");
    printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
    printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
    printf("\t|-Protocol : %d\n",eth->h_proto);
}

//Print of func ip information
inline static void DIpPrint(struct iphdr* ip)
{
    struct sockaddr_in source;
    struct sockaddr_in dest;

    memset(&source, 0, sizeof(source));
    memset(&dest, 0, sizeof(dest));

    source.sin_addr.s_addr = ip->saddr;
    dest.sin_addr.s_addr = ip->daddr;

    printf("\t____________________IP____________________\n");
    printf("\t|-Version : %d\n",(unsigned int)ip->version);
    printf("\t|-Internet Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
    printf("\t|-Type Of Service : %d\n",(unsigned int)ip->tos);
    printf("\t|-Total Length : %d Bytes\n",ntohs(ip->tot_len));
    printf("\t|-Identification : %d\n",ntohs(ip->id));
    printf("\t|-Time To Live : %d\n",(unsigned int)ip->ttl);
    printf("\t|-Protocol : %d\n",(unsigned int)ip->protocol);
    printf("\t|-Header Checksum : %d\n",ntohs(ip->check));
    printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));
    printf("\t|-Destination IP : %s\n",inet_ntoa(dest.sin_addr));
}

//Print of func udp information
inline static void DUdpPrint(struct udphdr* udp)
{
    printf("\t____________________UDP______________________\n");
    printf("\t|-Source Port : %d\n" , udp->source);
    printf("\t|-Destination Port : %d\n" , udp->dest);
    printf("\t|-UDP Length : %d\n" , ntohs(udp->len));
    printf("\t|-UDP Checksum : %d\n" , ntohs(udp->check));
}

//Print of receiving information
inline static void DDataPrint(unsigned char* buffer)
{ 
    printf("\n");

    printf("%s\n",buffer);

    printf("\n");
}
