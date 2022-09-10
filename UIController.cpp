#include "UIController.hpp"

void UIController::InitUI(){
    log.init();
    auth.init();
    chating.init();
}

void UIController::DrawUI(){
    switch (page)
    {
    case LOGPAGE:
        log.draw();
        break;
    
    case AUTHPAGE:
        auth.draw();
        break;
    
    case MAINPAGE:
        chating.draw();
        break;

    default:
        break;
    }
}

void UIController::EventUI(sf::Event event){
    switch (page)
    {
    case LOGPAGE:
        page = log.events(event);
        break;
    
    case AUTHPAGE:
        page = auth.events(event);
        break;

    case MAINPAGE:
        page = chating.events(event);
        break;

    default:
        break;
    }
}