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

#include "./DHCP_Flooder.cpp"



int main()
{
    srand(time(nullptr));

    const unsigned int numThreads = std::thread::hardware_concurrency() * 2;
    std::cout << "Threads: " << numThreads << "\n";

    size_t packetPoolSize = 1024;
    DHCP_Flooder* flooder = new DHCP_Flooder(packetPoolSize);

    // std::print(sizeof(DHCP_Packet));

    char option;

    do
    {
        std::print("Enter option: ");
        std::cin >> option;

        if (option == '1')
            flooder->start(1);
    } while (option != '0');



    delete flooder;

    return 0;
}
