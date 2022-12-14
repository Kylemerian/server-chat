#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <pqxx/pqxx>
#include <iomanip>
#include <ctime>
#include <cstring>

class Database{
public:
    pqxx::connection conn;

    Database(): conn("user=postgres password=1 host=localhost port=5432 dbname=postgres"){
        std::cout << conn.is_open() << "\n";
    }

    std::pair <std::string, int> authClient(std::string log, std::string pass);

    int regClient(std::string nickname, std::string log, std::string pass);

    std::string message(int sender_id, std::string receiver_id, std::string message);

    std::vector <std::string> getHistoryMessages(std::string chat_id);
    
    std::vector <std::string> getChats(int id);

    std::vector <std::string> createPrivChat(std::string chat_name, std::string client_1, std::string client_2);

    std::vector <std::string> createPublicChat(std::string host_id, std::string chat_name, std::vector<std::string> members);

    std::string chatinfo(std::string chat_id);
    
    std::vector <std::string> help(std::string chat_id);
    
    std::string chatmembers(std::string chat_id);

    std::vector <int> chatMembersVector(std::string chat_id);

    std::string lastmessage(std::string chat_id);

    std::string findUser(std::string nickname);

    void savePush(int client_id, std::string event);

    std::vector <std::string> getPushes(int client_id); 
    
    ~Database();
};

#endif
