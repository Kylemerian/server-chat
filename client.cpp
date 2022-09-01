#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

int main(){
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("0.0.0.0", 55000);
    if (status != sf::Socket::Done){
        std::cout << "Didn't connect to server\n";
    }
    std::string s = "#Auth log pass\n";
    sf::Packet packet;
    packet << s;
    if (socket.send(packet) != sf::Socket::Done){
        std::cout << "Didn't send\n";
    }

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Chat");
    sf::Image image;
    image.loadFromFile("bg.png");
    sf::Texture bgTex;
    bgTex.loadFromImage(image);
    sf::Sprite bgSprite;
    bgSprite.setTexture(bgTex);
    sf::Vector2f targetSize(1024.0f, 768.0f);
    bgSprite.setScale(targetSize.x / bgSprite.getLocalBounds().width, targetSize.y / bgSprite.getLocalBounds().height);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                socket.disconnect();
            }
        }
       
        window.draw(bgSprite);
        window.display();
        window.clear();
    }

    return 0;
}