#include <string>
#include <fstream>
class File 
{
    public:
        File(const char *path);
        ~File();

        
        void overwriteFile(const char *text);
        void appendFile();
        std::string getText();
        static std::string getText(const char *path);
        void saveToMemory();


    private:
        std::string _text;
        std::ifstream _file;

};