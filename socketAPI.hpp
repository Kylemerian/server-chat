#ifndef SOCKETAPI_HPP
#define SOCKETAPI_HPP

sf::Socket::Status send(sf::TcpSocket& sock, std::string s);

sf::Socket::Status receive(sf::TcpSocket& sock, std::string &s);

int auth(sf::TcpSocket& sock, Database &db, std::string login, std::string password);

void registration(sf::TcpSocket& sock, Database &db, std::string nick, std::string login, std::string password);

void message(sf::TcpSocket& sock, Database &db, int sender_id, std::string receiver_id, std::string message, std::vector<std::pair<sf::TcpSocket *, int>> &clients);

void history(sf::TcpSocket& sock, Database &db, std::string chat_id);

void chats(sf::TcpSocket& sock, Database &db, int client_id);

void createChat(sf::TcpSocket& sock, Database &db, int client_id, std::vector<std::string>& args, std::vector<std::pair<sf::TcpSocket *, int>> &clients);

void lastmessage(sf::TcpSocket& sock, Database &db, std::string chat_id);

void chatinfo(sf::TcpSocket& sock, Database &db, std::string chat_id);

#endif
