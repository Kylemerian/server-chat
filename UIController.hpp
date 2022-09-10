#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

#include "constants.hpp"
#include "UILogPage.hpp"
#include "UIAuthPage.hpp"
#include "UIMainPage.hpp"
#include <SFML/Graphics.hpp>

class UIController{
    int page;
    UILogPage log;
    UIAuthPage auth;
    UIMainPage chating;
    sf::RenderWindow *window;
public:
    UIController(sf::RenderWindow *wind):
        window(wind),
        log(wind),
        auth(wind),
        chating(wind),
        page(AUTHPAGE){};

    void InitUI();

    void DrawUI();

    void EventUI(sf::Event event);
};

#endif