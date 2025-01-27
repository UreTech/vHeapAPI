#include "nHeap.h"


vhp::v120::nhp::nHeap_RuntimeClass* vhp::v120::nhp::_nhp;

void vhp::v120::nhp::init_nhp_runtime()
{
	_nhp = new nHeap_RuntimeClass();
}

uint32_t vhp::v120::nhp::nHeap_RuntimeClass::findClient(ClientInfo cl)
{
    for (uint32_t i = 0; i < clients.size(); i++) {
        if (cl == clients.at(i)) {
            return i;
        }
    }
    return -1;
}

void vhp::v120::nhp::nHeap_RuntimeClass::serverThread()
{
    while (mode == n_server) {
        //LOG_INFO("loop.");

        sockaddr_in clientAddr{};
        int clientAddrSize = sizeof(clientAddr);
        Packet pk;
        int bytesReceived = recvfrom(serverSocket, (char*)&pk, sizeof(Packet), 0,
            (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            LOG_WARN("recvfrom failed! Error: " + std::to_string(WSAGetLastError()));
            continue;
        }


        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        uint16_t clientPort = ntohs(clientAddr.sin_port);

        ClientInfo client{ clientIP, clientPort };
        LOG_INFO("packet from: " << client.ip << ":" << client.port << "/packet ID: " + std::to_string(pk.packetId) + "/total packet: " + std::to_string(pk.totalPackets) + "/size of packet: " + std::to_string(pk.size));
        if (!isClientAlreadyConnected(client)) {
            client.sockAddr.sin_family = AF_INET;
            client.sockAddr.sin_port = htons(clientPort);
            client.sockAddr.sin_addr.s_addr = inet_addr(clientIP);
            clients.push_back(client);
            clData.push_back(nhp::expectedPacket());
            clientHandler(pk, client);
            LOG_INFO("New client connected: " << clientIP << ":" << std::to_string(clientPort));
            std::thread t(&nHeap_RuntimeClass::clientThread, this, clients.size() - 1);
            t.detach();
            LOG_INFO("New client thread started: " << clientIP << ":" << std::to_string(clientPort));
        }
        else {
            clientHandler(pk, client);
        }
    }
    closesocket(serverSocket);
    WSACleanup();
}

void vhp::v120::nhp::nHeap_RuntimeClass::clientHandler(Packet pk, ClientInfo cl)
{
    uint32_t index = findClient(cl);
    if (index != -1) {
        expectedPacket* exP = &clData.at(index);
        if (exP->packetReady) {
            exP->packetReady = false;
            exP->packetUID = pk.packetUID;
            exP->packetId = 0;
            exP->totalPackets = pk.totalPackets;

            exP->data.resize(exP->data.size() + MAX_PACKET_SIZE);
            memcpy(exP->data.data(), pk.packetData, pk.size);
            exP->packetId++;
        }
        else {
            if (exP->packetUID == pk.packetUID) {
                if (exP->packetId == pk.packetId) {
                    exP->data.resize(exP->data.size() + MAX_PACKET_SIZE);
                    memcpy(exP->data.data() + (pk.packetId * MAX_PACKET_SIZE), pk.packetData, pk.size);
                    exP->packetId++;
                }
            }
        }
        if (exP->packetId == exP->totalPackets) {
            exP->packetReady = true;
            exP->Readydata.resize(exP->data.size());
            memcpy(exP->Readydata.data(), exP->data.data(), exP->data.size());
            exP->data.clear();
            clients[index].lastResponse = std::chrono::high_resolution_clock::now();
            LOG_INFO("data:" << std::string((char*)exP->Readydata.data(), exP->Readydata.size()));
        }
    }
    else {
        LOG_WARN("Client removed!");
    }
}

void vhp::v120::nhp::nHeap_RuntimeClass::clientThread(uint32_t clIndex)
{
    while (1) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - clients[clIndex].lastResponse);
        if (duration.count() > TIMEOUT_VALUE_MS) {
            LOG_WARN("Client on: " << clients[clIndex].ip << ":" << std::to_string(clients[clIndex].port) << " CONNECTION TIMEOUT (DISCONNECTED).");
            dropClient(clients[clIndex]);
            break;
        }
        Sleep(100);
    }
}

bool vhp::v120::nhp::nHeap_RuntimeClass::isClientAlreadyConnected(ClientInfo clInfo)
{
    for (uint32_t i = 0; i < clients.size(); i++) {
        if (clInfo == clients.at(i)) {
            return true;
        }
    }
    return false;
}

void vhp::v120::nhp::nHeap_RuntimeClass::serverListenThread()
{
    while (1) {
        Packet receivePacket;
        int fromAddrSize = sizeof(serverAddr);
        int bytesReceived = recvfrom(udpSocket, (char*)&receivePacket, sizeof(Packet), 0, (sockaddr*)&serverAddr, &fromAddrSize);
        if (bytesReceived > 0) {
            serverHandler(receivePacket);
        }
    }
}

