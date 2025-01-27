#pragma once

#include"../vHeap.h"
#ifndef nHeap_H
#define nHeap_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCKAPI_
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "nHeapConfig.h"

namespace vhp {
	namespace v120 {
		namespace nhp {
			void init_nhp_runtime();

			enum nhpMode
			{
				n_server,
				n_client,
				n_standalone,
				n_modeError = -1
			};
			enum nhpPerms
			{
				n_RW,
				n_RO,
				n_WO,
				n_NA,
				n_permError = -1
			};

			struct Packet {
				uint32_t packetUID;
				uint32_t packetId;
				uint32_t totalPackets;
				uint32_t size = 0;
				uint8_t packetData[MAX_PACKET_SIZE];
				void clearData();
			};
			
			struct expectedPacket {
				bool packetReady = true;
				uint32_t packetUID = 0x0000;
				uint32_t packetId = 0;
				uint32_t totalPackets = 0;
				std::vector<uint8_t> data;
				std::vector<uint8_t> Readydata;
			};

			struct ClientInfo {
				std::string ip;
				uint16_t port;
				sockaddr_in sockAddr;
				std::chrono::steady_clock::time_point lastResponse = std::chrono::high_resolution_clock::now();
				bool invalidated = false;
				bool operator==(const ClientInfo& other) const {
					return ip == other.ip && port == other.port && invalidated == other.invalidated;
				}
			};

			class nHeap_RuntimeClass : public vHeap_RuntimeClass {
			private:
				// common
				WSADATA wsaData;
				nhpMode mode = n_modeError;
				uint32_t findClient(ClientInfo cl);

				// server
				SOCKET serverSocket;
				std::vector<ClientInfo> clients;
				std::vector<expectedPacket> clData;
				void serverThread();
				void clientHandler(Packet pk,ClientInfo cl);
				void clientThread(uint32_t clIndex);
				bool isClientAlreadyConnected(ClientInfo clInfo);

				// client
				SOCKET udpSocket;
				sockaddr_in serverAddr;
				expectedPacket svData;
				void serverListenThread();
				void serverHandler(Packet pk);

			public:
				//server
				bool initServer(uint16_t port);
				void stopServer();
				void dropClient(ClientInfo cl);
				void sendToClient(ClientInfo cl, uint8_t* data, size_t size);

				//client
				bool initClient(std::string ip, uint16_t port);
				void disconnect();
				void sendToServer(uint8_t* data, size_t size);
			};
			extern nHeap_RuntimeClass* _nhp;
		}

	}
}

#endif // !nHeap_H