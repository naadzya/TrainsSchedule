#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "Route.h"
#include "Button.h"
#include "Textbox.h"

using namespace std;

void input_route(string filename, stringstream& info)
//Enter information about object in file
{
	ofstream f(filename, ios::binary | ios_base::app);
	if (!f.is_open())
	{
		cout << "The file can't be opened\n";
		exit(0);
	}
	Route obj;
	obj.getdata(info);
	f.write(reinterpret_cast<char*>(&obj), sizeof(obj));
	f.close();
}

bool isEmpty(fstream& stream)
{
	return (stream.eof() || stream.peek() == EOF);
}

string output_route(string filename)
{
	fstream f(filename, ios::binary | ios::in);

	if (!f.is_open())
	{
		cout << "The file can't be opened\n";
		exit(0);
	}

	Route obj;
	string info = "";
	while (!isEmpty(f))
	{
		f.read((char*)(&obj), sizeof(obj));
		info += obj.showdata() + "\n";
	}
	f.close();
	return info;
}

sf::Text allRotesText(std::string filename)
{
	string firstroute = output_route(filename);

	sf::Text text;
	text.setString(firstroute);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color(0, 0, 0));
	text.setPosition(sf::Vector2f(250.0, 10.0));

	return text;
}

sf::Text inputText(sf::Vector2f pos)
{
	ostringstream text;
	text << "Enter the departure station: \n"
		<< "Enter the arrival station: \n" << endl
		<< "Enter the departure weekday, e.g. 'mon': \n" << endl
		<< "Enter the departure time.\nUse space to separate hours and minutes: \n" << endl
		<< "Enter the arrival weekday, e.g. 'fri': \n" << endl
		<< "Enter the arrival time.\nUse space to separate hours and minutes: ";

	sf::Text prev;
	prev.setCharacterSize(20);
	prev.setFillColor(sf::Color(137, 39, 2));
	prev.setPosition(sf::Vector2f(250.0, 10.0));
	prev.setString(text.str());

	return prev;
}

void texttodat(string filename)
// Take data from filename.txt and write if to routes.dat (of class Route)
{
	ifstream ft(filename);   //ft is .txt file
	if (!ft.is_open())
	{
		cout << "The file can't be opened\n";
		exit(0);
	}

	ofstream fr("routes.dat", ios::binary);   //fr is a .dat file

	Route obj;
	string from, to, departday, arrivday;
	int deptime[2], arrtime[2];

	while (!ft.eof())
	{
		ft >> from >> to >> departday >> deptime[0] >> deptime[1] >> arrivday >> arrtime[0] >> arrtime[1];
		obj.setstations(from, to);
		obj.setday(departday, arrivday);
		obj.settime(deptime, arrtime);
		fr.write(reinterpret_cast<char*>(&obj), sizeof(obj));
	}
	ft.close();
	fr.close();
}

void textdown(sf::Text& info)
{
	string infostr = info.getString(), newstr;

	if (infostr.length() != 0)
	{
		int empty_line_pos = infostr.find("\n");
		newstr = infostr.substr(empty_line_pos + 1, infostr.length());
		if (newstr.length() != 0)
			info.setString(newstr);
		else
			info.setString(infostr);
	}
}

void textup(sf::Text& info, string filename)
{
	string allroutes = output_route(filename),
		infostr = info.getString();

	if (infostr.length() != 0)
	{

		int info_pos_in_all = allroutes.find(infostr);
		string differ = allroutes.substr(1, info_pos_in_all - 3);
		int last_empty_line = differ.find_last_of("\n\n");

		if (last_empty_line == -1)
			info.setString(allroutes);
		else
		{
			string newstr = allroutes.substr(last_empty_line + 2, allroutes.length());
			if (newstr.length() != 0)
				info.setString(newstr);
			else
				info.setString(infostr);
		}
	}
}

string findobj(string filename, string fromstr, string tostr)
// Find the route according to the stations
{
	fstream f(filename, ios::binary | ios::in);

	if (!f.is_open())
	{
		cout << "The file can't be opened\n";
		exit(0);
	}

	Route obj, searched_obj;
	searched_obj.setstations(fromstr, tostr);
	string info = "";
	while (!isEmpty(f))
	{
		f.read((char*)(&obj), sizeof(obj));
		if (searched_obj.isEqualto(obj))
		{
			info += obj.showdata() + "\n";
			break;
		}
	}
	f.close();
	return info;
}

