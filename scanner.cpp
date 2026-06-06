#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Функция для проверки одного конкретного порта
bool check_port(const std::string& ip, int port) {
    // Создаем TCP-сокет
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    // Настраиваем структуру адреса
    sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    
    // Преобразуем IP из текста в бинарный формат
    if (inet_pton(AF_INET, ip.c_str(), &target.sin_addr) <= 0) {
        close(sock);
        return false;
    }

    // Ставим таймаут на подключение (чтобы не ждать вечно)
    struct timeval timeout;
    timeout.tv_sec = 1;  // 1 секунда на ожидание ответного сигнала
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));

    // Пробуем подключиться к порту
    int result = connect(sock, (struct sockaddr*)&target, sizeof(target));
    close(sock); // Обязательно закрываем сокет за собой

    return (result == 0); // Если результат 0 — порт открыт
}

int main() {
    std::cout << "=======================================" << std::endl;
    std::cout << "  🚀 C++ Наводчик: Сканер TCP-портов   " << std::endl;
    std::cout << "=======================================" << std::endl;

    std::string ip;
    std::cout << "[?] Введите IP-адрес для проверки: ";
    std::cin >> ip;

    // Список популярных портов для быстрой разведки
    std::vector<int> common_ports = {21, 22, 23, 25, 53, 80, 139, 443, 445, 8080};

    std::cout << "[*] Начинаем сканирование целей..." << std::endl;

    for (int port : common_ports) {
        if (check_port(ip, port)) {
            std::cout << "[+] Порт " << port << " [OPEN] 🔓" << std::endl;
        }
    }

    std::cout << "[*] Сканирование успешно завершено!" << std::endl;
    return 0;
}
