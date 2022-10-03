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
#include <set>

std::vector<std::string> cmds = {
    "#auth \"login\" \"pass\"\n",
    "#register \"nick\" \"login\" \"pass\"\n",
    "#message \"chat_id\" {message}\n",
    "#history \"chat_id\"\n",
    "#chats\n",
    "#createchat \"name\" \"id2\"\n"
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
		std::cout << "--------------------------\n";
		std::cout << "client.size() = " << clients.size() << "\n";
		std::cout << "Discon = " << sf::Socket::Status::Disconnected << "\n";
                std::set <std::pair<sf::TcpSocket *, int>> disconnectedClients;
                for (int i = 0; i < clients.size(); i++){
		    clients[i].first->setBlocking(false);
		    char s[] = "#ping\n";
		    if(clients[i].first->send(s, 7) == 3){
			selector.remove(*(clients[i].first));
			disconnectedClients.insert(clients[i]);
		    }
		    else
			clients[i].first->setBlocking(true);
		}
		std::cout << clients.size() << " SDDDDDDDDDDDD\n"; 
                while(!disconnectedClients.empty()){
		    for(auto it = clients.begin(); it != clients.end(); it++)
		        if(disconnectedClients.contains(*it)){
			    std::cout << "before delete cli; size =" << disconnectedClients.size() <<  "\n";
			    disconnectedClients.erase(disconnectedClients.find(*it));
			    std::cout << "after cli; size =" << disconnectedClients.size() <<  "\n";
			    clients.erase(it);
			    break;
			}
		        
		    std::cout << "HERE " << disconnectedClients.size() << "\n";
		}
		std::cout << "client.size() = " << clients.size() << "\n";
		        

                for (int i = 0; i < clients.size(); i++){
                    sf::TcpSocket& client = *(clients[i].first);
                    if (selector.isReady(client)){
                        std::string data;
                        if (receive(client, data) == sf::Socket::Done){
			    std::vector <std::string> requests = getRequestsFrom(data);
			    for (auto x: requests) {
			    	requestHandler(&clients[i], x, db, clients);
                            }
			}
                    }
                }
		std::cout << "--------------------------\n";
            }
        }
//  }
    return 0;
}
