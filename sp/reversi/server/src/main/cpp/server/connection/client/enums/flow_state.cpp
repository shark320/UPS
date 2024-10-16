#include "flow_state.hpp"

const std::unordered_map<std::string, flow_state> flow_state_mapper::state_map = {
        {"MENU", flow_state::MENU},
        {"LOBBY", flow_state::LOBBY},
        {"GAME", flow_state::GAME},
        {"NULL_STATE", flow_state::NULL_STATE}
};

flow_state flow_state_mapper::get_state(const std::string &str) {
    auto it = state_map.find(str);
    if (it != state_map.end()){
        return it->second;
    }
    return flow_state::NULL_STATE;
}

std::string flow_state_mapper::get_string(flow_state state) {
    switch(state){
        case flow_state::NULL_STATE: return "NULL_STATE";
        case flow_state::MENU: return "MENU";
        case flow_state::LOBBY: return "LOBBY";
        case flow_state::GAME: return "GAME";
        default: return "";
    }
}


