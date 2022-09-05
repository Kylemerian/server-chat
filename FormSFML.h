#include <string>
#include <SFML/Graphics.hpp>
 
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
        txt.setCharacterSize(25);
        
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
    void handleInput(sf::Event e){
        if (!m_hasfocus || e.type != sf::Event::TextEntered)
            return;

        if (e.text.unicode == 8){   // Delete key
            m_text = m_text.substr(0, m_text.size() - 1);
        }
        else if (m_text.size() < m_size){
            if(!is_hidden)
                m_text += e.text.unicode;
            else
                m_text += "*";
        }
        txt.setString(m_text);
        txt.setPosition({m_rect.getPosition().x + 5, m_rect.getPosition().y - 2});
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
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor, int tPos, std::string btnSubText = "") {
		button.setSize(buttonSize);
		button.setFillColor(bgColor);
		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;
        textPoss = tPos;
        font.loadFromFile("ttf/None.ttf");
        subText.setString(btnSubText);
        subText.setCharacterSize(15);
        subText.setFillColor(textColor);
        subText.setFont(font);
		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setFillColor(textColor);
        text.setFont(font);
        text.setStyle(sf::Text::Bold);
	}
    void setBold(){
        text.setStyle(sf::Text::Bold);
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
		if(textPoss == CENTERED){
            xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		    yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
        }
        else if(textPoss == UPPED){
            xPos = (point.x + 15);
		    yPos = (point.y + btnHeight / 4) - (text.getLocalBounds().height / 2);
        }
        else if(textPoss == DOWNED){
            xPos = (point.x + 15);
		    yPos = (point.y + btnHeight / 1.5) - (text.getLocalBounds().height / 2);
        }
        subText.setPosition((point.x + 15), (point.y + btnHeight / 1.5) - (subText.getLocalBounds().height / 2));
		text.setPosition(xPos, yPos);
	}

	void drawTo(sf::RenderWindow &window) {
		window.draw(button);
		window.draw(text);
        if(subText.getString() != "")
            window.draw(subText);
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
    string getText(){
        return text.getString();
    }
private:
	sf::RectangleShape button;
	sf::Text text;
    sf::Text subText;
    sf::Font font;
	int btnWidth;
	int btnHeight;
    int textPoss;
};

class Scroll{
    std::vector<Button *> contents;
    sf::RenderWindow * window;
public:
    Scroll(sf::RenderWindow * wind):window(wind){}
    void addContent(Button * btn){
        contents.push_back(btn);
    }
    void draw(sf::RenderWindow& window){
        for(int i = 0; i < contents.size(); i++)
            if (contents[i] -> isMouseOver(window))
                contents[i] -> setBackColor(sf::Color(30, 38, 48));
            else
                contents[i] -> setBackColor(sf::Color(26, 34, 44));
        for(auto obj : contents){
            obj -> drawTo(window);
        }
    }
    int amount(){
        return contents.size();
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
        else if(btnyPosHeight*1.0f - window -> getSize().y - delta * 20 + 16 < 0){
            delta = 0;
        }
        else if(btnPosY - delta * 20 > 1024 * 0.3f - 5.0f){
            delta = 0;
        }
        /*if(btnyPosHeight - delta * 20 > window -> getSize().y)
            delta = (window -> getSize().y - btnyPosHeight) / 20;*/
        for(int i = 0; i < contents.size(); i++){
            contents[i] -> setPosition({0.0f, contents[i]->getPosition().y - delta*20});
        }    
    }
};

vector<pair<string, string>> parseChats(string s){
    vector<pair<string, string>> res;
    string tmpName, tmpMess;
    int flag = 0;
    for(auto c : s){
        if(c == '\n'){
            res.push_back(make_pair(tmpName, tmpMess));
            tmpName = "";
            tmpMess = "";
            flag = 0;
        }
        else{
            if(c == ' ')
                flag = 1;
            else{
                if(flag)
                    tmpMess += c;
                else
                    tmpName += c;
            }
            
        }
    }
    return res;
}