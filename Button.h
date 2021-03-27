#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

class Button
{
private:
	sf::RectangleShape button;
	sf::Text text;

	int btnWidth;
	int btnHeight;

public:
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor)
	{
		button.setSize(buttonSize);
		button.setFillColor(bgColor);

		// For position of the text:
		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;

		text.setString(btnText);
		text.setFillColor(textColor);
		text.setCharacterSize(charSize);
	}

	void setFont(sf::Font& fonts)
	{
		text.setFont(fonts);
	}

	void setBackColor(sf::Color color) 
	{
		button.setFillColor(color);
	}

	void setTextColor(sf::Color color)
	{
		text.setFillColor(color);
	}

	void setPosition(sf::Vector2f point) 
	{
		button.setPosition(point);

		// Get position of the text
		float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}

	void drawTo(sf::RenderWindow& window)
	{
		window.draw(button);
		window.draw(text);
	}

	bool isMouseOver(sf::RenderWindow& window)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int mouseY = sf::Mouse::getPosition(window).y;

		int bntPosX = button.getPosition().x;
		int btnPosY = button.getPosition().y;

		int btnxPosWidth = bntPosX + btnWidth;
		int btnyPosHeight = btnPosY + btnHeight;

		return (mouseX < btnxPosWidth && mouseX > bntPosX && mouseY < btnyPosHeight && mouseY > btnPosY);
	}
};