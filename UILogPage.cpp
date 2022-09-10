#include "UILogPage.hpp"
#include "FormSFML.h"
#include <iostream>
#include "constants.hpp"
#include <SFML/Network.hpp>
#include "sockets.hpp"

extern sf::TcpSocket socket;

void UILogPage::init(){
    sendReg.setBold();
}

void UILogPage::draw(){
    login.setSize(window->getSize().x / 3.f, 30);
    login.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 30);
    pass.setSize(window->getSize().x / 3.f, 30);
    pass.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f + 100);
    nickname.setSize(window->getSize().x / 3.f, 30);
    nickname.setPosition(window->getSize().x / 3.f, window->getSize().y / 2.f - 40);
    sendReg.setSize(window->getSize().x / 3.f, 50);
	sendReg.setPosition({window->getSize().x / 3.f, window->getSize().y / 2.f + 170});
    
    window -> draw(login);
    window -> draw(pass);
    window -> draw(nickname);
    sendReg.drawTo(*window);
}

int UILogPage::events(sf::Event event){
    if(event.type == sf::Event::MouseMoved)
        if (sendReg.isMouseOver(*window))
            sendReg.setBackColor(sf::Color(52, 108, 171));
        else 
            sendReg.setBackColor(sf::Color(41, 85, 135));
    
    if(event.type == sf::Event::MouseButtonPressed)
        if (sendReg.isMouseOver(*window)) {
            int status = 0;
            string request = "#register " + nickname.getText() +
                " " + login.getText() + " " + pass.getText() + "\n";
            if(send(socket, request) != sf::Socket::Done)
                cout << "error in registration\n";
            if(receive(socket, request) != sf::Socket::Done)
                cout << "error in receive in reg\n";
            vector<string> args = getArgs(request);
            login.setText("");
            pass.setText(""); 
            nickname.setText("");
            if(args[1] == "0")
                return AUTHPAGE;
            else
                return LOGPAGE;
        }
    
    if (event.type == sf::Event::MouseButtonPressed){
        auto pos = sf::Mouse::getPosition(*window);
        login.setFocus(false);
        pass.setFocus(false);
        nickname.setFocus(false);

        if (login.contains(sf::Vector2f(pos)))
            login.setFocus(true);

        if (pass.contains(sf::Vector2f(pos)))
            pass.setFocus(true);

        if (nickname.contains(sf::Vector2f(pos)))
            nickname.setFocus(true);
    }
    else{
        login.handleInput(event);
        pass.handleInput(event);
        nickname.handleInput(event);
    }
    return LOGPAGE;
}