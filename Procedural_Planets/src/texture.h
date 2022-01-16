#pragma once

#include <string>

class Texture
{
public:
	Texture(std::string path);

	void use();
	unsigned int getId() { return ID; }
private:
	unsigned int ID;
	unsigned char* data;

	void loadFile(std::string path);
};
