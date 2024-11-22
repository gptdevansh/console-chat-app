#include <iostream>
#include <winSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

// initialize winsock library 
// create shocket
// get ip and port 
// bind the ip/ shochet 
// listening
// accept (blocking call)(jb tak client ki request nhi ati tab tak ruka rahega)
// recv and send
// close the socket 
// close / clean winsock

bool Initialize() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

int main() {

    if (!Initialize()) {
        cerr << "winsock initialize failed" << endl;
        return 1;
    }

    // AF_INET : IPV4, SOCK_STREAM : TCP 
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (listenSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        return 1;
    }

    // create address structure
    int port = 353535;
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port); // host to network short

    //convert to ipaddress to binary ()
    if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
        cerr << "Setting address structure failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    //bind ip with socket
    if (bind( listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr) ) == SOCKET_ERROR) {
        cerr << "bind failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    //listen
    if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
        cerr << "listen failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server start to listening on port : " << port << endl;

    //accept
    SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "invalid client socket ." << endl;
    }

    char buffer[4096];
    int byteRecvd = recv(clientSocket, buffer, sizeof(buffer), 0);

    string message(buffer, byteRecvd);
    cout << "Message from client : " << message << endl;

    closesocket(clientSocket);
    closesocket(listenSocket);

    WSACleanup();
    return 0;
}