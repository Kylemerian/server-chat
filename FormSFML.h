#ifndef FORMSFML_H
#define FORMSFML_H

#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
 
using namespace std;

enum{
    CENTERED,
    UPPED,
    DOWNED
};
 
class TextField : public sf::Transformable, public sf::Drawable{
public:
    TextField(unsigned int maxChars, sf::RenderWindow * wind, bool hidden) :
    window(wind),
    m_size(maxChars),
    m_rect({1, 1}), // 15 pixels per char, 20 pixels height, you can tweak
    m_hasfocus(false),
    is_hidden(hidden)
    {
        m_font.loadFromFile("ttf/None.ttf"); // I'm working on Windows, you can put your own font instead
        m_rect.setOutlineThickness(2);
        m_rect.setFillColor(sf::Color(26, 34, 44));
        m_rect.setOutlineColor(sf::Color(127,127,127));
        m_rect.setPosition(this->getPosition());
        txt.setFont(m_font);
        txt.setFillColor(sf::Color::White);
        txt.setCharacterSize(15);
    }
    bool isFocused(){
        return m_hasfocus;
    }
    void setPosition(int textPosX, int textPosY){
        m_rect.setPosition({textPosX * 1.0f, textPosY * 1.0f});
    }
    void setSize(float textX, float textY){
        m_rect.setSize({textX, textY});
    }
    const std::string getText() const{
        return m_text;
    }
    void setPosition(float x, float y){
        sf::Transformable::setPosition(x, y);
        m_rect.setPosition(x, y);
    }
    bool contains(sf::Vector2f point) const{
        return m_rect.getGlobalBounds().contains(point);
    }
    void setFocus(bool focus){
        m_hasfocus = focus;
        if (focus){
            m_rect.setOutlineColor(sf::Color::White);
        }
        else{
            m_rect.setOutlineColor(sf::Color(88, 88, 88));
        }
    }
    void setOutline(sf::Color fillColor, sf::Color outlineColor, int thickness){
        m_rect.setFillColor(fillColor);
        m_rect.setOutlineThickness(thickness);
        m_rect.setOutlineColor(outlineColor);
    }
    void setText(std::string s){
        m_text = s;
        txt.setString(m_text);
    }
    string hide(){
        string res = "";
        for(int i = 0; i < m_text.size(); i++)
            res += '*';
        return res;
    }
    void handleInput(sf::Event e){
        if (!m_hasfocus || e.type != sf::Event::TextEntered)
            return;

        if (e.text.unicode == 8){   // Delete key
            m_text = m_text.substr(0, m_text.size() - 1);
        }
        else if(e.text.unicode == 13)
            ;
        else if (m_text.size() < m_size){    
            m_text += e.text.unicode;
        }
        if(is_hidden)
            txt.setString(hide());
        else
            txt.setString(m_text);
        txt.setPosition({m_rect.getPosition().x + 5, m_rect.getPosition().y + 5});
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{ 
        window -> draw(m_rect);
        window -> draw(txt);
    }  
private:
    unsigned int m_size;
    sf::RenderWindow * window;
    sf::Font m_font;
    std::string m_text;
    sf::RectangleShape m_rect;
    bool m_hasfocus;
    bool is_hidden;
    sf::Text txt;
};

class Button {
public:
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor,
        sf::Color textColor) {
		
        button.setSize(buttonSize);
		button.setFillColor(bgColor);
		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;
        font.loadFromFile("ttf/None.ttf");
		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setFillColor(textColor);
        text.setFont(font);
        text.setStyle(sf::Text::Bold);
	}
    void setTexture(sf::Texture & tex){
        button.setTexture(&tex);
    }
    void setBold(){
        text.setStyle(sf::Text::Bold);
    }
    void setText(string s){
        text.setString(s);
    }
    void setOutline(sf::Color fillColor, sf::Color outlineColor, int thickness){
        button.setFillColor(fillColor);
        button.setOutlineThickness(thickness);
        button.setOutlineColor(outlineColor);
    }
	// Pass font by reference:
	void setFont(sf::Font &fonts) {
		text.setFont(fonts);
	}
    void setSize(float x, float y){
        button.setSize({x, y});
        btnWidth = x;
        btnHeight = y;
    }
    sf::Color getBgColor(){
        return button.getFillColor();
    }
	void setBackColor(sf::Color color) {
		button.setFillColor(color);
	}

