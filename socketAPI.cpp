#include <SFML/Network.hpp>
#include <cstring>
#include <iostream>
#include "database.hpp"

sf::Socket::Status send(sf::TcpSocket& sock, std::string s){
    char buff[s.size() + 1];
    strncpy(buff, s.c_str(), s.size());
    buff[s.size()] = 0;
    return sock.send(buff, s.size() + 1);
}

sf::Socket::Status receive(sf::TcpSocket& sock, std::string &s){
    char buff[2048];
    std::size_t status;
    sf::Socket::Status res = sock.receive(buff, 2048, status);
    buff[status] = 0;
    s = std::string(buff);
    return res;
}

int auth(sf::TcpSocket& sock, Database &db, std::string login, std::string password){
    int res = db.authClient(login , password);
    std::string response = "#auth " + std::to_string(res) + "\n";
    send(sock, response);
    return res;
}

void registration(sf::TcpSocket& sock, Database &db, std::string nick, std::string login, std::string password){
    int res = db.regClient(nick ,login, password);
    std::string response = "#register " + std::to_string(res) + "\n";
    send(sock, response);
}

void message(sf::TcpSocket& sock, Database &db, int sender_id, std::string receiver_id, std::string message){
    int res = db.message(sender_id, receiver_id, message);
    //TODO push notification to receiver_id
}

void history(sf::TcpSocket& sock, Database &db, std::string chat_id, std::string fMessage, std::string lMessage){
    std::string response = "#history ";
    std::vector <std::string> res = db.getHistoryMessages(chat_id, fMessage, lMessage);
    for(int i = 0; i < res.size(); i++)
        response += res[i];
    response[response.size() - 1] = '\n';
    send(sock, response);
}

void chats(sf::TcpSocket& sock, Database &db, int client_id){
    std::string response = "#chats ";
    std::vector <std::string> res = db.getChats(client_id);
    for(int i = 0; i < res.size(); i++)
        response += res[i];
    response[response.size() - 1] = '\n';
    send(sock, response);
}

void createChat(sf::TcpSocket& sock, Database &db, std::vector<std::string>& args){
    return;
}