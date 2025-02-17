#include "file.h"
#include <string>

File::File(const char *path) : _file(path){}

File::~File()
{
    _file.close();
}

std::string File::getText()
{
    // TODO: read file from stream
    std::string text;
    while (!_file.eof())
        text += _file.get();
    return text;
}

std::string File::getText(const char *path)
{
    std::ifstream file(path);
    // TODO: read file from stream
    file.close();
    return std::string();
}
