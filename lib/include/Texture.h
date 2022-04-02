#pragma once

#include <stb_image.h>
#include <string>

class Texture {

	int width, height, nrChannels;

	unsigned char* data;

public:

	Texture(const char* t); //TODO: const char na inna zmienna z jakiegoœ pliku
	~Texture();

	unsigned char* getTexture();

};