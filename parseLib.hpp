#ifndef PARSELIBAPI_HPP
#define PARSELIBAPI_HPP
#include <string>
#include <vector>

enum{
    authCode = 0,
    registerCode = 1,
    messageCode = 2,
    historyCode = 3,
    chatsCode = 4,
    createChatCode = 5,
    chatInfoCode = 6,
    errorCode = -1
};

std::string parseMessage(std::vector<std::string> s);

std::vector<std::string> getArgs(std::string s);

std::string getFirstArg(std::string s);

void requestHandler(
    std::pair<sf::TcpSocket *, int> * client,
    std::string request,
    Database &db,
    std::vector<std::pair<sf::TcpSocket *, int>> & clients);

int requestIdentify(std::string request);

#endif
