#include <iostream>
#include "settings.hpp"

int newLine = 0;

std::string getStatus(int sock) {
    char buffer[2];
    std::string response = "";
    memset(&buffer, 0, 2);
    while (buffer[0] != '\n') {
        read(sock, buffer, 1);
        response.append(buffer, 1);
    }
    return response;
}

void getBits(int sock, Settings *settings) {
    char buffer[2];
    memset(&buffer, 0, 2);
    char one[2] = "1";
    char zero[2] = "0";
    char nl[2] = "\n";
    while (read(sock, buffer, 1) > 0) {
        for (int i = 0; i < 8; i++) {
            if (newLine >= 72) {
                settings->writeToOutput(nl);
                newLine = 0;
            }
            if ((buffer[0] & 0b10000000) == 0b10000000) {
                settings->writeToOutput(one);
            } else {
                settings->writeToOutput(zero);
            }
            buffer[0] <<= 1;
            newLine++;
        }
    }
}

int main(int argc, char **argv) {
    Settings settings(argc, argv);
    int sock;
    while (true) {
        sock = settings.connectToAddress();
        write(sock, (const void *)settings.getRequest().c_str(), settings.getRequest().length());
        std::string status = getStatus(sock);
        if (status == "ICY 200 OK\r\n") {
            getBits(sock, &settings);
        } else {
            char buffer[4097];
            memset(&buffer, 0, 4097);
            while (read(sock, &buffer, 4096) != 0) {
                settings.writeToOutput(buffer);
                memset(&buffer, 0, 4096);
            }
            close(sock);
            settings.closeFile();
            return 0;
        }
        close(sock);
    }
}
