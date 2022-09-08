#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstring>
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

sf::Socket::Status send(sf::TcpSocket& sock, std::string s){
    char buff[s.size() + 1];
    strncpy(buff, s.c_str(), s.size());
    buff[s.size()] = 0;
    return sock.send(buff, s.size() + 1);
}

sf::Socket::Status receive(sf::TcpSocket& sock, std::string &s){
    char buff[1024];
    std::size_t status;
    sf::Socket::Status res = sock.receive(buff, 1024, status);
    buff[status] = 0;
    s = std::string(buff);
    return res;
}

class UI{
    int page;//current page
    //general
    sf::RenderWindow * window;
    //forAuth
    TextField * log;
    TextField * pass;
    Button * btn;
    int resAuth;
    //forMain
    TextField * message;
    Button * sendBtn;
    sf::Font font;
    Scroll chats;
    Scroll msgs;
    pair<Button *, int> convName;
    //forReg
    int resReg;
public:
    UI(sf::RenderWindow * x) : 
        resAuth(-1),
        resReg(-1),
        window(x),
        chats(x, sf::Color(30, 38, 48), sf::Color(26, 34, 44)),
        msgs(x, sf::Color(38, 76, 110), sf::Color(43, 82, 120))
    {
        log = new TextField(20, window, false);
        pass = new TextField(20, window, true);
        btn = new Button("Sign In", {window->getSize().x / 3.f, 50}, 30, sf::Color(41, 85, 135), sf::Color::White, CENTERED);
        message = new TextField(500, window, false);
        page = AUTHPAGE;
        font.loadFromFile("ttf/None.ttf");
        sendBtn = new Button("", {25, 25}, 0, sf::Color(35, 40, 53), sf::Color::Black, CENTERED);
        convName.first = new Button("", {window -> getSize().x - 0.32f * HEIGHT, 30.0f}, 20, sf::Color(26, 34, 44), sf::Color::White, CENTERED);
        convName.second = -1;
    }
    void authInit(){
        log->setSize(window->getSize().x / 3.f, 30);
        log->setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f - 50);
        pass->setSize(window->getSize().x / 3.f, 30);
        pass->setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 20);
        btn->setFont(font);
        btn->setBold();
        btn->setSize(window->getSize().x / 3.f, 50);
	    btn->setPosition({window->getSize().x / 3.f, window->getSize().y / 2.f + 90} );
    }
    void authDraw(){
        window -> draw(*log);
        window -> draw(*pass);
        btn -> drawTo(*window);
    }
    void authClean(){
        delete pass;
        delete log;
    }
    void authEvents(sf::Event event){
        if (event.type == sf::Event::MouseButtonReleased){
                auto pos = sf::Mouse::getPosition(*window);
                log -> setFocus(false);
                pass -> setFocus(false);
                if (log -> contains(sf::Vector2f(pos)))
                    log -> setFocus(true);
                if (pass -> contains(sf::Vector2f(pos)))
                    pass -> setFocus(true);
        }
        else{
            log -> handleInput(event);
            pass -> handleInput(event);
        }

        if(event.type == sf::Event::MouseMoved)
            if (btn -> isMouseOver(*window))
                btn -> setBackColor(sf::Color(52, 108, 171));
            else 
                btn -> setBackColor(sf::Color(41, 85, 135));

	    if(event.type == sf::Event::MouseButtonReleased)
            if (btn -> isMouseOver(*window)) {
                string s = "#auth " + log->getText() + ' ' + pass->getText() + "\n";
                cout << "snding auth" << s << "!\n";
                
                if (send(socket, s) != sf::Socket::Done)
                    std::cout << "Didn't send\n";
                s = ""; 
                if(receive(socket, s) != sf::Socket::Done)
                    cout << "Error receive" << "\n";
                cout << "receive auth\n";
                resAuth = std::stoi(s);
                cout << "get pack: " << resAuth << "\n";
            }
    }
    void mainInit(){
        string s;
        if (send(socket, string("#chats\n")) != sf::Socket::Done)
            ;//std::cout << "Didn't send\n";
        if(receive(socket, s) != sf::Socket::Done)
            ;//cout << "Didnt receive" << "\n";
        
        if(chats.isClear()){
            vector<pair<string, string>> listChats = parseChats(s);
            for(int i = 0; i < listChats.size(); i++){
                Button * tmp = new Button(listChats[i].first, {HEIGHT * 0.3f - 5.0f, 65.0f}, 15, sf::Color(26, 34, 44),
                    sf::Color::White, UPPED, listChats[i].second);

                tmp -> setPosition({0.0f, i * 70 * 1.0f + WIDTH /18 + 5});
                chats.addContent(tmp);
            }
        }
        //
        if(convName.second != -1 && msgs.isClear()){
            string outS = "#history " + to_string(convName.second) + string("\n");
            send(socket, outS);
            outS = "";
            receive(socket, outS);
            vector<string> listMsgs = parseMsgs(outS);
            for(int i = 0; i < listMsgs.size(); i++){
                Button * tmp = new Button(listMsgs[listMsgs.size() - 1 - i], {HEIGHT * 0.25f - 5.0f, 35.0f}, 13, sf::Color::Red,
                    sf::Color::White, CENTERED);
                tmp -> setPosition({0.31f * HEIGHT, i * 50 * 1.0f + WIDTH /18 + 10});
                msgs.addContent(tmp);
            }
        }
        
        message -> setSize(window -> getSize().x - 0.35f * HEIGHT, 30.0f);
        message -> setPosition(HEIGHT * 0.31f, window -> getSize().y - 40);
        
        convName.first -> setSize(window -> getSize().x - 0.32f * HEIGHT, 30.0f);
        convName.first -> setPosition({HEIGHT * 0.31f, 7.0f});
        
        sendBtn -> setPosition({1.0f * window -> getSize().x - 32, 1.0f * window -> getSize().y - 37});
        if (sendBtn -> isMouseOver(*window))
            sendBtn -> setBackColor(sf::Color(40, 45, 58));
        else
            sendBtn -> setBackColor(sf::Color(35, 40, 53));        
    }
    void mainDraw(){
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

        window -> draw(Boxes[0]);
        window -> draw(Boxes[2]);
        chats.draw(*window);
        msgs.draw(*window);
        window -> draw(Boxes[1]);
        if(convName.second != -1){
            window -> draw(*message);
            sendBtn -> drawTo(*window);
        }
        window -> draw(Boxes[3]);
        convName.first -> drawTo(*window);
    }
    void mainEvents(sf::Event event){
	    if(event.type == sf::Event::MouseButtonPressed)
			for(int i = 0; i < chats.amount(); i++)
            	if (chats.isMouseOver(*window)) {
                    cout << "click chat: " << chats.idClickedBox() << "\n";//send req to open chat
                    if(chats.idClickedBox() != ""){
                        convName.second = stoi(chats.idClickedBox());
                        convName.first->setText(to_string(convName.second));
                        msgs.clear();
                    }
                    // packet.clear();
//                    string outS = "#history " + to_string(convName.second) + string("\n");
//                    packet << outS;
//                   socket.send(packet);
//                    packet.clear();
//                  socket.receive(packet);
//                    outS = ""; packet >> outS;                  
//                    cout << outS << "\n";
			    }
        if (event.type == sf::Event::MouseButtonPressed){
            auto pos = sf::Mouse::getPosition(*window);
            message -> setFocus(false);
            if (message -> contains(sf::Vector2f(pos)))
                message -> setFocus(true);
        }
        else
            message->handleInput(event);

        if((event.type == sf::Event::MouseButtonPressed && sendBtn -> isMouseOver(*window)) || 
            (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter && message -> isFocused())){
            
            string outS = "#message " + to_string(convName.second) + " " + message -> getText() + "\n";
            cout << "message = " << message -> getText() << " sent\n";//send req mess
            send(socket, outS);
            message -> setText("");   
        }
        if(event.type == sf::Event::MouseWheelScrolled){
            if(chats.isMouseOver(*window) && !chats.isClear())
                chats.updatePoses(event.mouseWheelScroll.delta);
            if(!msgs.isClear() && convName.second != -1 && msgs.isMouseOver(*window))
                msgs.updatePoses(event.mouseWheelScroll.delta);
            cout << "delta = " << event.mouseWheelScroll.delta << "\n";
        }
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape){
            convName.second = -1;
            convName.first->setText("");
            msgs.clear();
        }
    }
    void mainClean(){
        /*for(int i = 0; i < chats.size(); i++)
            //cout << chats[i]->getText() << "\n";
            delete chats[i];
        chats.clear();*/
        //delete send;
    }
    void logInit(){
        btn->setText("Sign Up");
        log->setSize(window->getSize().x / 3.f, 30);
        log->setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 20);
        pass->setSize(window->getSize().x / 3.f, 30);
        pass->setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 90);
        message->setSize(window->getSize().x / 3.f, 30);
        message->setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f - 50);
        btn->setBold();
        btn->setSize(window->getSize().x / 3.f, 50);
	    btn->setPosition({window->getSize().x / 3.f, window->getSize().y / 2.f + 160});
    }
    void logDraw(){
        btn->drawTo(*window);
        window->draw(*log);
        window->draw(*message);
        window->draw(*pass);
    }
    void logEvents(sf::Event event){
         if(event.type == sf::Event::MouseMoved)
			if (btn -> isMouseOver(*window)) {
				btn -> setBackColor(sf::Color(52, 108, 171));
			}
			else {
				btn -> setBackColor(sf::Color(41, 85, 135));
			}
        if(event.type == sf::Event::MouseButtonPressed)
            if (btn -> isMouseOver(*window)) {
                string outS = "#register " + log -> getText() + " " + pass -> getText() + " " + message -> getText() + "\n";
                send(socket, outS);
                cout << outS << "\n";
                outS = "";
                receive(socket, outS);
                cout << "regback = " << outS << "\n";
                resReg = stoi(outS);
                log->setText(""); pass->setText(""); 
            }
        if (event.type == sf::Event::MouseButtonPressed){
            auto pos = sf::Mouse::getPosition(*window);
            log -> setFocus(false);
            pass -> setFocus(false);
            message -> setFocus(false);
            if (log -> contains(sf::Vector2f(pos))){
                log -> setFocus(true);
            }
            if (pass -> contains(sf::Vector2f(pos))){
                pass -> setFocus(true);
            }
            if (message -> contains(sf::Vector2f(pos))){
                message -> setFocus(true);
            }
        }
        else{
            log -> handleInput(event);
            pass -> handleInput(event);
            message -> handleInput(event);
        }
    }
    void init(){
        if(resAuth != -1){
            page = MAINPAGE;
            resReg = -1;
        }
        if(resReg != -1){
            page = AUTHPAGE;
            resAuth = -1;
        }
        switch (page){
        case AUTHPAGE:
            authInit();
            break;
        case LOGPAGE:
            logInit();
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
            break;
        case LOGPAGE:
            logEvents(e);
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
            authClean();
            break;
        case LOGPAGE:
            logDraw();
            break;
        case MAINPAGE:
            mainClean();
            break;
        }
    }
    ~UI(){
        
    }
};

int main(int argc, char ** argv){
    sf::Socket::Status status = socket.connect("0.0.0.0", stoi(argv[1]));
    if (status != sf::Socket::Done){
        std::cout << "Didn't connect to server\n";
    }
    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "Chat");
    //window.setFramerateLimit(1);
    UI ui(&window);

    sf::View view = window.getDefaultView();
    //window.setVerticalSyncEnabled(true);
    while(window.isOpen()){
        sf::Event event;
        ui.init();//init all shapes
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                socket.disconnect();
            }
            if (event.type == sf::Event::Resized){
                if(window.getSize().y < 400)
                    window.setSize({window.getSize().x, 400});
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            ui.events(event);//event handler
        }
        window.clear(sf::Color(26, 34, 44));
        ui.draw();//draw all shapes
        window.display();
    }
    return 0;
}