#pragma once

#include <string>
#include <unordered_map>

enum class flow_state {
    NULL_STATE,
    MENU,
    LOBBY,
    GAME,

};

class flow_state_mapper{
    static const std::unordered_map<std::string, flow_state> state_map;
public:
    static flow_state get_state(const std::string& str);

    static std::string get_string(flow_state state);
};

