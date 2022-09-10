#include <SFML/Network.hpp>
#include <cstring>

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