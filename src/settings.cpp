#include "settings.hpp"

Settings::Settings(int argc, char **argv) {
    setSaveFlag(argc, argv);
    setCasterAndPort(argc, argv);
    setAddress();
    setMountpoint(argc, argv);
    setRequest();
    openFile();
}

Settings::~Settings() {
    close(socketfd);
    closeFile();
}

void Settings::setCasterAndPort(int argc, char **argv) {
    int pos = 1;
    if (saveFlag) {
        pos++;
    }
    if (argc >= pos + 1) {
        std::string *tmp = new std::string(argv[pos]);
        unsigned long posOfDel = tmp->find(':', 0);
        caster = strdup(tmp->substr(0, posOfDel).c_str());
        port = strdup(tmp->substr(posOfDel + 1, tmp->length()).c_str());

    } else {
        std::cout << "Error: not enough arguments!" << std::endl;
        exit(1);
    }
}

void Settings::setSaveFlag(int argc, char **argv) {
    if (argc >= 2) {
        if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-S") == 0) {
            saveFlag = true;
        } else {
            saveFlag = false;
        }
    } else {
        std::cout << "Error: not enough arguments!" << std::endl;
        exit(1);
    }
}

void Settings::setRequest() {
    if (strcmp(mountpoint, "") != 0) {
        request = "GET /" + std::string(mountpoint) + " HTTP/1.1\r\nContent-Type: */*\r\n\r\n";
    } else {
        request = "GET / HTTP/1.1\r\nContent-Type: */*\r\n\r\n";
    }
}

void Settings::setAddress() {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int err = getaddrinfo(caster, port, &hints, &address);
    if (err != 0) {
        std::cout << "Error: " << gai_strerror(err) << std::endl;
        exit(1);
    }
}

void Settings::setMountpoint(int argc, char **argv) {
    int pos = 2;
    if (saveFlag) {
        pos++;
    }
    if (argc == pos + 1) {
        mountpoint = argv[pos];
    } else {
        mountpoint = (char *)"\0";
    }
}

std::string Settings::getRequest() {
    return request;
}

struct addrinfo Settings::getAddress() {
    return *address;
}

int Settings::connectToAddress() {
    struct addrinfo *tmp;
    for (tmp = address; tmp != nullptr; tmp = tmp->ai_next) {
        int sock = socket(tmp->ai_family, tmp->ai_socktype, 0);
        if (sock == -1) {
            continue;
        }
        if (connect(sock, tmp->ai_addr, tmp->ai_addrlen) != -1) {
            return sock;
        }
        close(sock);
    }
    return -1;
}

bool Settings::getFlag() {
    return saveFlag;
}

char *Settings::getCaster() {
    return caster;
}

void Settings::openFile() {
    if (saveFlag) {
        file.open(caster, std::ios::trunc);
    }
}

void Settings::closeFile() {
    if (file.is_open()) {
        file.close();
    }
}

void Settings::writeToOutput(char *buffer) {
    if (saveFlag) {
        file << buffer;
    } else {
        std::cout << buffer;
    }
}