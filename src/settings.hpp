#ifndef SETTINGS_HPP
# define SETTINGS_HPP

# include <netdb.h>
# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <unistd.h>
# include <fstream>

class Settings {
private:
    bool saveFlag;
    struct addrinfo *address;
    std::string request;
    char *caster;
    char *port;
    char *mountpoint;
    int socketfd;
    std::ofstream file;

    void setSaveFlag(int argc, char **argv);
    void openFile();
    void setRequest();
    void setCasterAndPort(int argc, char **argv);
    void setMountpoint(int argc, char **argv);
    void setAddress();
public:
    Settings(int argc, char **argv);
    ~Settings();

    void closeFile();
    void writeToOutput(char *buffer);
    bool getFlag();
    std::string getRequest();
    struct addrinfo getAddress();
    int connectToAddress();
    char *getCaster();
};

#endif