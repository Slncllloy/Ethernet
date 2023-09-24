/********************************
Last Modified		Description
-------------		---------------
10_07_23			Initial development

********************************/


#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

typedef struct MessageInfo
{
    /*
        Struct of getting data from recv
    */
    struct ethhdr* hdrEthernetGettingPacket;
    struct iphdr * hdrIpGettingPacket;
    struct udphdr* hdrUdpGettingPacket;

    unsigned char* BufferGettingPacket;
    int LengthBufferGettingPacket;

    short Resault;

}MessageInfo,ReceivedDataInfo;

extern MessageInfo InitReceivedDataInfo();
extern short ReleaseRecv();

//in debugging
extern short CreateRecv();
