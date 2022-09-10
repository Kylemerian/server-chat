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
    
    Database(): conn("user=postgres password=1 host=localhost port=5432 dbname=postgres"){}

    int authClient(std::string log, std::string pass);

    int regClient(std::string nickname, std::string log, std::string pass);

    int message(int sender_id, std::string receiver_id, std::string message);

    std::vector <std::string> getHistoryMessages(std::string chat_id, std::string fMessage, std::string lMessage);
    
    std::vector <std::string> getChats(int id);

    void createPrivChat(std::string chat_name, std::string client_1, std::string client_2);
    
    ~Database();
};

#endif