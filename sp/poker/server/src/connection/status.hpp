#include <iostream>
#include <map>

class Status {
public:
    enum Enum {
        NO_STATUS = 0,
        OK = 200,
        NOT_FOUND = 404,
        CONFLICT = 409,
        NOT_ALLOWED = 405
    };

private:
    static const std::map<int, Status::Enum> statusMap;

public:
    static Enum getStatus(int code) {
        auto it = statusMap.find(code);
        if (it != statusMap.end()) {
            return it->second;
        }
        return NO_STATUS;
    }

    static int getCode(Enum status) {
        for (const auto& entry : statusMap) {
            if (entry.second == status) {
                return entry.first;
            }
        }
        return -1;  // Handle cases where the status code is not found
    }
};

const std::map<int, Status::Enum> Status::statusMap = {
        {0, Status::NO_STATUS},
        {200, Status::OK},
        {404, Status::NOT_FOUND},
        {409, Status::CONFLICT},
        {405, Status::NOT_ALLOWED}
};
