#ifndef SOCKETS_HPP
#define SOCKETS_HPP

sf::Socket::Status send(sf::TcpSocket& sock, std::string s);

sf::Socket::Status receive(sf::TcpSocket& sock, std::string &s);

std::vector<std::string> getArgs(std::string s);

std::vector<pair<int, string>> getHistoryChats(sf::TcpSocket& sock);

vector<pair<pair<string, int>, pair<string, string>>> getHistoryMessages(sf::TcpSocket& sock, int chat_id);

std::string setChatName(sf::TcpSocket& sock, int chatId);

#endif