#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

int main(){

    json j;
    j["pi"] = 3.141;

    int s = j["pi"];

    std::cout << s << std::endl;
    return 0;
}