//
// Created by AJ Hermosillo on 3/4/25.
//
#include <arpa/inet.h>

struct DHCP_Packet {
    uint8_t op;             // Operation Code
    uint8_t htype;          // Hardware Type
    uint8_t hlen;           // Hardware Length
    uint8_t hops;           // Hop Count
    uint32_t xid;           // Transaction ID
    uint16_t secs;          // Seconds
    uint16_t flags;         // Flags
    uint32_t ciaddr;        // Client IP
    uint32_t yiaddr;        // Your IP
    uint32_t siaddr;        // Server IP
    uint32_t giaddr;        // Gateway IP
    uint8_t chaddr[16];     // Client Hardware Address (MAC ADDRESS)
    char sname[64];         // Server Name
    char file[128];         // Boot File
    uint8_t options[240];   // Options
};