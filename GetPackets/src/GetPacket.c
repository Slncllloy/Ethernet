/********************************
Last Modified		Description
-------------		---------------
10_07_23			Initial development

********************************/

#include <stdlib.h>

#include "debug.c"
#include "GetPacket.h"

//Max packet UDP/TCP
#define LEN_SEGMENT 65535

struct entry_recv
{
    /*
        struct for devide ferst initialise and while
    */
    int socket;
    unsigned char* buffer;
    struct sockaddr saddr;
    ssize_t saddr_len;
    int len_segment;
};
struct entry_recv s_recv;
int iphdrlen = 0;


struct ethhdr* init_recv_ethhdr(unsigned char* buffer)
{
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    return eth;
}
struct iphdr* init_recv_iphdr(unsigned char* buffer)
{
    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    iphdrlen = ip->ihl*4;
    return ip;
}
struct udphdr* init_recv_udphdr(unsigned char* buffer)
{
    struct udphdr *udp=(struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
    return udp;
}
short ReleaseRecv()
{
    s_recv.socket =         -1;
    s_recv.len_segment =    0;
    free(s_recv.buffer);
    s_recv.saddr_len =      -1;

    return SUCCESS;
}
MessageInfo InitReceivedDataInfo()
{
    int length=recvfrom(s_recv.socket,
                        s_recv.buffer,
                        s_recv.len_segment,
                        0,
                        &s_recv.saddr,
                        (socklen_t *)&s_recv.saddr_len);
    if(length < 0)
    {
        DGetLastError("GetPacket.c - recvfrom() :");

        MessageInfo message_fail = 
        {
            .BufferGettingPacket = 0x00,
            .hdrEthernetGettingPacket = 0x00,
            .hdrIpGettingPacket = 0x00,
            .hdrUdpGettingPacket = 0x00,
            .LengthBufferGettingPacket = -1,
            .Resault = FAILED,
        };

        return message_fail;
    }

    MessageInfo msg_info = 
    {
        .hdrEthernetGettingPacket =     init_recv_ethhdr(s_recv.buffer),
        .hdrIpGettingPacket =           init_recv_iphdr(s_recv.buffer),
        .hdrUdpGettingPacket =          init_recv_udphdr(s_recv.buffer),
        .BufferGettingPacket =          s_recv.buffer,
        .LengthBufferGettingPacket =    length,
        .Resault =                      SUCCESS,
    };

    return msg_info;
}
short CreateRecv()
{
    struct sockaddr saddr = {0};

    s_recv.socket =         socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    s_recv.buffer =         (unsigned char *) malloc(LEN_SEGMENT);
    s_recv.saddr =          saddr;
    s_recv.len_segment =    LEN_SEGMENT;

    if(s_recv.socket < 0)
    {
        DGetLastError("GetPacket.c - socket() :");
        return FAILED;
    }
    return SUCCESS;
}