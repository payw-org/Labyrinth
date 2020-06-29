#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;


class File
{
private:
	string textline;

public:
	void setText(string test)
	{
		textline = test;
		return;
	}
	string getText()
	{

		return textline;
	}

	void writeFile()
	{

        std::ofstream out("/Users/Royal/labyrinth.txt");
		out << textline;
		out.close();
		return;


	}
	void readFile()
	{
		ifstream nameFileout;
        nameFileout.open("/Users/Royal/labyrinth.txt");
		string line;
		while (std::getline(nameFileout, line))
		{
			textline.append(line);
			textline.append("\n");
		}
	
	}

};
