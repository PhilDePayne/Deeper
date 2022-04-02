#include "Texture.h"

Texture::Texture(const char* t) {

	data = stbi_load(t, &width, &height, &nrChannels, 0);

}

Texture::~Texture() {

	stbi_image_free(data);

}

unsigned char* Texture::getTexture() {

	return data;

}
