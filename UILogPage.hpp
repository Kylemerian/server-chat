#ifndef UILOGPAGE_HPP
#define UILOGPAGE_HPP
#include "FormSFML.h"

class UILogPage{
    sf::RenderWindow * window;
    TextField login;
    TextField pass;
    TextField nickname;
    Button sendReg;
public:
    UILogPage(sf::RenderWindow * wind):
        window(wind),
        login(30, window, false),
        pass(30, window, true),
        nickname(30, window, false),
        sendReg("Register", {50, 50}, 15, sf::Color(41, 85, 135), sf::Color::White){}

    void init();

    void draw();
    
    int events(sf::Event event);
};

#endif