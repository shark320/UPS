#include <iostream>
#include <string>
#include <sstream>

class MyClass {
private:
    int age;
    std::string msg;
    bool flag;

public:
    MyClass(int age, const std::string& msg, bool flag)
            : age(age), msg(msg), flag(flag) {}

    // JSON Serialization
    std::string toJsonString() const {
        std::ostringstream json;
        json << "{";
        json << "\"age\":" << age << ",";
        json << "\"msg\":\"" << msg << "\",";
        json << "\"flag\":" << (flag ? "true" : "false");
        json << "}";
        return json.str();
    }

    // JSON Parsing
    static MyClass fromJsonString(const std::string& json) {
        int age = 0;
        std::string msg;
        bool flag = false;

        size_t pos = 0;
        while (pos < json.size()) {
            pos = json.find("\"age\":", pos);
            if (pos == std::string::npos) {
                break;
            }
            pos += 6;
            age = std::stoi(json.substr(pos, json.find_first_of(',', pos) - pos));

            pos = json.find("\"msg\":\"", pos);
            if (pos == std::string::npos) {
                break;
            }
            pos += 7;
            size_t endPos = json.find_first_of('"', pos);
            msg = json.substr(pos, endPos - pos);

            pos = json.find("\"flag\":", pos);
            if (pos == std::string::npos) {
                break;
            }
            pos += 7;
            flag = json.substr(pos, json.find_first_of(',', pos) - pos) == "true";
        }

        return MyClass(age, msg, flag);
    }

    int getAge() const {
        return age;
    }

    const std::string &getMsg() const {
        return msg;
    }

    bool getFlag() const {
        return flag;
    }
};

int main() {
    MyClass obj(25, "Hello, JSON!", true);

    // Serialize to JSON
    std::string json = obj.toJsonString();
    std::cout << "Serialized JSON: " << json << std::endl;

    // Parse JSON
    MyClass parsedObj = MyClass::fromJsonString(json);
    std::cout << "Parsed JSON: age=" << parsedObj.getAge() << ", msg=" << parsedObj.getMsg() << ", flag=" << parsedObj.getFlag() << std::endl;

    return 0;
}