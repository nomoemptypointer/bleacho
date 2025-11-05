#ifndef BLEACHO_ARGUMENTS_H
#define BLEACHO_ARGUMENTS_H
#include <string>
#include <unordered_map>

class arguments {
    public:
    static bool get(const std::string &key, std::string &outValue);
    static void parse(int argc, char* argv[]);

    private:
    static std::unordered_map<std::string, std::string> args;
};


#endif