#include "UIMainPage.hpp"
#include "FormSFML.h"
#include <iostream>
#include "constants.hpp"
#include <SFML/Network.hpp>
#include <vector>
#include "sockets.hpp"

extern sf::TcpSocket socket;
int myId;
string myName;

void UIMainPage::init(){
    for(int i = 0; i < 4; i++){
        Boxes[i].setOutlineThickness(4);
        Boxes[i].setOutlineColor(sf::Color(20, 30, 40));
        Boxes[i].setFillColor(sf::Color(14, 22, 33));
    }
    Boxes[0].setSize({HEIGHT * 0.3f, window -> getSize().y * 1.0f});
    Boxes[1].setSize({HEIGHT * 0.3f, WIDTH /18});
    
    Boxes[2].setFillColor(sf::Color(14, 22, 33));
    Boxes[2].setSize({window -> getSize().x * 0.7f, window -> getSize().y * 1.0f});
    Boxes[3].setSize({window -> getSize().x * 0.7f, WIDTH /18});
    Boxes[2].setPosition({HEIGHT * 0.3f, 0});
    Boxes[3].setPosition({HEIGHT * 0.3f, 0});
}

void UIMainPage::draw(){
    window -> draw(Boxes[0]);
    window -> draw(Boxes[2]);
    
    interlocutor.setSize(window -> getSize().x - 0.30f * HEIGHT, WIDTH /18);
    interlocutor.setPosition({HEIGHT * 0.30f, 0.0f});
    
    sendMessage.setPosition({1.0f * window -> getSize().x - 34, 1.0f * window -> getSize().y - 40});
    messageField.setPosition(HEIGHT * 0.31f, window -> getSize().y - 40);
    messageField.setSize(window -> getSize().x - 0.35f * HEIGHT, 30.0f);
    
    vector <pair<int, string>> parsedChats; //temporary filled that way
    parsedChats.push_back(std::make_pair(1, "chatname1"));
    parsedChats.push_back(std::make_pair(3, "chatname3"));
    parsedChats.push_back(std::make_pair(5, "chatname5"));
    parsedChats.push_back(std::make_pair(1, "chatname1"));
    parsedChats.push_back(std::make_pair(3, "chatname3"));
    parsedChats.push_back(std::make_pair(5, "chatname5"));
    parsedChats.push_back(std::make_pair(1, "chatname1"));
    parsedChats.push_back(std::make_pair(3, "chatname3"));
    parsedChats.push_back(std::make_pair(5, "chatname5"));
    parsedChats.push_back(std::make_pair(1, "chatname1"));
    parsedChats.push_back(std::make_pair(3, "chatname3"));
    parsedChats.push_back(std::make_pair(5, "chatname5"));
    vector<pair<pair<string, int>, pair<string, string>>> parsedMsgs;//temporary filled that way
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello1", "2000-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello2", "2010-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello3", "2020-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello4", "2030-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello5", "2040-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello6", "2050-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello7", "2060-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello8", "2070-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello9", "2080-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello1", "2000-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello2", "2010-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello3", "2020-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello4", "2030-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello5", "2040-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello6", "2050-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender2", 2), make_pair("Hello7", "2060-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello8", "2070-10-10 00:01:02")));
    parsedMsgs.push_back(make_pair(make_pair("Sender1", 1), make_pair("Hello9", "2080-10-10 00:01:02")));
    if(chats.isClear()){
        for(int i = 0; i < parsedChats.size(); i++){
            ChatBox * tmp = new ChatBox(parsedChats[i].second, {HEIGHT * 0.3f - 5.0f, 65.0f}, 15, sf::Color(14, 22, 80),
                sf::Color::White, parsedChats[i].first);

            tmp -> setPosition({0.0f, i * 70 * 1.0f + WIDTH /18 + 5});
            chats.addContent(tmp);
        }
    }
    if(msgs.isClear() && currentChatId != -1){
        for(int i = 0; i < parsedMsgs.size(); i++){
                MessageBox * tmp = new MessageBox(parsedMsgs[parsedMsgs.size() - 1 - i].second.first, {HEIGHT * 0.45f, 45.0f}, 13, sf::Color(20, 30, 40),
                    sf::Color::White, parsedMsgs[parsedMsgs.size() - 1 - i].first.second, parsedMsgs[parsedMsgs.size() - 1 - i].second.second);
                tmp -> setPosition({0.31f * HEIGHT, i * 50 * 1.0f + WIDTH /18 + 10});
                tmp -> setSenderName(parsedMsgs[parsedMsgs.size() - 1 - i].first.first);
                msgs.addContent(tmp);
            }
    }
    chats.draw(*window);
    msgs.draw(*window);
    window -> draw(Boxes[1]);
    window -> draw(Boxes[3]);
    logout.drawTo(*window);
    if(currentChatId != -1){
        window -> draw(messageField);
        sendMessage.drawTo(*window);
        interlocutor.drawTo(*window);
    }
}

int UIMainPage::events(sf::Event event){
    if(event.type == sf::Event::MouseWheelScrolled){
        if(chats.isMouseOver(*window) && !chats.isClear())
            chats.updatePoses(event.mouseWheelScroll.delta);
        if(!msgs.isClear() && currentChatId != -1 && msgs.isMouseOver(*window))
            msgs.updatePoses(event.mouseWheelScroll.delta);
    
    }
    if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape){
        currentChatId = -1;
        msgs.clear();
    }

    if(event.type == sf::Event::MouseButtonPressed){
        if (chats.isMouseOver(*window)) {
            cout << "click chat: " << chats.getIdChat() << "\n";//send req to open chat
            currentChatId = chats.getIdChat();
            if(currentChatId != -1)
                interlocutor.setText(chats.getNameChat());
            msgs.clear();
        }
        if(logout.isMouseOver(*window)){
            msgs.clear();
            chats.clear();
            return AUTHPAGE;
        }
    }

    if (event.type == sf::Event::MouseButtonPressed){
        auto pos = sf::Mouse::getPosition(*window);
        messageField.setFocus(false);
        if (messageField.contains(sf::Vector2f(pos)))
            messageField.setFocus(true);
    }
    else
        messageField.handleInput(event);

    if((event.type == sf::Event::MouseButtonPressed && sendMessage.isMouseOver(*window)) || 
            (event.type == sf::Event::KeyReleased &&
            event.key.code == sf::Keyboard::Enter && messageField.isFocused()))
        if(messageField.getText() != ""){
            cout << "message = " << messageField.getText() << " sent\n";//send req mess
            MessageBox * tmp = new MessageBox(messageField.getText(), {HEIGHT * 0.45f, 45.0f}, 13, sf::Color(20, 30, 40),
                    sf::Color::White, 2, "2020-20-01 01:01:10");
            tmp -> setSenderName("SENDERSFDSKFDSFDSFDLF");
            tmp -> setPosition({0.31f * HEIGHT, msgs.getLowerBounce() + 50});
            msgs.addContent(tmp);
            string request = "#message " + to_string(currentChatId) + " " + messageField.getText() + "\n";
            if(send(socket, request) != sf::Socket::Done)
                cout << "cant send message to server\n";
            messageField.setText("");
        }

    if(event.type == sf::Event::MouseMoved){
        chats.setFocus();
        msgs.setFocus();
        if(logout.isMouseOver(*window))
            logout.setBackColor(sf::Color::Red);
        else
            logout.setBackColor(sf::Color::Blue);
        
        if(sendMessage.isMouseOver(*window))
            sendMessage.setBackColor(sf::Color::Red);
        else
            sendMessage.setBackColor(sf::Color::Blue);
    }
    return MAINPAGE;
}