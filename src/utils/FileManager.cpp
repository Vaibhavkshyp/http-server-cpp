#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

const unordered_map<string, string> mimeTypes = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".png", "image/png"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},
    {".ico", "image/x-icon"}
};


struct fileData
{
    bool success;
    string content;
    string type;
};

fileData readFile(const string &fileName)
{
    fileData file;
    file.success = false;
    file.content = "";
    file.type = "application/octet-stream";
    ifstream f(fileName, ios_base::binary);
    if (!f)
    {
        file.type = "text/html";
        return file;
    }
    else
    {
        stringstream buffer;
        buffer << f.rdbuf();
        file.success = true;
        file.content = buffer.str();
        size_t pos = fileName.find_last_of(".");
        if (pos != string::npos)
        {
            string extension = fileName.substr(pos);
            auto it = mimeTypes.find(extension);
            if (it != mimeTypes.end())
                file.type = it->second;
        }
    }
    return file;
}