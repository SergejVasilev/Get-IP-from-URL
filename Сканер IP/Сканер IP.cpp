#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib") 
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::cerr;

// Функция для отображения IP-адреса
void display_ip(struct sockaddr* addr)
{
    struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
    unsigned char* ip = (unsigned char*)&(addr_in->sin_addr);
    printf("ip: %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
}

int main()
{
    WSADATA wsa_data;
    unsigned short wsa_version = MAKEWORD(2, 2);
    // Инициализация Winsock
    if (WSAStartup(wsa_version, &wsa_data) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    struct addrinfo sourse_info;
    memset(&sourse_info, 0, sizeof(struct addrinfo));
    sourse_info.ai_family = AF_INET;       // IPv4
    sourse_info.ai_socktype = SOCK_STREAM; // TCP
    sourse_info.ai_protocol = IPPROTO_TCP; // Протокол TCP
    struct addrinfo* addr_info = nullptr;

    cout << "Enter URL to get domain IP" << endl; 

    string url = "";
    cin >> url;

    string prefix_1 = "https://www.";
    // Удаление префикса "https://www."
    if (url.compare(0, prefix_1.length(), prefix_1) == 0)
        url = url.substr(prefix_1.length());

    string prefix_2 = "https://";
    // Удаление префикса "https://"
    if (url.compare(0, prefix_2.length(), prefix_2) == 0)
        url = url.substr(prefix_2.length());

    string prefix_3 = "http://";
    // Удаление префикса "http://"
    if (url.compare(0, prefix_3.length(), prefix_3) == 0)
        url = url.substr(prefix_3.length());

    string prefix_4 = "http://www.";
    // Удаление префикса "http://www."
    if (url.compare(0, prefix_4.length(), prefix_4) == 0)
        url = url.substr(prefix_4.length());

    string prefix_5 = "www.";
    // Удаление префикса "www."
    if (url.compare(0, prefix_5.length(), prefix_5) == 0)
        url = url.substr(prefix_5.length());

    // Проверка, является ли последний символ '/'
    if (!url.empty() && url.back() == '/') {
        // Удаление последнего символа
        url.pop_back();
    }

    cout << endl << url << endl; // Вывод измененного URL

    // Получение информации о хосте
    if (getaddrinfo(url.c_str(), "8888", &sourse_info, &addr_info) != 0) {
        cerr << "getaddrinfo failed" << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr* remote_addr = addr_info->ai_addr;

    while (addr_info != nullptr)
    {
        // Отображение IP-адреса
        display_ip(remote_addr);
        addr_info = addr_info->ai_next;
        if (addr_info != nullptr) {
            remote_addr = addr_info->ai_addr;
        }
    }

    // Освобождение памяти, выделенной для addrinfo
    freeaddrinfo(addr_info);
    // Завершение работы с Winsock
    WSACleanup();

    return 0;
}
