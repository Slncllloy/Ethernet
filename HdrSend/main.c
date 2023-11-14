#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <netinet/ip.h>
//#include <netinet/tcp.h>

struct udphdr 
{
    __be16   source;
    __be16   dest;
    __be16   len;
    __sum16  check;
};

struct tcphdr 
{
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
    uint8_t  res1;
    uint8_t  doff;
    uint8_t  fin:1,
             syn:1,
             rst:1,
             psh:1,
             ack:1,
             urg:1,
             res2:2;
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
};


struct udphdr InitUdpHdr()
{
	struct udphdr udp = {
		.source = 	htons(3490),
		.dest = 	htons(80),
		.len = 		htons(sizeof(struct udphdr)),
		.check = 	0,
	};
	return udp;
}

struct tcphdr InitTcpHdr()
{
	struct tcphdr tcp = {
		.source = 	htons(3490),
		.dest = 	htons(80),
		.seq = 		10,
		.ack_seq = 	0,
		.res1 = 	0,
		.doff = 	5,
		.window = 	htons(5840),
		.check = 	0,
		.urg_ptr =	0,
		
	};
	return tcp;
}

struct iphdr InitIpHdr(int data_length)
{
	struct iphdr ip = {
		.check = 		0,
		.daddr = 		inet_addr("127.0.0.1"),
		.frag_off = 	0,
		.id = 			htons(54321),
		.ihl = 			5,
		.protocol = 	IPPROTO_UDP,//IPPROTO_TCP,
		.saddr = 		inet_addr("192.168.0.100"),
		.tos = 			0,
		.tot_len = 		htons(sizeof(struct iphdr) + data_length),
		.ttl = 			255,
		.version = 		4,
	};

	return ip;
}

struct ethhdr InitEthHdr()
{
	struct ethhdr eh = {
		.h_dest[0] = 0x00,
		.h_dest[1] = 0x00,
		.h_dest[2] = 0x00,
		.h_dest[3] = 0x00,
		.h_dest[4] = 0x00,
		.h_dest[5] = 0x00,

		.h_source[0] = 0x4c,
		.h_source[1] = 0x03,
		.h_source[2] = 0x4f,
		.h_source[3] = 0x15,
		.h_source[4] = 0x6c,
		.h_source[5] = 0xf1,

		.h_proto =htons(ETH_P_IP)
	};
	return eh;
}
struct sockaddr_ll InitSockaddr_ll()
{
	struct sockaddr_ll sa = {
		.sll_ifindex = if_nametoindex("wlp0s20f3"),
		.sll_protocol = htons(ETH_P_ALL),
		.sll_family = PF_PACKET,
	};
	return sa;
}


int main(int argc, char *argv[])
{
	char message[] = "Hello world";
	int size_message = sizeof(message)/sizeof(message[0]);

    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    struct ethhdr eth = InitEthHdr();
	struct iphdr ip = InitIpHdr(size_message);
	struct tcphdr tcp = InitTcpHdr();
	struct udphdr udp = InitUdpHdr();
    struct sockaddr_ll sa = InitSockaddr_ll();

    char sendbuf[ETH_FRAME_LEN];
    memset(sendbuf, 0, ETH_FRAME_LEN);

    memcpy(sendbuf, &eth, sizeof(struct ethhdr));
	memcpy(sendbuf + sizeof(struct ethhdr), &ip, sizeof(struct iphdr));
	memcpy(sendbuf + sizeof(struct ethhdr) + sizeof(struct iphdr), &udp, sizeof(struct udphdr));
    memcpy(sendbuf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr), message, size_message);
	// memcpy(sendbuf + sizeof(struct ethhdr) + sizeof(struct iphdr), &tcp, sizeof(struct tcphdr));
    // memcpy(sendbuf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct tcphdr), message, size_message);

    if (sendto(sockfd, 
				sendbuf, 
				sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + size_message, 
				0, 
				(struct sockaddr*)&sa, 
				sizeof(struct sockaddr_ll)) 
	== -1) 
	{
        perror("sendto() failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    return 0;
}