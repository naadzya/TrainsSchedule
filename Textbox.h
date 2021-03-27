#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define BACKSPACE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

using namespace std;

class Textbox
{
private:
	sf::Text textbox;
	ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit;

	void deleteLastChar()
	{
		string textstr = text.str();
		string newstr = "";

		for (int i = 0; i < textstr.length() - 1; i++)
			newstr += textstr[i];
		text.str("");   // Clear text
		text << newstr;

		textbox.setString(text.str() + "_");
	}

	void logicOfInput(int charTyped)
	// The way the text input works
	{
		if (charTyped != BACKSPACE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
			text << static_cast<char>(charTyped);
		else if (charTyped == BACKSPACE_KEY && (text.str().length() > 0))
			deleteLastChar();
		if (charTyped == ENTER_KEY)
			isSelected = false;

		// Set the textbox text, _ keeps track on where we are in the textbox
		textbox.setString(text.str() + "_");
	}

public:
	Textbox(int size, sf::Color textcolor, bool sel)
	{
		textbox.setCharacterSize(size);
		textbox.setFillColor(textcolor);
		isSelected = sel;

		// Check if the textbox is selected upon creation and display it accordingly:
		if (isSelected)
			textbox.setString("_");
		else
			textbox.setString("");
	}

	void setFont(sf::Font& font)
	{
		textbox.setFont(font);
	}

	void setPositon(sf::Vector2f pos)
	{
		textbox.setPosition(pos);
	}

	void setLimit(bool ToF, int lim)
	// ToF <=> true or false
	{
		hasLimit = ToF;
		limit = lim;
	}

	void setSelected(bool sel)
	// The text has _ if it is selected
	{
		isSelected = sel;
		if (!sel)
		{
			string textstr = text.str();
			string newstr = "";

			for (int i = 0; i < textstr.length(); i++)
				newstr += textstr[i];

			textbox.setString(newstr);
		}
	}

	string getText()
	{
		return text.str();
	}

	void drawTo(sf::RenderWindow& window) 
	{
		window.draw(textbox);
	}

	void typedOn(sf::Event input)
	{
		if (isSelected)
		{
			int charTyped = input.text.unicode;
			if (charTyped < 128)
			{
				if (hasLimit)
				{
					// If there's a limit, don't go over it:
					if (text.str().length() <= limit)
						logicOfInput(charTyped);
					else if ((text.str().length() > limit) && charTyped == BACKSPACE_KEY)
						deleteLastChar();
				}
				// If no limit exists, just run the function:
				else
					logicOfInput(charTyped);
			}
		}
	}

	void clean()
	{
		text.str("");
	}
};