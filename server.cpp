#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include <pqxx/pqxx>
#include "handshake.hpp"

enum{
    authCode = 0,
    regCode = 1,
    messageCode = 2,
    historyCode = 3,
    chatCode = 4,
    HTTPCode = 5,
    errCode = -1
};

std::vector<std::string> getArgs(std::string s) {
  std::vector<std::string> v;
  std::string tmp = "";
  for(int i = 0; i < s.size(); i++){
        if(!isspace(s[i]))
            tmp += s[i];
        else{
            v.push_back(tmp);
            tmp = "";
        }
  }
  return v;
}

class Database{
public:
    pqxx::connection conn;
    //pqxx::work w;
    Database():/*w(conn),*/ conn("user=postgres password=1 host=localhost port=5432 dbname=postgres"){
        /*pqxx::result r = w.exec("SELECT * from clients where client_id = 1;");
        w.commit();
        std::cout << r[0][0].c_str() << r[0][1].c_str() << r[0][2].c_str() << r[0][3].c_str() << std::endl;*/
        //message(2, 3, "mess", "\'2010-10-10 00:00:00\'");
        //regClient("nick2", "log2", "pass2");
        //std::cout << authClient("log2", "pass") << " = id\n";
    }

    int authClient(std::string log, std::string pass){
        pqxx::work tr(conn);
        pqxx::result r = tr.exec("SELECT * FROM clients WHERE password = \'" + pass + "\' AND login = \'" + log + "\';");
        tr.commit();
        if(r.empty())
            return -1;
        else
            return atoi(r[0][0].c_str());
    }
/*
    int regClient(std::string nickname, std::string log, std::string pass){
        pqxx::result r = w.exec("SELECT * FROM clients WHERE login = \'" + log + "\';");
        if(r.size() != 0)
            return -1;
        r = w.exec("INSERT INTO clients(nickname, login, password)"
                                "VALUES(\'"+ nickname + "\', \'" + log + "\', \'" + pass +"\');");
        w.commit();
        return 0;
    }

    int message(int sender_id, int receiver_id, std::string message, std::string time){
        pqxx::result r = w.exec("INSERT INTO messages(sender_id, receiver_id, time, message)"
                                "VALUES(" + std::to_string(sender_id) + ", " + std::to_string(receiver_id) + ", " + time + ", \'" + message + "\');");
        w.commit();
        return r.size();
    }

    int getHistoryMessages(int client_id){
        //get req to db
        return 1;
    }*/
    std::vector <std::string> getChats(int id){
        pqxx::work tr(conn);
        pqxx::result r = tr.exec("SELECT * FROM chats WHERE client_id_1 = " + std::to_string(id) + ";");
        std::vector <std::string> res;
        std::cout << id << " =id; rsize = " << r.size() << "\n";
        for(int i = 0; i < r.size(); i++){
            std::string s = r[i][2].c_str();
            res.push_back(s + std::string(" messageeeeeeeeeeeeeeeeeeeeee\n"));
        }
        tr.commit();
        return res;
    }
    ~Database(){
        conn.close();
    }
};

std::string getFirstArg(std::string s){
    std::string res = "";
    for(auto c : s)
        if(std::isspace(c) || c == '\n')
            break;
        else
            res += c;
    return res;
}

int requestIdentify(std::string request){
    std::vector <std::string> requests = {"#auth", "#register", "#message", "#history", "#chats", "HTTP"};
    std::string cmd = getFirstArg(request);
    //std::cout << cmd << "! " << (cmd == requests[0]) << "!\n";
    for(int i = 0; i < requests.size(); i++){
        if(cmd == requests[i]){
            //std::cout << cmd << " " << requests[i] << " reqCode = " << i << "\n";
            return i;
        }
    }
    return -1;
}

void requestHandler(std::pair<sf::TcpSocket *, int> * client, int reqCode, std::string request, Database &db){
    sf::Packet packet;
    int res;
    std::string outS;
    std::vector<std::string> args = getArgs(request);
    std::vector<std::string> chats;
    std::cout << "reqCode = " << reqCode << "\n";
    switch (reqCode)
    {
    case authCode:
        std::cout << "try to auth client\n";
        res = db.authClient(args[1], args[2]);
        client -> second = res;
        outS = std::to_string(res) + "\n";
        packet << outS;
        if(client -> first -> send(packet) != sf::Socket::Done)
            std::cout << "Didnt send auth code\n";
        break;
    case regCode:
        //db.regClient();
        break;
    case messageCode:
        //db.message();
        break;
    case historyCode:
        //db.getHistoryMessages();
        break;
    case chatCode:
        chats = db.getChats(client -> second);
        packet.clear();
        outS = "";
        for(int i = 0; i < chats.size(); i++)
            outS += chats[i];
        packet << outS;
        std::cout << outS << " THAT SENT\n";
        client -> first -> send(packet);
        break;
    case HTTPCode:
        outS = handshake(request);
        client -> first -> send(outS.c_str(), outS.size());
    case errCode:
        return;
    }
}

int main(int argc, char ** argv){
    Database db;
    // Create a socket to listen to new connections
    sf::TcpListener listener;
    listener.listen(std::stoi(argv[1]), "0.0.0.0");
    std::cout << sf::IpAddress::getPublicAddress() << "\n";
    // Create a list to store the future clients
    std::vector <std::pair<sf::TcpSocket*, int>> clients;
    // Create a selector
    sf::SocketSelector selector;
    // Add the listener to the selector
    selector.add(listener);
    // Endless loop that waits for new connections
    //while(1){
        // Make the selector wait for data on any socket
        while(selector.wait()){
            // Test the listener
            if (selector.isReady(listener)){
                // The listener is ready: there is a pending connection
                sf::TcpSocket* client = new sf::TcpSocket;
                //client->setBlocking(false);
                if (listener.accept(*client) == sf::Socket::Done){
                    // Add the new client to the clients list
                    clients.push_back(std::make_pair(client, -1));
                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*client);
                    std::cout << "new connection\n";
                    //printClients(clients);/**/
                }
                else{
                    // Error, we won't get a new connection, delete the socket
                    delete client;
                }
            }
            else{
                // The listener socket is not ready, test all other sockets (the clients)
                for (int i = 0; i < clients.size(); i++){
                    sf::TcpSocket& client = *(clients[i].first);
                    if (selector.isReady(client)){
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        std::string request;
                        if (client.receive(packet) == sf::Socket::Done){
                            std::cout << "Receive packet\n";
                            packet >> request;
                            std::cout << request << "\n";
                            int reqCode = requestIdentify(request);
                            requestHandler(&clients[i], reqCode, request, db);
                            //clients[i].second = 4;
                        }
                    }
                }
            }
        }
    //  }

 
    return 0;
}