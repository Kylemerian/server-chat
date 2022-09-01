#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include <pqxx/pqxx>

enum{
    authCode,
    regCode,
    messageCode,
    historyCode,
    errCode = -1
};

class Database{
public:
    pqxx::connection conn;
    pqxx::work w;
    Database():w(conn), conn("user=postgres password=1 host=localhost port=5432 dbname=postgres"){
        /*pqxx::result r = w.exec("SELECT * from clients where client_id = 1;");
        w.commit();
        std::cout << r[0][0].c_str() << r[0][1].c_str() << r[0][2].c_str() << r[0][3].c_str() << std::endl;*/
        //  message(2, 3, "mess", "\'2010-10-10 00:00:00\'");
        //regClient("nick2", "log2", "pass2");
        std::cout << authClient("log2", "pass") << " = id\n";
    }

    int authClient(std::string log, std::string pass){
        pqxx::result r = w.exec("SELECT * FROM clients WHERE password = \'" + pass + "\' AND login = \'" + log + "\';");
        w.commit();
        if(r.empty())
            return -1;
        else
            return atoi(r[0][0].c_str());
    }

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
    }
    ~Database(){
        conn.close();
    }
};




int requestIdentify(std::string request){
    std::vector <std::string> requests = {"#auth", "#message", "#register"};
    for(int i = 0; i < requests.size(); i++){
        if(request.substr(0, requests[i].size()) == request)
            return i;
    }
    return -1;
}

void requestHandler(int reqCode, Database &db){
    switch (reqCode)
    {
    case authCode:
        //db.authClient();
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
    case errCode:
        return;
    }
}

int main(){
    Database db;
    // Create a socket to listen to new connections
    sf::TcpListener listener;
    listener.listen(55000);
    // Create a list to store the future clients
    std::vector <sf::TcpSocket*> clients;
    // Create a selector
    sf::SocketSelector selector;
    // Add the listener to the selector
    selector.add(listener);
    // Endless loop that waits for new connections
    while(1)
    {
        // Make the selector wait for data on any socket
        if (selector.wait()){
            // Test the listener
            if (selector.isReady(listener)){
                // The listener is ready: there is a pending connection
                sf::TcpSocket* client = new sf::TcpSocket;
                //client->setBlocking(false);
                if (listener.accept(*client) == sf::Socket::Done){
                    // Add the new client to the clients list
                    clients.push_back(client);
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
                for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it){
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client)){
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        std::string request;
                        if (client.receive(packet) == sf::Socket::Done){
                            packet >> request;
                            std::cout << request << "\n";
                            int reqCode = requestIdentify(request);
                            requestHandler(reqCode);
                        }
                    }
                }
            }
        }
    }

 
    return 0;
}