sf::Text foundtext(string foundinfo, sf::Vector2f pos, sf::Font &font)
{
	sf::Text infotext;
	infotext.setCharacterSize(20);
	infotext.setPosition(pos);
	infotext.setFont(font);
	infotext.setFillColor(sf::Color::Black);
	
	if (foundinfo.length() > 0)
		infotext.setString(foundinfo);
	else
		infotext.setString("Not found");
	return infotext;
}

void deleteroute(string filename, string fromstr, string tostr)
// Delete the route from the file according to the stations
{
	fstream oldfile(filename, ios::binary | ios::in), 
		newfile("tmprotes.dat", ios::binary | ios::out);
	Route obj, del_obj;
	del_obj.setstations(fromstr, tostr);
	while (!isEmpty(oldfile))
	{
		oldfile.read((char*)(&obj), sizeof(obj));
		if (!del_obj.isEqualto(obj))
		{
			newfile.write((char*)(&obj), sizeof(obj));
		}
		else
		{
			cout << "It's equal\n";
		}
	}
	oldfile.close();
	newfile.close();
	remove(filename.c_str());;
	rename("tmprotes.dat", filename.c_str());
}

int main()
{
	cout << "Start\n";
	texttodat("routes.txt");
	//Create the main rendering window
	sf::RenderWindow trainwindow(sf::VideoMode(1000, 600, 32), "Schedule", sf::Style::Close | sf::Style::Titlebar);

	sf::RectangleShape mainwind(sf::Vector2f(760.0, 600.0));
	mainwind.setFillColor(sf::Color(175, 175, 175));
	mainwind.setPosition(sf::Vector2f(240.0, 0.0));

	//Set font for our text
	sf::Font font;
	if (!font.loadFromFile("LSANS.ttf"))
	{
		cout << "Error" << endl;
	}
	
	// Button for getting all routes
	Button btn1("Get all routes", sf::Vector2f(200.0, 50.0), 20, sf::Color(135, 65, 155), sf::Color::Black);
	btn1.setFont(font);
	btn1.setPosition(sf::Vector2f(20, 20));

	// Button for adding one route
	Button btn2("Add a route", sf::Vector2f(200.0, 50.0), 20, sf::Color(135, 65, 155), sf::Color::Black);
	btn2.setFont(font);
	btn2.setPosition(sf::Vector2f(20, 100));

	sf::Text routestext, addprev, findinfo;

	// Textboxes for adding Route
	Textbox textboxfrom(20, sf::Color::Black, false);
	textboxfrom.setFont(font);
	textboxfrom.setPositon(sf::Vector2f(540.0, 10.0));
	textboxfrom.setLimit(true, 30);

	Textbox textboxto(20, sf::Color::Black, false);
	textboxto.setFont(font);
	textboxto.setPositon(sf::Vector2f(500.0, 35.0));
	textboxto.setLimit(true, 30);

	Textbox boxdepday(20, sf::Color::Black, false);
	boxdepday.setFont(font);
	boxdepday.setPositon(sf::Vector2f(660.0, 80.0));
	boxdepday.setLimit(true, 2);

	Textbox boxdeptime(20, sf::Color::Black, false);
	boxdeptime.setFont(font);
	boxdeptime.setPositon(sf::Vector2f(670.0, 155.0));
	boxdeptime.setLimit(true, 4);

	Textbox boxarrivday(20, sf::Color::Black, false);
	boxarrivday.setFont(font);
	boxarrivday.setPositon(sf::Vector2f(600.0, 200.0));
	boxarrivday.setLimit(true, 2);

	Textbox boxarrivtime(20, sf::Color::Black, false);
	boxarrivtime.setFont(font);
	boxarrivtime.setPositon(sf::Vector2f(670.0, 275.0));
	boxarrivtime.setLimit(true, 4);

	Button submitbtn("Submit", sf::Vector2f(200.0, 50.0), 20, sf::Color(28, 165, 81), sf::Color::Black);
	submitbtn.setFont(font);

	// Button and textboxes for finding one route
	Button btnfind("Find a route", sf::Vector2f(200.0, 50.0), 20, sf::Color(135, 65, 155), sf::Color::Black);
	btnfind.setFont(font);
	btnfind.setPosition(sf::Vector2f(20, 180));

	Textbox finddelboxfrom(20, sf::Color::Black, false);
	finddelboxfrom.setFont(font);
	finddelboxfrom.setPositon(sf::Vector2f(540.0, 10.0));
	finddelboxfrom.setLimit(true, 30);

	Textbox finddelboxto(20, sf::Color::Black, false);
	finddelboxto.setFont(font);
	finddelboxto.setPositon(sf::Vector2f(500.0, 35.0));
	finddelboxto.setLimit(true, 30);

	sf::Text findprev;
	findprev.setFont(font);
	findprev.setString("Enter the departure station: \nEnter the arrival station: \n");
	findprev.setCharacterSize(20);
	findprev.setFillColor(sf::Color(137, 39, 2));
	findprev.setPosition(sf::Vector2f(250.0, 10.0));

	// For deleting the route

	Button btndel("Delete a route", sf::Vector2f(200.0, 50.0), 20, sf::Color(135, 65, 155), sf::Color::Black);
	btndel.setFont(font);
	btndel.setPosition(sf::Vector2f(20, 260));

	bool canwritefile = false, isfound = false, candeleteroute = false;
	int whichbox = 1, whichbtn = 0, whichfindbox = 1;
	stringstream routegetinfo;
	string fromstr, tostr, foundstr;

	while (trainwindow.isOpen())
	{
		sf::Event event;

		if (trainwindow.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				trainwindow.close();
				break;

			case sf::Event::MouseMoved:
			{
				if (btn1.isMouseOver(trainwindow))
				{
					btn1.setBackColor(sf::Color(90, 45, 103));
				}
				else
				{
					btn1.setBackColor(sf::Color(135, 65, 155));
				}

				if (btn2.isMouseOver(trainwindow))
				{
					btn2.setBackColor(sf::Color(90, 45, 103));
				}
				else
				{
					btn2.setBackColor(sf::Color(135, 65, 155));
				}

				if (btnfind.isMouseOver(trainwindow))
				{
					btnfind.setBackColor(sf::Color(90, 45, 103));
				}
				else
				{
					btnfind.setBackColor(sf::Color(135, 65, 155));
				}

				if (btndel.isMouseOver(trainwindow))
				{
					btndel.setBackColor(sf::Color(90, 45, 103));
				}
				else
				{
					btndel.setBackColor(sf::Color(135, 65, 155));
				}

				if (submitbtn.isMouseOver(trainwindow))
				{
					submitbtn.setBackColor(sf::Color(27, 127, 66));
				}
				else
				{
					submitbtn.setBackColor(sf::Color(28, 165, 81));
				}
			}
			break;

			case sf::Event::MouseButtonPressed:
			{
				if (btn1.isMouseOver(trainwindow))
				{
					routestext = allRotesText("routes.dat");
					routestext.setFont(font);
					whichbtn = 1;
				}
				if (btn2.isMouseOver(trainwindow))
				{
					whichbtn = 2;
					addprev = inputText(sf::Vector2f(250.0, 10.0));
					addprev.setFont(font);
					whichbox = 1;
					textboxfrom.setSelected(true);
					whichfindbox = 0;
				}
				if (btnfind.isMouseOver(trainwindow))
				{
					whichbtn = 3;
					whichbox = 0;
					whichfindbox = 1;
					finddelboxfrom.setSelected(true);
				}
				if (btndel.isMouseOver(trainwindow))
				{
					whichbtn = 4;
					whichfindbox = 1;
					whichbox = 0;
					finddelboxfrom.setSelected(true);
				}

				if (submitbtn.isMouseOver(trainwindow))
				{
					switch (whichbtn)
					{
					case 2:
						routegetinfo << textboxfrom.getText() + " " << textboxto.getText() + " "
							<< boxdepday.getText() + " " << boxdeptime.getText() + " "
							<< boxarrivday.getText() + " " << boxarrivtime.getText() + " ";
						canwritefile = true;
						textboxfrom.clean();
						textboxto.clean();
						boxdepday.clean();
						boxdeptime.clean();
						boxarrivday.clean();
						boxarrivtime.clean();
						break;
					case 3:
						fromstr = finddelboxfrom.getText();
						tostr = finddelboxto.getText();
						foundstr = findobj("routes.dat", fromstr, tostr);
						findinfo = foundtext(foundstr, sf::Vector2f(250.0, 80.0), font);
						finddelboxfrom.clean();
						finddelboxto.clean();
						break;
					case 4:
						fromstr = finddelboxfrom.getText();
						tostr = finddelboxto.getText();
						candeleteroute = true;
						finddelboxfrom.clean();
						finddelboxto.clean();
					}
					
				}
			}
			break;

			case sf::Event::TextEntered:
			{
				textboxfrom.setSelected(false);
				textboxto.setSelected(false);
				boxdepday.setSelected(false);
				boxdeptime.setSelected(false);
				boxarrivday.setSelected(false);
				boxarrivday.setSelected(false);

				finddelboxfrom.setSelected(false);
				finddelboxto.setSelected(false);

				switch (whichbox)
				{
				case 1:
					textboxfrom.setSelected(true);
					textboxfrom.typedOn(event);
					break;
				case 2:
					textboxto.setSelected(true);
					textboxto.typedOn(event);
					break;
				case 3:
					boxdepday.setSelected(true);
					boxdepday.typedOn(event);
					break;
				case 4:
					boxdeptime.setSelected(true);
					boxdeptime.typedOn(event);
					break;
				case 5:
					boxarrivday.setSelected(true);
					boxarrivday.typedOn(event);
					break;
				case 6:
					boxarrivtime.setSelected(true);
					boxarrivtime.typedOn(event);
					break;
				default:
					break;
				}

				switch (whichfindbox)
				{
				case 1:
					finddelboxfrom.setSelected(true);
					finddelboxfrom.typedOn(event);
					break;
				case 2:
					finddelboxto.setSelected(true);
					finddelboxto.typedOn(event);
					break;
				default:
					break;
				}			
			}
			break;

			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					textboxfrom.setSelected(false);
					textboxto.setSelected(false);
					boxdepday.setSelected(false);
					boxdeptime.setSelected(false);
					boxarrivday.setSelected(false);
					boxarrivtime.setSelected(false);
				}

				if (event.key.code == sf::Keyboard::Down)
				{
					textdown(routestext);
				}

				if (event.key.code == sf::Keyboard::Up)
				{
					textup(routestext, "routes.dat");
				}

				if (event.key.code == sf::Keyboard::Enter)
				{
					if (whichbtn == 2)
						whichbox++;
					else
						whichfindbox++;				
				}
			}
			break;
			}
		}

		trainwindow.clear(sf::Color(29, 103, 92));
		trainwindow.draw(mainwind);

		switch (whichbtn)
		{
		case 1:
			trainwindow.draw(routestext);
			break;
		case 2:
			trainwindow.draw(addprev);
			textboxfrom.drawTo(trainwindow);
			textboxto.drawTo(trainwindow);
			boxdepday.drawTo(trainwindow);
			boxdeptime.drawTo(trainwindow);
			boxarrivday.drawTo(trainwindow);
			boxarrivtime.drawTo(trainwindow);
			submitbtn.setPosition(sf::Vector2f(700, 320));
			submitbtn.drawTo(trainwindow);
			break;	
		case 3:
			trainwindow.draw(findprev);
			finddelboxfrom.drawTo(trainwindow);
			finddelboxto.drawTo(trainwindow);
			trainwindow.draw(findinfo);
			submitbtn.setPosition(sf::Vector2f(700, 100));
			submitbtn.drawTo(trainwindow);
			break;
		case 4:
			trainwindow.draw(findprev);
			// Use finddelboxes for getting stations for deleting a route
			finddelboxfrom.drawTo(trainwindow);
			finddelboxto.drawTo(trainwindow);
			submitbtn.setPosition(sf::Vector2f(700, 100));
			submitbtn.drawTo(trainwindow);
		}

		if (canwritefile)
		{
			input_route("routes.dat", routegetinfo);
			canwritefile = false;
			whichbox++;
		}

		if (candeleteroute)
		{
			deleteroute("routes.dat", fromstr, tostr);
			candeleteroute = false;
			whichfindbox++;
		}

		btn1.drawTo(trainwindow);
		btn2.drawTo(trainwindow);
		btnfind.drawTo(trainwindow);
		btndel.drawTo(trainwindow);
		trainwindow.display();
	}

	return 0;
}