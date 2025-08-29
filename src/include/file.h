#pragma once
#include <string>
#include <fstream>
class FileReader 
{
    public:
        FileReader(const char *path);
        ~FileReader();

        std::string getText();
        static std::string getBinary(const char *path);
        static std::string getText(const std::string& path);

    private:
        std::string _path;
        std::string _text;
        std::ifstream _file;

};
