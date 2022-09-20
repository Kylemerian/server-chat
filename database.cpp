#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include <pqxx/pqxx>
#include <iomanip>
#include <ctime>
#include "database.hpp"


int Database::authClient(std::string log, std::string pass){
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("SELECT * FROM clients WHERE password = \'" +
        pass + "\' AND login = \'" + log + "\';");
    
    tr.commit();
    if(r.empty())
        return -1;
    else
        return atoi(r[0][0].c_str());
}

int Database::regClient(std::string nickname, std::string log, std::string pass){
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("SELECT * FROM clients WHERE login = \'" +
        log + "\' OR nickname = \'" + nickname + "\';");
    
    tr.commit();
    if(r.size() != 0)
        return -1;
    pqxx::work tr2(conn);
    r = tr2.exec("INSERT INTO clients(nickname, login, password)"
        "VALUES(\'"+ nickname + "\', \'" + log + "\', \'" + pass +"\');");
    tr2.commit();
    return 0;
}

std::string Database::message(int sender_id, std::string receiver_id, std::string message){
    pqxx::work tr(conn);
    std::time_t t = std::time(nullptr);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%F %T");
    pqxx::result r = tr.exec("INSERT INTO messages(sender_id, chat_id, message, time, "
        "s_deleted, r_deleted) VALUES(" + std::to_string(sender_id) + ", " +
        receiver_id + ", \'" + message + "\', \'" + ss.str() + "\', false, false);");

    tr.commit();
    pqxx::work tr2(conn);
    pqxx::result client_name = tr2.exec("select nickname from clients where client_id = " + std::to_string(sender_id) + ";");
    std::string nickname = std::string(client_name[0][0].c_str());
    std::string msg = receiver_id + " " + nickname + " " + std::to_string(sender_id) + " {" + message + "} " + ss.str();
    tr2.commit();
    return msg;
}

std::vector <std::string> Database::getHistoryMessages(std::string chat_id){
    std::vector <std::string> res;
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select sender_id, message, time from messages "
        "where chat_id = " + chat_id + ";");
    tr.commit();
    for(int i = 0; i < r.size(); i++){
	pqxx::work tr(conn);
	pqxx::result client_name = tr.exec("select nickname from clients where client_id =" + std::string(" ") + r[i][0].c_str());
        std::string s = chat_id + " " + client_name[0][0].c_str() + " " + r[i][0].c_str() + std::string(" ") +
            "{" + r[i][1].c_str() + "} " + r[i][2].c_str() + " ";
        res.push_back(s);
    }
    return res;
}

std::vector <std::string> Database::getChats(int id){
    std::vector <std::string> res;    
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select chatmembers.chat_id, chat_name, host_id "
        "from chatmembers join chats on chats.chat_id = chatmembers.chat_id "
        "where client_id = " + std::to_string(id) + ";");
    tr.commit();
    for(int i = 0; i < r.size(); i++){
        std::string s = r[i][0].c_str() + std::string(" ") + r[i][1].c_str() + " " + r[i][2].c_str() + " ";
        res.push_back(s);
    }
    return res;
}
std::vector <std::string> Database::createPrivChat(
    std::string chat_name,
    std::string client_1,
    std::string client_2)
{
    pqxx::work tr1(conn);
    pqxx::result r1 = tr1.exec("insert into chats(chat_name) values (\'" + chat_name + "\');");
    tr1.commit();
    pqxx::work tr0(conn);
    pqxx::result r0 = tr0.exec("select currval(pg_get_serial_sequence('chats', 'chat_id'));");
    tr0.commit();
    int chatid = std::stoi(r0[0][0].c_str());
    pqxx::work tr2(conn);
    pqxx::result r2 = tr2.exec("Insert into chatmembers(chat_id, client_id)"
        " values(" + std::to_string(chatid) + ", " + client_1 + ");");
    tr2.commit();
    pqxx::work tr3(conn);
    pqxx::result r3 = tr3.exec("Insert into chatmembers(chat_id, client_id)"
        " values(" + std::to_string(chatid) + ", " + client_2 + ");");
    tr3.commit();

    std::vector <std::string> chat_keys;
    chat_keys.push_back(std::to_string(chatid));
    chat_keys.push_back(chat_name);
    chat_keys.push_back(client_2);
    return chat_keys;
}

std::string Database::chatinfo(std::string chat_id){
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select * from chats where chat_id = " + chat_id + ";");
    tr.commit();
    return r[0][1].c_str();
}

std::vector <int> Database::chatmembers(std::string chat_id) {
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select client_id from chatmembers where chat_id = " + chat_id + ";");
    tr.commit();
    std::vector <int> res;
    for (int i = 0; i < r.size(); ++i) {
        res.push_back(std::stoi(r[i][0].c_str()));
    }
    return res;
}

std::string Database::lastmessage(std::string chat_id) {
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select sender_id, message, time from messages order by message_id desc limit 1");
    tr.commit();
    pqxx::work tr2(conn);
    std::string client_id = std::string(r[0][0].c_str());
    pqxx::result nicknameResult = tr2.exec("select nickname from clients where client_id = " + client_id + ";");
    std::string nickname = std::string(nicknameResult[0][0].c_str());
    std::string text = std::string(r[0][1].c_str());
    std::string date = std::string(r[0][2].c_str());
    std::string msg = chat_id + " " + nickname + " " + client_id + " {" + text + "} " + date + "\n";
    return msg;
}

Database::~Database(){

    conn.close();
}
