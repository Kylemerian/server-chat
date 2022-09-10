#ifndef UIAUTHPAGE_HPP
#define UIAUTHPAGE_HPP
#include "FormSFML.h"

class UIAuthPage{
    sf::RenderWindow * window;
    TextField login;
    TextField pass;
    Button toReg;
    Button sendReg;
public:
    UIAuthPage(sf::RenderWindow * wind):
        window(wind),
        login(30, window, false),
        pass(30, window, true),
        sendReg("Sign In", {50, 50}, 15, sf::Color(41, 85, 135), sf::Color::White),
        toReg("Sign Up", {50, 50}, 15, sf::Color(41, 85, 135), sf::Color::White){}

    void init();

    void draw();
    
    int events(sf::Event event);
};

#endif