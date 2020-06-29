#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;


class Encryption
{
private:
	string deString;
	string enString;

public:
	string getDeString()
	{
		return deString;
	}
	string getEnString()
	{
		return enString;
	}
	void setDeString(string database)
	{
		deString = database;
	}
	void setEnString(string file)
	{
		enString = file;
	}

	void encrypt(std::string key)
	{
		std::string msg = deString;
		// Make sure the key is at least as long as the message
		std::string tmp(key);
		while (key.size() < msg.size())
			key += tmp;

		// And now for the encryption part
		for (std::string::size_type i = 0; i < msg.size(); ++i)
			msg[i] ^= key[i];
		enString = msg;
	}
	void decrypt(std::string key)
	{
		std::string msg = enString;
		// Make sure the key is at least as long as the message
		std::string tmp(key);
		while (key.size() < msg.size())
			key += tmp;

		// And now for the encryption part
		for (std::string::size_type i = 0; i < msg.size(); ++i)
			msg[i] ^= key[i];
		deString = msg;
	}


};