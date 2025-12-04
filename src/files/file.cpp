#include "files/file.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

FileReader::FileReader(const char *path) : _path(path){}

FileReader::~FileReader()
{
    if (_file.is_open()){
        _file.close();
    }
}

std::string FileReader::getText()
{
    return std::string();
}

std::string FileReader::readBinary(const char *path)
{
    std::ifstream fin(path, std::ios::binary);
    if (!fin) throw std::runtime_error("Failed to open file");
    
    fin.seekg(0, std::ios::end);
    std::streamsize size = fin.tellg();
    if (size <= 0) return ""; // Пустой файл или ошибка
    
    fin.seekg(0, std::ios::beg);
    std::string data(size, '\0');
    if (!fin.read(&data[0], size)) throw std::runtime_error("Read error");
    return data;
}

std::string FileReader::readText(const std::string &path)
{
    std::string fileText;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        file.close();
        fileText = fileStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Ошибка чтения файла: " << e.what() << std::endl;
    }

    return fileText;
}
