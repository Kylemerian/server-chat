#include <SFML/Network.hpp>
#include <string>
#include <iostream>


int main(){
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("0.0.0.0", 55000);
    if (status != sf::Socket::Done){
        std::cout << "Didn't connect to server\n";
    }
    std::string s = "Hello!\n";
    sf::Packet packet;
    packet << s;
    if (socket.send(packet) != sf::Socket::Done){
        std::cout << "Didn't send\n";
    }
    return 0;
}