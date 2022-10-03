#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include <pqxx/pqxx>
#include <iomanip>
#include <ctime>
#include "database.hpp"


std::pair <std::string, int>  Database::authClient(std::string log, std::string pass){
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("SELECT * FROM clients WHERE password = \'" +
        pass + "\' AND login = \'" + log + "\';");
    
    tr.commit();
    std::pair <std::string, int> resp;
    if(r.empty()) {
        resp.first = std::string("notfound");
	resp.second = -1;
	return resp;
    }
    resp.first = r[0][1].c_str();
    resp.second = std::stoi(r[0][0].c_str());
    return resp;
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
    std::cout << "MESS = " << message << "\n";
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
    pqxx::result r = tr.exec("select chatmembers.chat_id, chat_name, host_id, date_created "
        "from chatmembers join chats on chats.chat_id = chatmembers.chat_id "
        "where client_id = " + std::to_string(id) + ";");
    tr.commit();
    for(int i = 0; i < r.size(); i++) {
	std::string name;
	std::cout << "host: " << r[i][2].c_str() << "\n";
	if (std::stoi(r[i][2].c_str()) == -1) {
	    std::vector <std::string> h = help(std::string(r[i][0].c_str()));
	    for (auto x:h) std::cout << x << "\n"; 
	    if (std::stoi(h[1].c_str()) == id) {
		name = h[2];
	    } else {
		name = h[0];
	    }
	} else {
	    name = std::string(r[i][1].c_str());
	}
	std::string s = r[i][0].c_str() + std::string(" ") +
	       	 name + " " + r[i][2].c_str() + " " +r[i][3].c_str() + " ";
        res.push_back(s);
    }
    return res;
}
std::vector <std::string> Database::createPrivChat(
    std::string chat_name,
    std::string client_1,
    std::string client_2)
{
    std::time_t t = std::time(nullptr);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%F %T");
    pqxx::work tr1(conn);
    pqxx::result r1 = tr1.exec("insert into chats(chat_name, date_created) values (\'" + chat_name + "\', \'" + ss.str() + "\');");
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
    chat_keys.push_back(ss.str());
    return chat_keys;
}

std::string Database::chatinfo(std::string chat_id){
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select * from chats where chat_id = " + chat_id + ";");
    tr.commit();
    return r[0][1].c_str();
}

std::vector <std::string> Database::help(std::string chat_id) {
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select clients.nickname, chatmembers.client_id from chatmembers join clients on clients.client_id = chatmembers.client_id where chat_id = " + chat_id + ";");
    tr.commit();
    std::vector <std::string> resp;
    resp.push_back(r[0][0].c_str());
    resp.push_back(r[0][1].c_str());
    resp.push_back(r[1][0].c_str());
    resp.push_back(r[1][1].c_str());
    return resp;
}

std::string Database::chatmembers(std::string chat_id) {
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select clients.nickname, chatmembers.client_id from chatmembers join clients on clients.client_id = chatmembers.client_id where chat_id = " + chat_id + ";");
    tr.commit();
    std::string res;
    for (int i = 0; i < r.size(); ++i) {
        res += " " + std::string(r[i][0].c_str()) + " " + std::string(r[i][1].c_str()); 
    }
    res += std::string("\n");
    return res;
}

std::vector <int> Database::chatMembersVector(std::string chat_id) {
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
    pqxx::result r = tr.exec("select sender_id, message, time from messages where chat_id = " + chat_id + "order by message_id desc limit 1");
    tr.commit();
    if (r.size() == 0) {
	return std::string("\n");
    }
    pqxx::work tr2(conn);
    std::string client_id = std::string(r[0][0].c_str());
    pqxx::result nicknameResult = tr2.exec("select nickname from clients where client_id = " + client_id + ";");
    std::string nickname = std::string(nicknameResult[0][0].c_str());
    std::string text = std::string(r[0][1].c_str());
    std::string date = std::string(r[0][2].c_str());
    std::string msg = " " + chat_id + " " + nickname + " " + client_id + " {" + text + "} " + date + "\n";
    return msg;
}

std::string Database::findUser(std::string nickname) {
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("select client_id from clients where nickname = '" + nickname + "';");
    if (r.size() == 0) {
	return std::string("-1\n");
    }
    return (std::string(r[0][0].c_str()) + "\n"); 
}

std::vector <std::string> Database::createPublicChat(std::string host_id, std::string chat_name, std::vector<std::string> members){
    pqxx::work tr1(conn);
    std::time_t t = std::time(nullptr);
    std::ostringstream ss;
    std::vector <std::string> chat_keys;
    ss << std::put_time(std::localtime(&t), "%F %T");
    pqxx::result r1 = tr1.exec("insert into chats(chat_name, host_id, date_created) values (\'" + chat_name + "\', " + host_id + ", \'" + ss.str() + "\');");
    tr1.commit();
    pqxx::work tr0(conn);
    pqxx::result r0 = tr0.exec("select currval(pg_get_serial_sequence('chats', 'chat_id'));");
    tr0.commit();
    int chatid = std::stoi(r0[0][0].c_str());
    chat_keys.push_back(std::to_string(chatid));
    for(int i = 2; i < members.size() - 1; i++){
        pqxx::work tr2(conn);
        pqxx::result r2 = tr2.exec("Insert into chatmembers(chat_id, client_id)"
            " values(" + std::to_string(chatid) + ", " + members[i] + ");");
        tr2.commit();
    }
    pqxx::work tr2(conn);
    pqxx::result r2 = tr2.exec("Insert into chatmembers(chat_id, client_id) values(" + std::to_string(chatid) + ", " + host_id + ");");
    tr2.commit();
    chat_keys.push_back(ss.str());
    return chat_keys;
}

void Database::savePush(int client_id, std::string event) {
    pqxx::work tr(conn);
    pqxx::result r = tr.exec("insert into pushes values(" + std::to_string(client_id) + ", '" + event + "');");
    tr.commit();
}

std::vector <std::string> Database::getPushes(int client_id) {
    pqxx::work tr(conn);
    pqxx::result r = tr.exec(
		    "select event from pushes where client_id = " + std::to_string(client_id) + ";");
    tr.commit();
    std::vector <std::string> response;
    for (int i = 0; i < r.size(); ++i) {
	response.push_back(std::string(r[i][0].c_str()));
    }
    pqxx::work tr1(conn);
    pqxx::result r1 = tr1.exec("delete from pushes where client_id = " + std::to_string(client_id) + ";");
    tr1.commit();
    return response;
}

Database::~Database(){
    conn.close();
}
