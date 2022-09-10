#ifndef SOCKETS_HPP
#define SOCKETS_HPP

sf::Socket::Status send(sf::TcpSocket& sock, std::string s);

sf::Socket::Status receive(sf::TcpSocket& sock, std::string &s);

std::vector<std::string> getArgs(std::string s);

#endif