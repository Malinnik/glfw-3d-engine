#pragma once
#include <string>
#include <fstream>
class FileReader 
{
    public:
        FileReader(const char *path);
        ~FileReader();

        std::string getText();
        static std::string readBinary(const char *path);
        static std::string readText(const std::string& path);

    private:
        std::string _path;
        std::string _text;
        std::ifstream _file;

};