	void setTextColor(sf::Color color) {
		text.setFillColor(color);
	}
    sf::Vector2f getPosition(){
        return button.getPosition();
    }
    sf::Vector2i getSize(){
        return {btnWidth, btnHeight};
    }
	void setPosition(sf::Vector2f point) {
		button.setPosition(point);
        float xPos;
        float yPos;
		// Center text on button:
        xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}

	void drawTo(sf::RenderWindow &window) {
		window.draw(button);
		window.draw(text);
	}

	// Check if the mouse is within the bounds of the button:
	bool isMouseOver(sf::RenderWindow &window) {
		int mouseX = sf::Mouse::getPosition(window).x;
		int mouseY = sf::Mouse::getPosition(window).y;

		int btnPosX = button.getPosition().x;
		int btnPosY = button.getPosition().y;

		int btnxPosWidth = button.getPosition().x + btnWidth;
		int btnyPosHeight = button.getPosition().y + btnHeight;

		if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
			return true;
		}
		return false;
	}
protected:
	sf::RectangleShape button;
	sf::Text text;
    sf::Font font;
	int btnWidth;
	int btnHeight;
};

class ChatBox : public Button{
    sf::Text chatName;
    int chat_id;
public:
    ChatBox(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor,
        sf::Color textColor, int chatid):Button(btnText, buttonSize, charSize, bgColor, textColor){
        chatName.setString("smthing");
		chatName.setCharacterSize(15);
		chatName.setFillColor(sf::Color::White);
        chatName.setFont(font);
        chatName.setStyle(sf::Text::Bold);
        chat_id = chatid;
    }
    int getChatId(){
        return chat_id;
    }
    string getSenderName(){
        return text.getString();
    }
    void setPosition(sf::Vector2f point) {
		button.setPosition(point);
        float xPos1, xPos2;
        float yPos1, yPos2;
        xPos1 = 10;
		yPos1 = (point.y + btnHeight / 1.5) - (text.getLocalBounds().height / 2);
        xPos2 = 10;
		yPos2 = (point.y + btnHeight / 3.7) - (chatName.getLocalBounds().height / 2);
		text.setPosition(xPos2, yPos2);
        chatName.setPosition(xPos1, yPos1);
	}
    void setChatName(std::string s){
        chatName.setString(s);
    }
    void drawTo(sf::RenderWindow &window) {
		window.draw(button);
		window.draw(text);
        window.draw(chatName);
	}
};

class MessageBox : public Button{
    sf::Text senderName;
    int sender_id;
    string time;
public:
    MessageBox(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor,
        sf::Color textColor, int senderid, string _time):Button(btnText, buttonSize, charSize, bgColor, textColor){
        senderName.setString("smthing");
		senderName.setCharacterSize(15);
		senderName.setFillColor(sf::Color::White);
        senderName.setFont(font);
        senderName.setStyle(sf::Text::Bold);
        text.setStyle(sf::Text::Regular);
        sender_id = senderid;
        time = _time;
    }
    int getSenderId(){
        return sender_id;
    }
    void setPosition(sf::Vector2f point) {
		button.setPosition(point);
        float xPos1, xPos2;
        float yPos1, yPos2;
        xPos1 = 10 + button.getPosition().x;
		yPos1 = (point.y + btnHeight / 1.5) - (text.getLocalBounds().height / 2);
        xPos2 = 10 + button.getPosition().x;
		yPos2 = (point.y + btnHeight / 3.7) - (senderName.getLocalBounds().height / 2);
		text.setPosition(xPos1, yPos1);
        senderName.setPosition(xPos2, yPos2);
	}
    void setSenderName(std::string s){
        senderName.setString(s);
    }
    void drawTo(sf::RenderWindow &window) {
		window.draw(button);
		window.draw(text);
        window.draw(senderName);
	}
};

