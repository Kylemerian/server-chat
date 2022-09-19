#include <SFML/Network.hpp>
#include <cstring>
/**/#include <iostream>
#include <cctype>

using namespace std;

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

std::string setChatName(sf::TcpSocket& sock, int chatId){
    string request = "#chatinfo " + to_string(chatId) + "\n";
    send(sock, request);
    receive(sock, request);
    return request;
}

std::vector<pair<int, string>> getHistoryChats(sf::TcpSocket& sock){
    std::cout << "cheking" << endl;
    std::vector<pair<int, string>> res;
    string request = "#chats\n";
    send(sock, request);
    receive(sock, request);
    int i = 7;
    int cnt = 0;
    string arg[3] = {"", "", ""};
    while(request[i] != '\n'){
        if(isspace(request[i])){
            if(cnt == 2){
                res.push_back(make_pair(stoi(arg[0]), arg[1]));
                arg[0] = ""; arg[1] = ""; arg[2] = "";
            }
            cnt = (cnt + 1) % 3;
        }
        else{
            arg[cnt] += request[i];
        }
        i++;
    }
    if(cnt == 2)
        res.push_back(make_pair(stoi(arg[0]), arg[1]));
    return res;
}

 vector<pair<pair<string, int>, pair<string, string>>> getHistoryMessages(sf::TcpSocket& sock, int chat_id){
    vector<pair<pair<string, int>, pair<string, string>>> res;
    string request = "#history " + to_string(chat_id) + " 0 50" + "\n";
    send(sock, request);
    receive(sock, request);
    cout << "m = " << request << "\n";
    int i = 9;
    int cnt = 0;
    string arg[5] = {"", "", "", "", ""};
    while(request[i] != '\n'){
        if(isspace(request[i])){
            if(cnt == 4){
                res.push_back(make_pair(make_pair(arg[0], stoi(arg[1])), make_pair(arg[2], arg[3] + " " + arg[4])));
                arg[0] = ""; arg[1] = ""; arg[2] = ""; arg[3] = ""; arg[4] = "";
            }
            cnt = (cnt + 1) % 5;
        }
        else if(request[i] == '{'){
            i++;
            while(request[i] != '}'){
                arg[cnt] += request[i];
                i++;
            }
            //arg[cnt] += request[i];
        }
        else{
            arg[cnt] += request[i];
        }
        i++;
    }
    if(cnt == 4)
        res.push_back(make_pair(make_pair(arg[0], stoi(arg[1])), make_pair(arg[2], arg[3] + " " + arg[4])));
    return res;
}
