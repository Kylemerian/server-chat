#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "FormSFML.h"

using namespace std;

enum pages{
    LOGPAGE = 0,
    AUTHPAGE,
    MAINPAGE
};

enum WindowSize{
    HEIGHT = 1024,
    WIDTH = 728
};

class UI{
    int page;
    sf::RenderWindow * window;
    TextField log;
    TextField pass;
public:
    UI(sf::RenderWindow * x) : window(x), log(20, window), pass(20, window){
        page = AUTHPAGE;
    }
    void authDraw(){
        
        /*log.setSize({window->getSize().x / 3.f, 30});
        pass.setSize({window->getSize().x / 3.f, 30});
        signInButton.setSize({window->getSize().x / 3.f, 30});
        log.setPosition();
        pass.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 20);
        signInButton.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 90);
        log.setFillColor(sf::Color(80, 162, 242));
        pass.setFillColor(sf::Color(80, 162, 242));
        signInButton.setFillColor(sf::Color(80, 162, 242));*/
        log.setSize(window->getSize().x / 3.f, 30);
        log.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f - 50);
        pass.setSize(window->getSize().x / 3.f, 30);
        pass.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 20);
        window -> draw(log);
        window -> draw(pass);
    }
    void authEvents(sf::Event event){
        if (event.type == sf::Event::MouseButtonReleased){
                auto pos = sf::Mouse::getPosition(*window);
                log.setFocus(false);
                pass.setFocus(false);
                if (log.contains(sf::Vector2f(pos))){
                    log.setFocus(true);
                }
                if (pass.contains(sf::Vector2f(pos))){
                    pass.setFocus(true);
                }
            }
            else{
                log.handleInput(event);
                pass.handleInput(event);
            }
    }
    void mainDraw(){

    }
    void logDraw(){

    }
    void events(sf::Event e){
        switch (page){
        case AUTHPAGE:
            authEvents(e);
            break;
        case LOGPAGE:
            //logEvents();
            break;
        case MAINPAGE:
           // mainEvents();
            break;
        }
    }
    void draw(){
        switch (page){
        case AUTHPAGE:
            authDraw();
            break;
        case LOGPAGE:
            logDraw();
            break;
        case MAINPAGE:
            mainDraw();
            break;
        }
    }
    ~UI(){
        
    }
};

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
    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "Chat");
    UI ui(&window);

    sf::View view = window.getDefaultView();

    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                socket.disconnect();
            }
            if (event.type == sf::Event::Resized){
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            ui.events(event);
				   
        }
        ui.draw();
        window.display();
        window.clear(sf::Color(26, 34, 44));
    }

    return 0;
}