#include <string>
#include <SFML/Graphics.hpp>
 
using namespace std;
 
class TextField : public sf::Transformable, public sf::Drawable{
public:
    TextField(unsigned int maxChars, sf::RenderWindow * wind) :
    window(wind),
    m_size(maxChars),
    m_rect({1, 1}), // 15 pixels per char, 20 pixels height, you can tweak
    m_hasfocus(false)
    {
    m_font.loadFromFile("ttf/None.ttf"); // I'm working on Windows, you can put your own font instead
    m_rect.setOutlineThickness(2);
    m_rect.setFillColor(sf::Color(26, 34, 44));
    m_rect.setOutlineColor(sf::Color(127,127,127));
    m_rect.setPosition(this->getPosition());
    }
    void setPosition(int textPosX, int textPosY){
        m_rect.setPosition({textPosX, textPosY});
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
            m_rect.setOutlineColor(sf::Color(88, 88, 88)); // Gray color
        }
    }
    void handleInput(sf::Event e){
        if (!m_hasfocus || e.type != sf::Event::TextEntered)
            return;

        if (e.text.unicode == 8){   // Delete key
            m_text = m_text.substr(0, m_text.size() - 1);
        }
        else if (m_text.size() < m_size){
            m_text += e.text.unicode;
        }
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{ 
        window -> draw(m_rect);
        sf::Text txt;
        txt.setFont(m_font);
        txt.setColor(sf::Color::White);
        txt.setString(m_text);
        txt.setCharacterSize(25);
        txt.setPosition({m_rect.getPosition().x + 5, m_rect.getPosition().y - 2});
        window -> draw(txt);
    }  
private:
    unsigned int m_size;
    sf::RenderWindow * window;
    sf::Font m_font;
    std::string m_text;
    sf::RectangleShape m_rect;
    bool m_hasfocus;
};

class Button {
public:
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
		button.setSize(buttonSize);
		button.setFillColor(bgColor);

		// Get these for later use:
		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;

		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setColor(textColor);
	}
    void setBold(){
        text.setStyle(sf::Text::Bold);
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
		text.setColor(color);
	}

	void setPosition(sf::Vector2f point) {
		button.setPosition(point);

		// Center text on button:
		float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
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
private:
	sf::RectangleShape button;
	sf::Text text;

	int btnWidth;
	int btnHeight;
};