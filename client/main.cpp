#include<iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>

using namespace std;

// Initialize WinSock
// create socket
// connect to the server
// send/ recv
// close the socket

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}
int main() {
	string clientName;
	cout << "Enter your name : ";
	cin >> clientName;

	if (!Initialize()) {
		cerr << "WinSock Initialization failed !" << endl;
		return 1;
	}
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET) {
		cerr << "invalid socket created " << endl;
		return 1;
	}

	int port = 3535;
	string serveraddress = "127.0.0.1";

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

	if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		cerr << "Not able to connect server" << endl;
		closesocket(s);
		WSACleanup();
		return 1;
	}

	cout << "you are connected to the server " << endl;

	// send

	string message = "hello! I'm " + clientName;
	int bytesent;
	bytesent = send(s, message.c_str(), static_cast<int>(message.length()), 0);
	if (bytesent == SOCKET_ERROR) {
		cerr << "Send failed with error "<< WSAGetLastError() << endl;
	}

	closesocket(s);	
    WSACleanup();

	return 0;
}


