#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include <pqxx/pqxx>
#include <iomanip>
#include <ctime>
#include "database.hpp"
#include "socketAPI.hpp"
#include "parseLib.hpp"
 
std::vector<std::string> cmds = {
    "#auth \"login\" \"pass\"\n",
    "#register \"nick\" \"login\" \"pass\"\n",
    "#message \"chat_id\" {message}\n",
    "#history \"chat_id\"\n",
    "#chats\n",
    "#createchat \"name\" \"id1\" \"id2\"\n"
};

int main(int argc, char ** argv){
    for(auto cmd : cmds)
        std::cout << cmd;
    Database db;
    sf::TcpListener listener;
    listener.listen(std::stoi(argv[1]), "0.0.0.0");
    std::cout << sf::IpAddress::getPublicAddress() << "\n";
    std::vector <std::pair<sf::TcpSocket*, int>> clients;
    sf::SocketSelector selector;
    selector.add(listener);
//while(1)
        while(selector.wait()){
            if (selector.isReady(listener)){
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done){
                    clients.push_back(std::make_pair(client, -1));
                    selector.add(*client);
                }
                else
                    delete client;
            }
            else{
                for (int i = 0; i < clients.size(); i++){
                    sf::TcpSocket& client = *(clients[i].first);
                    if (selector.isReady(client)){
                        std::string request;
                        if (receive(client, request) == sf::Socket::Done){
                            requestHandler(&clients[i], request, db, clients);
                        }
                    }
                }
            }
        }
//  }
    return 0;
}
