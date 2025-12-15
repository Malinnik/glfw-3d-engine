#pragma once
#include <string>

class Texture;

extern Texture* load_texture(std::string filepath);
unsigned char* load_png_data(const char* file, int* width, int* height);
