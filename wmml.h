#ifndef WMML_H
#define WMML_H

#include <string>
#include <vector>
#include <fstream>

class wmml
{
private:
    unsigned short int ObjectsInThread = 0;
    std::string filename;
    std::fstream targetFile;
public:
    wmml(std::string& file);
    wmml(std::string& file, int OIT);
    ~wmml();
    
    unsigned short int size();
    bool read(std::vector<std::string>& out);
    bool read();
    void add(std::vector<std::string>& in);
    void reset();
    int sizeRequest();
    void overwriting (int tag, int field, std::string newStr);
private:
    void abcerr (std::string error);
};

#endif