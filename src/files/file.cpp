#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "file.h"
#include <loguru.hpp>
#include "fmt/format.h"

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

std::string FileReader::readBinary(const char* path)
{
    std::ifstream fin(path, std::ios::binary);
    if (!fin)
    {
        throw std::runtime_error("Failed to open file");
        LOG_F(INFO, fmt::format("Failed to open file {}", path).c_str());
    } 
    
    fin.seekg(0, std::ios::end);
    std::streamsize size = fin.tellg();
    if (size <= 0) return ""; // Пустой файл или ошибка
    
    fin.seekg(0, std::ios::beg);
    std::string data(size, '\0');
    if (!fin.read(&data[0], size)) throw std::runtime_error("Read error");
    return data;
}

void FileReader::readBinary(const char* path, char* data, size_t size)
{
    std::ifstream output(path, std::ios::binary);
    if (!output.is_open())
    {
        throw "Failed to open file";
        LOG_F(INFO, fmt::format("Failed to open file {}", path).c_str());
    }
    output.read(data, size);
    output.close();
}


void FileReader::writeBinary(const char* path, const char* data, size_t size)
{
    std::ofstream output(path, std::ios::binary);
	if (!output.is_open())
    {
		throw "Error in opening file";
        LOG_F(INFO, fmt::format("Failed to open file {}", path).c_str());
    }
	output.write(data, size);
	output.close();
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
