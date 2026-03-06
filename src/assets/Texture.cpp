//
// Created by Orik on 20/09/2025.
//

#include <motor/assets/Texture.h>
#include "stb_image.h"

using namespace Motor;

Texture::~Texture() { stbi_image_free(data); }

const int& Texture::getWidth() const { return width; }
void Texture::setWidth(const int &width) { this->width = width; }

const int& Texture::getHeight() const { return height; }
void Texture::setHeight(const int &height) { this->height = height; }

const int& Texture::getNrChannels() const { return nrChannels; }
void Texture::setNrChannels(const int &nrChannels) { this->nrChannels = nrChannels; }

const bool& Texture::isTransparent() const { return transparent; }
void Texture::setTransparent(const bool &transparent) { this->transparent = transparent; }

const Texture::Filter& Texture::getFilter() const { return filter; }
void Texture::setFilter(const Filter &filter) { this->filter = filter; }

const Texture::Wrap & Texture::getWrap() const { return wrap; }
void Texture::setWrap(const Wrap &wrap) { this->wrap = wrap; }

const Texture::Type & Texture::getTextureType() const { return type; }
void Texture::setTextureType(const Type &type) { this->type = type; }

bool Texture::isColor() const {
    if (type == DIFFUSE || type == CUSTOM)
        return true;
    return false;
}

const unsigned char * Texture::getData() const { return data; }
auto Texture::setData(unsigned char *data) -> void { this->data = data; }

const char * Texture::getType() const { return "Texture"; }

Texture& Texture::GetEmpty() {
    static Texture empty;
    return empty;
}
