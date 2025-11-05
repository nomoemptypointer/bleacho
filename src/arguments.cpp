#include "arguments.h"
#include <regex>

std::unordered_map<std::string, std::string> arguments::args;

bool arguments::get(const std::string& key, std::string& outValue)
{
    auto it = args.find(key);
    if (it == args.end())
        return false;

    outValue = it->second;
    return true;
}

void arguments::parse(int argc, char* argv[]) {
    std::regex pattern(R"delim(^([^=]+)=(?:"([^"]*)"|(.*))$)delim");

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        std::smatch match;

        // Match: key="value" OR key=value
        if (std::regex_match(arg, match, pattern)) {
            std::string key = match[1];
            std::string value;

            if (match[2].matched)
                value = match[2]; // Quoted
            else
                value = match[3]; // Unquoted

            args[key] = value;
        }
    }
}