void vhp::v120::nhp::nHeap_RuntimeClass::serverHandler(Packet pk)
{
    expectedPacket* exP = &svData;
    if (exP->packetReady) {
        exP->packetReady = false;
        exP->packetUID = pk.packetUID;
        exP->packetId = 0;
        exP->totalPackets = pk.totalPackets;

        exP->data.resize(exP->data.size() + MAX_PACKET_SIZE);
        memcpy(exP->data.data(), pk.packetData, pk.size);
        exP->packetId++;
    }
    else {
        if (exP->packetUID == pk.packetUID) {
            if (exP->packetId == pk.packetId) {
                exP->data.resize(exP->data.size() + MAX_PACKET_SIZE);
                memcpy(exP->data.data() + (pk.packetId * MAX_PACKET_SIZE), pk.packetData, pk.size);
                exP->packetId++;
            }
        }
    }
    if (exP->packetId == exP->totalPackets) {
        exP->packetReady = true;
        exP->Readydata.resize(exP->data.size());
        memcpy(exP->Readydata.data(), exP->data.data(), exP->data.size());
        exP->data.clear();
        LOG_INFO("data:" << std::string((char*)exP->Readydata.data(), exP->Readydata.size()));
    }
}

bool vhp::v120::nhp::nHeap_RuntimeClass::initServer(uint16_t port)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        LOG_ERROR("WSAStartup failed!");
        return 0;
    }

    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        LOG_ERROR("Socket creation failed! Error: " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return 0;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        LOG_ERROR("Bind failed! Error: " + std::to_string(WSAGetLastError()));
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    LOG_INFO("UDP server is running on port " + std::to_string(port));
    mode = n_server;
    std::thread t(&nHeap_RuntimeClass::serverThread, this);
    t.detach();
    LOG_INFO("Server thread started.");
}

void vhp::v120::nhp::nHeap_RuntimeClass::stopServer()
{
    mode = n_modeError;
}

void vhp::v120::nhp::nHeap_RuntimeClass::dropClient(ClientInfo cl)
{
    uint32_t clIndex = findClient(cl);
    if (clIndex != -1) {
        if (!clients[clIndex].invalidated) {
            clients[clIndex].invalidated = true;
            LOG_INFO("Client " << clients[clIndex].ip << ":" << std::to_string(clients[clIndex].port) << " disconnected.");
        }
        else {
            LOG_ERROR("Client is invalidated!");
        }
    }
    else {
        LOG_ERROR("Client not found!");
    }
}

void vhp::v120::nhp::nHeap_RuntimeClass::sendToClient(ClientInfo cl, uint8_t* data, size_t size)
{
    uint32_t clIndex = findClient(cl);
    if (clIndex != -1) {
        if (!clients[clIndex].invalidated) {
            uint64_t packetCount = 0;
            while (MAX_PACKET_SIZE <= size - (MAX_PACKET_SIZE * packetCount)) {
                packetCount++;
            }
            packetCount++;

            Packet pk;
            pk.totalPackets = packetCount;
            uint32_t lastPacketSize = size % MAX_PACKET_SIZE;

            for (uint64_t i = 0; i < packetCount; i++) {
                pk.clearData();
                pk.packetId = i;
                if (i != packetCount - 1) {
                    pk.size = MAX_PACKET_SIZE;
                }
                else {
                    pk.size = lastPacketSize;
                }
                memcpy(pk.packetData, (data + (i * MAX_PACKET_SIZE)), pk.size);
                if (sendto(serverSocket, (char*)&pk, sizeof(Packet), 0, (sockaddr*)&clients[clIndex].sockAddr, sizeof(clients[clIndex].sockAddr)) == SOCKET_ERROR) {
                    LOG_WARN("sendto failed: " + std::to_string(WSAGetLastError()));
                }
            }
        }
        else {
            LOG_ERROR("Client is invalidated!");
        }
    }
    else {
        LOG_ERROR("Client not found!");
    }
}

bool vhp::v120::nhp::nHeap_RuntimeClass::initClient(std::string ip, uint16_t port)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        LOG_ERROR("WSAStartup failed!");
        return 0;
    }

    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        LOG_ERROR("Socket creation failed! Error: " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return 0;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    std::thread t(&nHeap_RuntimeClass::serverListenThread, this);
    t.detach();
    LOG_INFO("serverListen thread started.");
    return 1;
}

void vhp::v120::nhp::nHeap_RuntimeClass::disconnect()
{
    closesocket(udpSocket);
    WSACleanup();
    mode = n_modeError;
    LOG_INFO("Disconnected!");
}

void vhp::v120::nhp::nHeap_RuntimeClass::sendToServer(uint8_t* data, size_t size)
{
    uint64_t packetCount = 0;
    while (MAX_PACKET_SIZE <= size - (MAX_PACKET_SIZE * packetCount)) {
        packetCount++;
    }
    packetCount++;

    Packet pk;
    pk.totalPackets = packetCount;
    uint32_t lastPacketSize = size % MAX_PACKET_SIZE;

    for (uint64_t i = 0; i < packetCount; i++) {
        pk.clearData();
        pk.packetId = i;
        if (i != packetCount - 1) {
            pk.size = MAX_PACKET_SIZE;
        }
        else {
            pk.size = lastPacketSize;
        }
        memcpy(pk.packetData, (data + (i * MAX_PACKET_SIZE)), pk.size);
        if (sendto(udpSocket, (char*)&pk, sizeof(Packet), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            LOG_WARN("sendto failed: " + std::to_string(WSAGetLastError()));
        }
    }
}

void vhp::v120::nhp::Packet::clearData()
{
    memset(packetData, 0x00, MAX_PACKET_SIZE);
}
