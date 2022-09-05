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
    TextField message;
    Button btn;
    Button * send;
    int resAuth;
    sf::Font font;
    vector<Button *> chats;
public:
    UI(sf::RenderWindow * x) : 
        resAuth(-1),
        window(x), log(20, window, false),
        pass(20, window, true),
        message(500, window, false),
        btn("Sign In", {window->getSize().x / 3.f, 50}, 30, sf::Color(41, 85, 135), sf::Color::White, CENTERED){
        page = AUTHPAGE;
        font.loadFromFile("ttf/None.ttf");
    }
    void authInit(){

    }
    void authDraw(){
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
    void mainInit(){
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
        
        for(int i = 0; i < listChats.size(); i++){
            Button * tmp = new Button(listChats[i].first, {HEIGHT * 0.3f - 5.0f, 65.0f}, 15, sf::Color(26, 34, 44),
                sf::Color::White, UPPED, listChats[i].second);
            tmp -> setPosition({0.0f, i * 70 * 1.0f + WIDTH /18 + 5});
            //tmp -> setOutline(sf::Color(26, 34, 44), sf::Color(20, 30, 40), 4);
            chats.push_back(tmp);
        }

        message.setSize(window -> getSize().x - 0.35f * HEIGHT, 30.0f);
        message.setPosition(HEIGHT * 0.31f, window -> getSize().y - 40);
        send = new Button("", {25, 25}, 0, sf::Color(35, 40, 53), sf::Color::Black, CENTERED);
        send -> setPosition({1.0f * window -> getSize().x - 32, 1.0f * window -> getSize().y - 37});
        if (send -> isMouseOver(*window))
                send -> setBackColor(sf::Color(40, 45, 58));
            else
                send -> setBackColor(sf::Color(35, 40, 53));
        sf::RectangleShape Boxes[4];
        for(int i = 0; i < 4; i++){
            Boxes[i].setOutlineThickness(4);
            Boxes[i].setOutlineColor(sf::Color(20, 30, 40));
            Boxes[i].setFillColor(sf::Color(26, 34, 44));
        }
        Boxes[0].setSize({HEIGHT * 0.3f, window -> getSize().y * 1.0f});
        Boxes[1].setSize({HEIGHT * 0.3f, WIDTH /18});
        
        Boxes[2].setFillColor(sf::Color(20, 30, 40));
        Boxes[2].setSize({window -> getSize().x * 0.7f, window -> getSize().y * 1.0f});
        Boxes[3].setSize({window -> getSize().x * 0.7f, WIDTH /18});
        Boxes[2].setPosition({HEIGHT * 0.3f, 0});
        Boxes[3].setPosition({HEIGHT * 0.3f, 0});
        for(int i = 0; i < chats.size(); i++)
            if (chats[i] -> isMouseOver(*window))
                chats[i] -> setBackColor(sf::Color(30, 38, 48));
            else
                chats[i] -> setBackColor(sf::Color(26, 34, 44));
        window -> draw(Boxes[0]);
        window -> draw(Boxes[1]);
        window -> draw(Boxes[2]);
        window -> draw(Boxes[3]);
        send -> drawTo(*window);
    }
    void mainDraw(){
        
        for(int i = 0; i < chats.size(); i++){
            chats[i] -> drawTo(*window);
        }
        
        window -> draw(message);
    }
    void mainEvents(sf::Event event){
	    if(event.type == sf::Event::MouseButtonPressed)
			for(int i = 0; i < chats.size(); i++)
            	if (chats[i] -> isMouseOver(*window)) {
                    cout << "click chat: " << chats[i] -> getText() << "\n";//send req to open chat
                    sf::Packet packet;
                    string outS = "#chat " + chats[i] -> getText() + "\n";
                    packet << outS;
                    socket.send(packet);
                    packet.clear();
                    socket.receive(packet);
                    outS = ""; packet >> outS;
                    cout << outS << " messages\n";
				}
        if (event.type == sf::Event::MouseButtonReleased){
                auto pos = sf::Mouse::getPosition(*window);
                message.setFocus(false);
                if (message.contains(sf::Vector2f(pos)))
                    message.setFocus(true);
            }
            else
                message.handleInput(event);

        if(event.type == sf::Event::MouseButtonPressed)
            if(send -> isMouseOver(*window)){
                cout << "message = " << message.getText() << " sent\n";//send req mess
                message.setText("");
            }
    }
    void mainClean(){
        for(int i = 0; i < chats.size(); i++)
            //cout << chats[i]->getText() << "\n";
            delete chats[i];
        chats.clear();
        delete send;
    }
    void logDraw(){

    }
    void init(){
        switch (page){
        case AUTHPAGE:
            authInit();
            break;
        case LOGPAGE:
            //logEvents();
            break;
        case MAINPAGE:
            mainInit();
            break;
        }
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
            mainEvents(e);
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
    void cleanMem(){
        switch (page){
        case AUTHPAGE:
            //authDraw();
            break;
        case LOGPAGE:
            //logDraw();
            break;
        case MAINPAGE:
            mainClean();
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
    window.setVerticalSyncEnabled(true);
    UI ui(&window);

    sf::View view = window.getDefaultView();

    while(window.isOpen()){
        sf::Event event;
        ui.init();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                socket.disconnect();
            }
            if (event.type == sf::Event::Resized){
                // update the view to the new size of the window
                /*if(window.getSize().x < 500)
                    window.setSize({500, window.getSize().y});*/
                if(window.getSize().y < 400)
                    window.setSize({window.getSize().x, 400});
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            ui.events(event);
				   
        }
        ui.draw();
        window.display();
        window.clear(sf::Color(26, 34, 44));
        ui.cleanMem();
    }

    return 0;
}