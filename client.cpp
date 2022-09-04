#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "FormSFML.h"

using namespace std;


sf::TcpSocket socket;


enum pages{
    LOGPAGE = 0,
    AUTHPAGE,
    MAINPAGE
};

enum WindowSize{
    HEIGHT = 1024,
    WIDTH = 768
};

class UI{
    int page;
    sf::RenderWindow * window;
    TextField log;
    TextField pass;
    Button btn;
    int resAuth;
public:
    UI(sf::RenderWindow * x) : 
        resAuth(-1),
        window(x), log(20, window),
        pass(20, window),
        btn("Sign In", {window->getSize().x / 3.f, 50}, 30, sf::Color(41, 85, 135), sf::Color::White){
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

        sf::Font font;
        font.loadFromFile("ttf/None.ttf");
	    btn.setFont(font);
        btn.setBold();
        btn.setSize(window->getSize().x / 3.f, 50);
	    btn.setPosition({window->getSize().x / 3.f, window->getSize().y / 2.f + 90} );

        log.setSize(window->getSize().x / 3.f, 30);
        log.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f - 50);
        pass.setSize(window->getSize().x / 3.f, 30);
        pass.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 20);
        window -> draw(log);
        window -> draw(pass);

        btn.drawTo(*window);
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
        if(event.type == sf::Event::MouseMoved)
				if (btn.isMouseOver(*window)) {
					btn.setBackColor(sf::Color(52, 108, 171));
				}
				else {
					btn.setBackColor(sf::Color(41, 85, 135));
				}
	    if(event.type == sf::Event::MouseButtonPressed)
				if (btn.isMouseOver(*window)) {
                    sf::Packet inPacket, outPacket;
					string s = "#auth " + log.getText() + ' ' + pass.getText() + "\n";
                    outPacket << s;
                    if (socket.send(outPacket) != sf::Socket::Done){
                        std::cout << "Didn't send\n";
                    }
                    //cout << "send pack\n";
                    if(socket.receive(inPacket) != sf::Socket::Done)
                        cout << "smth" << "\n";
                    inPacket >> s;
                    resAuth = std::stoi(s);
                    cout << "get pack: " << resAuth << "\n";
				}
	
    }
    void mainDraw(){
        sf::Packet outPacket, inPacket;
        string s;
        outPacket << "#chats\n"; 
        if (socket.send(outPacket) != sf::Socket::Done){
            std::cout << "Didn't send\n";
        }
        if(socket.receive(inPacket) != sf::Socket::Done)
            cout << "smth" << "\n";
        inPacket >> s;
        vector<pair<string, string>> listChats = parseChats(s);
        for(int i = 0; i < listChats.size(); i++)
            cout << listChats[i].first << " " << listChats[i].second << "\n";
        
        sf::RectangleShape chats[listChats.size()];
        for(int i = 0; i < listChats.size(); i++){
            chats[i].setSize({HEIGHT * 0.3f - 5.0f, 65.0f});
            chats[i].setPosition({0, i * 70 + WIDTH /18 + 5});
            chats[i].setFillColor(sf::Color(26, 34, 44));
            chats[i].setOutlineThickness(4);
            chats[i].setOutlineColor(sf::Color(20, 30, 40));
        }

        sf::RectangleShape Boxes[4];
        for(int i = 0; i < 4; i++){
            Boxes[i].setOutlineThickness(4);
            Boxes[i].setOutlineColor(sf::Color(20, 30, 40));
            Boxes[i].setFillColor(sf::Color(26, 34, 44));
        }
        Boxes[0].setSize({HEIGHT * 0.3f, window -> getSize().y * 1.0f});
        Boxes[1].setSize({HEIGHT * 0.3f, WIDTH /18});
        
        Boxes[2].setSize({window -> getSize().x * 0.7f, window -> getSize().y * 1.0f});
        Boxes[3].setSize({window -> getSize().x * 0.7f, WIDTH /18});
        Boxes[2].setPosition({HEIGHT * 0.3f, 0});
        Boxes[3].setPosition({HEIGHT * 0.3f, 0});
    
        
        window -> draw(Boxes[0]);
        window -> draw(Boxes[1]);
        window -> draw(Boxes[2]);
        window -> draw(Boxes[3]);
        for(int i = 0; i < listChats.size(); i++)
            window -> draw(chats[i]);
    }
    void logDraw(){

    }
    void events(sf::Event e){
        switch (page){
        case AUTHPAGE:
            authEvents(e);
            if(resAuth != -1)
                page = MAINPAGE;
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
//185.204.0.32:3389
int main(int argc, char ** argv){
    sf::Socket::Status status = socket.connect("0", stoi(argv[1]));
    if (status != sf::Socket::Done){
        std::cout << "Didn't connect to server\n";
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