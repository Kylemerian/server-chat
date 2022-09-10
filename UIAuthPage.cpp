#include "UIAuthPage.hpp"
#include "FormSFML.h"
#include <iostream>
#include "constants.hpp"
#include <SFML/Network.hpp>
#include "sockets.hpp"

extern sf::TcpSocket socket;
extern int myId;

void UIAuthPage::init(){
    sendReg.setBold();
    toReg.setBold();
}

void UIAuthPage::draw(){
    login.setSize(window->getSize().x / 3.f, 30);
    login.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f - 40);
    pass.setSize(window->getSize().x / 3.f, 30);
    pass.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 30);
    sendReg.setSize(window->getSize().x / 3.f, 50);
	sendReg.setPosition({window->getSize().x / 3.f, window->getSize().y / 2.f + 100});
    toReg.setSize(window->getSize().x / 3.f, 50);
	toReg.setPosition({window->getSize().x / 3.f, window->getSize().y / 2.f + 170});
    
    window -> draw(login);
    window -> draw(pass);
    sendReg.drawTo(*window);
    toReg.drawTo(*window);
}

int UIAuthPage::events(sf::Event event){
    if(event.type == sf::Event::MouseMoved){
        if (sendReg.isMouseOver(*window))
            sendReg.setBackColor(sf::Color(52, 108, 171));
        else 
            sendReg.setBackColor(sf::Color(41, 85, 135));
        
        if (toReg.isMouseOver(*window))
            toReg.setBackColor(sf::Color(52, 108, 171));
        else 
            toReg.setBackColor(sf::Color(41, 85, 135));
    }
    
    if(event.type == sf::Event::MouseButtonPressed)
        if (sendReg.isMouseOver(*window)) {
            string request = "#auth " + login.getText() + " " + pass.getText() + "\n";
            if(send(socket, request) != sf::Socket::Done)
                cout << "error sending auth\n";
            if(receive(socket, request) != sf::Socket::Done)
                cout << "error receiving id in auth\n";
            cout << request << "\n";
            login.setText("");
            pass.setText(""); 
            vector <string> args = getArgs(request);
            myId = stoi(args[1]);
            if(myId == -1)
                return AUTHPAGE;
            else
                return MAINPAGE;
        }
        else if(toReg.isMouseOver(*window))
            return LOGPAGE;
    
    if (event.type == sf::Event::MouseButtonPressed){
        auto pos = sf::Mouse::getPosition(*window);
        login.setFocus(false);
        pass.setFocus(false);

        if (login.contains(sf::Vector2f(pos)))
            login.setFocus(true);

        if (pass.contains(sf::Vector2f(pos)))
            pass.setFocus(true);
    }
    else{
        login.handleInput(event);
        pass.handleInput(event);
    }
    return AUTHPAGE;
}