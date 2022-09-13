#ifndef UIMAINPAGE_HPP
#define UIMAINPAGE_HPP
#include "FormSFML.h"
#include "constants.hpp"

class UIMainPage{
    sf::RenderWindow * window;
    sf::RectangleShape Boxes[4];
    int currentChatId;
    ScrollChat chats;
    ScrollMessages msgs;
    Button logout;
    TextField messageField;
    Button sendMessage;
    Button interlocutor;
public:
    UIMainPage(sf::RenderWindow * wind):
        window(wind),
        msgs(wind, sf::Color::Black, sf::Color::Green),
        chats(wind, sf::Color::Black, sf::Color::Green),
        messageField(500, window, false),
        logout("L", {30, 30}, 15, sf::Color::Blue, sf::Color::White),
        sendMessage("L", {30, 30}, 15, sf::Color::Blue, sf::Color::White),
        interlocutor("", {window -> getSize().x - 0.32f * HEIGHT, 30.0f}, 15, sf::Color(20, 30, 40), sf::Color::White){
        
        logout.setPosition({HEIGHT * 0.26f, 8});
        currentChatId = -1;
    }

    void init();

    void draw();
    
    int events(sf::Event event);
};

#endif