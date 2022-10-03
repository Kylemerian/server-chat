#include <SFML/Network.hpp>
#include <cstring>
#include <iostream>
#include "database.hpp"

std::vector <std::string> getRequestsFrom(std::string data) {
    std::vector <std::string> requests;
    std::string tmp;
    for (char c: data) {
	tmp += c;
	if (c == '\n') {
	    requests.push_back(tmp);
	    tmp = "";
	}
    }
    return requests;
}

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

void sendPushes(sf::TcpSocket& sock, Database &db, int client_id) {
    std::vector <std::string> events = db.getPushes(client_id);
    for (int i = 0; i < events.size(); ++i) {
	send(sock, events[i]);
    }
}

int auth(sf::TcpSocket& sock, Database &db, std::string login, std::string password){
    std::pair <std::string, int> res = db.authClient(login , password);
    std::string response = "#auth " + std::to_string(res.second)  + " " + res.first+ "\n";
    std::cout << response << "\n";
    send(sock, response);
    if (res.second != -1) {
	sendPushes(sock, db, res.second);
    }
    return res.second;
}

void registration(sf::TcpSocket& sock, Database &db, std::string nick, std::string login, std::string password){
    int res = db.regClient(nick ,login, password);
    std::string response = "#register " + std::to_string(res) + "\n";
    send(sock, response);
}

void message(
    sf::TcpSocket& sock,
    Database &db,
    int sender_id,
    std::string receiver_id,
    std::string message,
    std::vector<std::pair<sf::TcpSocket *, int>> &clients)
{
    std::string msg = "#incomingMessage " +  db.message(sender_id, receiver_id, message) + "\n";
    std::vector <int> chatmembers = db.chatMembersVector(receiver_id);
    
    for (int i = 0; i < chatmembers.size(); ++i) {
        bool flag = false;
	for (int j = 0; j < clients.size(); ++j) {
	    if (clients[j].second == chatmembers[i]) {
		std::cout << clients[j].second << " aaa\n";
	        send(*(clients[j].first), msg);
	    	flag = true;
		//break;
	    }
	}
	if (!flag) {
	    db.savePush(chatmembers[i], msg);
        }
    }
}

void history(sf::TcpSocket& sock, Database &db, std::string chat_id){
    std::string response = "#history ";
    std::vector <std::string> res = db.getHistoryMessages(chat_id);
    for(int i = 0; i < res.size(); i++) {
        response += res[i];
    }
    response[response.size() - 1] = '\n';
    send(sock, response);
}

void chats(sf::TcpSocket& sock, Database &db, int client_id){
    std::string response = "#chats ";
    std::vector <std::string> res = db.getChats(client_id);
    for(int i = 0; i < res.size(); i++)
        response += res[i];
    response[response.size() - 1] = '\n';
    std::cout << "RESPONSE:" << response << "\n";
    send(sock, response);
}

void createChat(
    sf::TcpSocket& sock,
    Database &db,
    int client_id,
    std::vector<std::string>& args,
    std::vector<std::pair<sf::TcpSocket *, int>> &clients)
{

    for (auto x: args)
	std::cout << "arg " << x << "\n";
    if(args.size() == 3){
	std::cout << "Podumat'\n";   
        std::vector <std::string> chat_keys = db.createPrivChat(
			std::to_string(client_id) + "_" + args[2],
		       	std::to_string(client_id),
		       	args[2]);
	int receiver = std::stoi(chat_keys[2]);
	std::pair <int, bool> s;
	std::pair <int, bool> r;
	s.first = client_id;
	s.second = false;
	r.first = receiver;
	r.second = false;
	std::string opp;
	std::string me;
	std::vector <std::string> us = db.help(chat_keys[0]);
	if (us[1] == std::to_string(client_id)) {
	    opp = us[2];
	    me = us[0];
	} else {
	    opp = us[0];
	    me = us[2];
	}
	std::string msgOpp = "#incomingChat " + chat_keys[0] + " " + me + " -1 " + chat_keys[3] + "\n";
	std::string msgMe = "#incomingChat " + chat_keys[0] + " " + opp + " -1 " + chat_keys[3] + "\n";
        for (int j = 0; j < clients.size(); ++j) {
	    if (clients[j].second == receiver) {
	        send(*(clients[j].first), msgOpp);
		r.second = true;
		continue;
	    }
	    if (clients[j].second == client_id) {
		send(*(clients[j].first), msgMe);
		s.second = true;
	    }
	}
	if (!r.second) {
	    db.savePush(r.first, msgOpp);
        }
	if (!s.second) {
	    db.savePush(s.first, msgMe);
	}
    } else {
	std::vector <std::string> chat_keys = db.createPublicChat(std::to_string(client_id), args[1], args);
    	std::string msg = "#incomingChat " + chat_keys[0] + " " + args[1] + " " + std::to_string(client_id) + " " + chat_keys[1] + "\n";
	for (int m = 2; m < args.size() - 1; ++m) {
	    int member = std::stoi(args[m]);
	    bool flag = false;
	    for (int j = 0; j < clients.size(); ++j) {
		if (clients[j].second == member) {
		    send(*(clients[j].first), msg);
		    flag = true;
		    break; // ne nado break tak kak mojet bit mnogo podclucheney u odnogo clienta
		}
	    }
	    if (!flag) {
		db.savePush(member, msg);
	    }
	}
    	for (int i = 0; i < clients.size(); ++i) {
	    if (clients[i].second == client_id) {
		send(*(clients[i].first), msg);
		return;
	    }
	}
	db.savePush(client_id, msg);
    }
    return;
}

void lastmessage(sf::TcpSocket& sock, Database &db, std::string chat_id) {
    std::cout << "request for lastmessage in: " << chat_id << "\n";
    std::string msg = "#lastmessage" + db.lastmessage(chat_id);
    send(sock, msg);
}

void findUser(sf::TcpSocket& sock, Database &db, std::string nickname) {
    std::string msg = "#findUser " + db.findUser(nickname);
    send(sock, msg);
}

void chatinfo(sf::TcpSocket& sock, Database &db, std::string chat_id){
    std::string response = "#chatinfo ";
    std::string chatname = db.chatinfo(chat_id);
    response += (chatname + "\n");
    send(sock, response);
}

void chatmembers(sf::TcpSocket& sock, Database &db, std::string chat_id) {
    std::string msg = "#chatMembers";
    msg += db.chatmembers(chat_id);
    send(sock, msg);
}