class ScrollChat{
    std::vector<ChatBox *> contents;
    sf::RenderWindow * window;
public:
    ScrollChat(sf::RenderWindow * wind, sf::Color focusColor, sf::Color unfocusColor):window(wind){
    }
    void addContent(ChatBox * btn){
        contents.push_back(btn);
    }
    void draw(sf::RenderWindow& window){
        for(auto obj : contents){
            obj -> drawTo(window);
        }
    }
    void setFocus(){
        for(auto obj : contents)
            if(obj -> isMouseOver(*window))
                obj -> setBackColor(sf::Color(22, 33, 44));
            else
                obj -> setBackColor(sf::Color(20, 30, 40));
    }
    int amount(){
        return contents.size();
    }
    void clear(){
        contents.clear();
    }
    int isClear(){
        return contents.size() == 0;
    }
    int getIdChat(){
        for(auto obj : contents){
            if(obj->isMouseOver(*window))
                return obj->getChatId();
        }
        return -1;
    }
    string getNameChat(){
        for(auto obj : contents){
            if(obj->isMouseOver(*window))
                return obj->getSenderName();
        }
        return "";
    }
    bool isMouseOver(sf::RenderWindow &window) {
		int mouseX = sf::Mouse::getPosition(window).x;
		int mouseY = sf::Mouse::getPosition(window).y;
       
        int btnPosX = contents[0] -> getPosition().x;
        int btnPosY = contents[0] -> getPosition().y;
        int btnxPosWidth = contents[contents.size() - 1] -> getPosition().x + contents[contents.size() - 1] -> getSize().x;
        int btnyPosHeight = contents[contents.size() - 1] -> getPosition().y + contents[contents.size() - 1] -> getSize().y;
        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
            return true;
        }
        return false;
	}
    void updatePoses(float delta){
        delta = - delta;
        int btnPosY = contents[0] -> getPosition().y;
        int btnyPosHeight = contents[contents.size() - 1] -> getPosition().y + contents[contents.size() - 1] -> getSize().y;
        
        if(btnPosY > 40 && btnyPosHeight < window -> getSize().y)
            delta = 0;
        else if(btnyPosHeight*1.0f - window -> getSize().y - delta * 30 + 16 < 0){
            delta = 0;
        }
        else if(btnPosY - delta * 30 > 768 /18 + 10){
            delta = 0;
        }
        for(int i = 0; i < contents.size(); i++){
            contents[i] -> setPosition({1.0f * contents[i]->getPosition().x, contents[i]->getPosition().y - delta*30});
        }    
    }
};

class ScrollMessages{
    std::vector<MessageBox *> contents;
    sf::RenderWindow * window;
public:
    ScrollMessages(sf::RenderWindow * wind, sf::Color focusColor, sf::Color unfocusColor):window(wind){
    }
    void addContent(MessageBox * btn){
        contents.push_back(btn);
    }
    void draw(sf::RenderWindow& window){
        for(auto obj : contents){
            obj -> drawTo(window);
        }
    }
    void setFocus(){
        for(auto obj : contents)
            if(obj -> isMouseOver(*window))
                obj -> setBackColor(sf::Color(22, 33, 44));
            else
                obj -> setBackColor(sf::Color(20, 30, 40));
    }
    float getLowerBounce(){
        return contents[contents.size() - 1]->getPosition().y;
    }
    int amount(){
        return contents.size();
    }
    void clear(){
        contents.clear();
    }
    int isClear(){
        return contents.size() == 0;
    }
    int getIdChat(){
        for(auto obj : contents){
            if(obj->isMouseOver(*window))
                return obj->getSenderId();
        }
        return -1;
    }
    bool isMouseOver(sf::RenderWindow &window) {
		int mouseX = sf::Mouse::getPosition(window).x;
		int mouseY = sf::Mouse::getPosition(window).y;
       
        int btnPosX = contents[0] -> getPosition().x;
        int btnPosY = contents[0] -> getPosition().y;
        int btnxPosWidth = contents[contents.size() - 1] -> getPosition().x + contents[contents.size() - 1] -> getSize().x;
        int btnyPosHeight = contents[contents.size() - 1] -> getPosition().y + contents[contents.size() - 1] -> getSize().y;
        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
            return true;
        }
        return false;
	}
    void updatePoses(float delta){
        delta = - delta;
        int btnPosY = contents[0] -> getPosition().y;
        int btnyPosHeight = contents[contents.size() - 1] -> getPosition().y + contents[contents.size() - 1] -> getSize().y;
        
        if(btnPosY > 40 && btnyPosHeight < window -> getSize().y - 50)
            delta = 0;
        else if(btnyPosHeight*1.0f - window -> getSize().y - delta * 30 + 16 < -50){
            delta = 0;
        }
        else if(btnPosY - delta * 30 > 768 /18 + 10){
            delta = 0;
        }
        for(int i = 0; i < contents.size(); i++){
            contents[i] -> setPosition({1.0f * contents[i]->getPosition().x, contents[i]->getPosition().y - delta*30});
        }    
    }
};

#endif