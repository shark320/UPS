#include "message_manager.hpp"
#include "../connection/consts/consts.hpp"
#include "fmt/format.h"

std::shared_ptr<message> message_manager::process(const std::shared_ptr<message> &request,
                                                  const std::shared_ptr<client_connection> &client_connection) {
    if (!check_identifier(request)) {
        return nullptr;
    }
    client_connection->update_ping_timestamp();
    //Check preconditions: if handshake is performed
    if (_config->is_handshake_required() && !client_connection->is_handshake() &&
        !is_handshake_request(request->get_header())) {
        const std::string msg = "The client handshake was not performed.";
        client_connection->get_logger()->error(msg);
        return bad_request(request, client_connection->get_client(), msg);
    }
    //Check preconditions: if login is performed
    if (!client_connection->is_logged_in() &&
        !(is_login_request(request->get_header()) ||
          is_ping_request(request->get_header()) || is_handshake_request(request->get_header()))) {
        const std::string msg = "The client login was not performed.";
        client_connection->get_logger()->error(msg);
        return bad_request(request, client_connection->get_client(), msg);
    }
    switch (request->get_header()->get_type()) {
        case type::GET:
            return process_get(request, client_connection);
        case type::POST:
            return process_post(request, client_connection);
        default:
            return bad_request(request, client_connection->get_client());
    }
}

std::shared_ptr<message>
message_manager::process_post(const std::shared_ptr<message> &request,
                              const std::shared_ptr<client_connection> &client_connection) {
    switch (request->get_header()->get_subtype()) {
        case subtype::HANDSHAKE:
            return process_handshake(request, client_connection);
        case subtype::LOGIN:
            return process_login(request, client_connection);
        case subtype::LOGOUT:
            return process_logout(request, client_connection);
        case subtype::CREATE_GAME:
            return process_create_new_game(request, client_connection);
        case subtype::LOBBY_EXIT:
            return process_lobby_exit(request, client_connection);
        case subtype::LOBBY_CONNECT:
            return process_connect_to_the_lobby(request, client_connection);
        case subtype::START_GAME:
            return process_start_the_game(request, client_connection);
        case subtype::GAME_MOVE:
            return process_game_move(request, client_connection);
        default:
            return bad_request(request, client_connection->get_client());
    }
}

std::shared_ptr<message> message_manager::process_get(const std::shared_ptr<message> &request,
                                                      const std::shared_ptr<client_connection> &client_connection) {
    const auto request_subtype = request->get_header()->get_subtype();
    switch (request_subtype) {
        case subtype::PING:
            return process_ping(request, client_connection);
        case subtype::LOBBIES_LIST:
            return process_get_lobbies_list(request, client_connection);
        case subtype::LOBBY_STATE:
            return process_get_lobby_state(request, client_connection);
        case subtype::GAME_STATE:
            return process_get_game_state(request, client_connection);
        default:
            return bad_request(request, client_connection->get_client());
    }
}

std::shared_ptr<message> message_manager::process_handshake(const std::shared_ptr<message> &request,
                                                            const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    client_logger->debug("Processing client handshake.");
    if (client_connection->is_handshake()) {
        client_logger->error("The client handshake is already done.");
        return bad_request(request, client_connection->get_client(), "The client handshake is already done.");
    }
    const auto _response = std::make_shared<message>();
    const auto _header = std::make_shared<header>(request->get_header());
    _header->set_length(0);
    _header->set_status(status::OK);
    _response->set_header(_header);

    client_connection->set_handshake(true);
    client_connection->update_ping_timestamp();

    return _response;
}

bool message_manager::check_identifier(const std::shared_ptr<message> &request) const {
    std::string identifier = request->get_header()->get_identifier();

    return identifier == this->_config->get_identifier();
}

void message_manager::set_client_manager(const std::shared_ptr<client_manager> &client_manager) {
    this->_client_manager = client_manager;
}

void message_manager::set_lobby_manager(const std::shared_ptr<lobby_manager> &lobby_manager) {
    this->_lobby_manager = lobby_manager;
}

message_manager::message_manager(const std::shared_ptr<connection_config> &connection_config , const std::shared_ptr<game_config>& _game_config) : _config(
        connection_config), _game_config(_game_config) {

}

std::shared_ptr<message> message_manager::process_login(const std::shared_ptr<message> &request,
                                                        const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    client_logger->debug("Processing login.");
    if (const auto client = client_connection->get_client()) {
        client_logger->error("The client is already logged in.");
        return bad_request(request, client_connection->get_client(), "The client is already logged in.");
    }
    const auto _response = std::make_shared<message>();
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto request_payload = request->get_payload();
    const auto response_payload = std::make_shared<payload>();
    const auto login = request_payload->get_string("username");

    if (login == nullptr || login->empty()) {
        return bad_request(request, client_connection->get_client());
    }

    auto _client = this->_client_manager->get_client_by_login(*login);

    if (_client != nullptr) {
        if (_client->is_connected()) {
            client_logger->debug(fmt::format("Client with the username '{}' is already connected!", *login));
            response_header->set_status(status::CONFLICT);
            response_payload->set_value("msg", std::make_shared<string>(
                    "Provided username is already in use. Choose another one, please."));

            return std::make_shared<message>(response_header, response_payload);
        } else {
            client_logger->debug(fmt::format("Found client with the username '{}' in cache.", *login));
            client_connection->set_client(_client);
            _client->connect(client_connection);
        }

    } else {
        client_logger->debug(fmt::format("New client with the name '{}' is logged in", *login));
        _client = this->_client_manager->login_client(*login, client_connection);
        client_connection->set_client(_client);
    }

    response_header->set_status(status::OK);
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(_client->get_flow_state())));

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message>
message_manager::bad_request(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client) {
    return bad_request(request, client, "Bad request");
}

std::shared_ptr<message>
message_manager::bad_request(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client,
                             const std::string &msg) {
    const auto _header = std::make_shared<header>(request->get_header());
    const auto _payload = std::make_shared<payload>();
    _header->set_status(status::BAD_REQUEST);
    _payload->set_value("msg", std::make_shared<string>(msg));
    if (client != nullptr) {
        _payload->set_value("state", std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    }
    return std::make_shared<message>(_header, _payload);
}

std::shared_ptr<message> message_manager::process_create_new_game(const std::shared_ptr<message> &request,
                                                                  const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();
    const auto request_payload = request->get_payload();
    const auto name_str_ptr = request_payload->get_string("name");


    if (name_str_ptr == nullptr || name_str_ptr->empty()) {
        auto msg = "Cannot create a game: Invalid game name!";
        client_logger->error(msg);
        return bad_request(request, client_connection->get_client(), msg);
    }
    client_logger->debug(fmt::format("Performing game creation with name '{}'.", *name_str_ptr));

    if (client->get_flow_state() != flow_state::MENU || client->get_lobby() != nullptr) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    auto new_lobby = this->_lobby_manager->create_lobby(*name_str_ptr, client);
    if (new_lobby == nullptr) {
        //Name is already taken
        response_header->set_status(status::CONFLICT);
        response_payload->set_value("msg", std::make_shared<string>(
                "Provided game name is already in use. Choose another one, please."));
        client_logger->debug(fmt::format("Cannot create a game: the name '{}' is already taken.", *name_str_ptr));
    } else {
        response_header->set_status(status::OK);
        client->update_flow_state(flow_state::LOBBY);
        response_payload->set_value("state",
                                    std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
        response_payload->set_value("game", std::make_shared<string>(*name_str_ptr));
        response_payload->set_value("user", std::make_shared<string>(client->get_username()));
        client_logger->debug(fmt::format("Game with the name '{}' is created!", *name_str_ptr));
    }
    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_lobby_exit(const std::shared_ptr<message> &request,
                                                             const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }

    auto lobby = client->get_lobby();

    if (lobby == nullptr || !client->is_in_state({flow_state::LOBBY, flow_state::GAME})) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    this->_lobby_manager->exit_lobby(client);
    client->update_flow_state(flow_state::MENU);
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    response_header->set_status(status::OK);
    //TODO: check lobby ptr destruction on second player exit.
    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_ping(const std::shared_ptr<message> &request,
                                                       const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    if (client == nullptr) {
        response_header->set_status(status::OK);
    } else {
        response_payload->set_value("state",
                                    std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
        response_payload->set_value("username", std::make_shared<string>(client->get_username()));
        auto lobby = client->get_lobby();
        response_payload->set_value("lobby", lobby == nullptr ? nullptr : std::make_shared<string>(lobby->get_name()));
    }


    return std::make_shared<message>(response_header, response_payload);
}

bool message_manager::is_handshake_request(const std::shared_ptr<header> &header) {
    return header->get_type() == type::POST && header->get_subtype() == subtype::HANDSHAKE;
}

bool message_manager::is_login_request(const std::shared_ptr<header> &header) {
    return header->get_type() == type::POST && header->get_subtype() == subtype::LOGIN;
}

bool message_manager::is_ping_request(const std::shared_ptr<header> &header) {
    return header->get_type() == type::GET && header->get_subtype() == subtype::PING;
}

std::shared_ptr<message> message_manager::process_get_lobbies_list(const std::shared_ptr<message> &request,
                                                                   const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }

    client_logger->debug("Processing get lobbies request.");

    if (client->get_flow_state() != flow_state::MENU) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    const auto hosts_map = this->_lobby_manager->get_available_lobby_names_and_hosts();
    const auto lobby_names = std::make_shared<objects_vector>();
    const auto host_usernames = std::make_shared<objects_vector>();

    for (const auto &host_pair: *hosts_map) {
        lobby_names->push_back(std::make_shared<string>(host_pair.first));
        host_usernames->push_back(std::make_shared<string>(host_pair.second));
    }

    response_header->set_status(status::OK);
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    response_payload->set_value("lobbies", lobby_names);
    response_payload->set_value("lobby_hosts", host_usernames);

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_connect_to_the_lobby(const std::shared_ptr<message> &request,
                                                                       const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }

    if (!client->is_in_state({flow_state::MENU})) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    const auto lobby_name_ptr = request->get_payload()->get_string("lobby");
    if (lobby_name_ptr == nullptr || lobby_name_ptr->empty()) {
        std::string msg = "Can not connect to a lobby: lobby name to connect is empty.";
        client_logger->error(msg);
        return bad_request(request, client, msg);
    }

    auto lobby = this->_lobby_manager->get_lobby(*lobby_name_ptr);
    if (lobby == nullptr || !lobby::connect_player(client, lobby)) {
        std::string msg = fmt::format("Can not connect to a lobby: lobby with the name '{}' is not available.",
                                      *lobby_name_ptr);
        client_logger->error(msg);
        response_payload->set_value("msg", std::make_shared<string>(msg));
        response_payload->set_value("state",
                                    std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
        response_payload->set_value("lobby", std::make_shared<string>(*lobby_name_ptr));
        return std::make_shared<message>(response_header, response_payload);
    }

    client->update_flow_state(flow_state::LOBBY);
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    add_lobby_info(lobby, response_payload);


    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_get_lobby_state(const std::shared_ptr<message> &request,
                                                                  const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();
    const auto lobby = client->get_lobby();

    if (lobby == nullptr || !client->is_in_state({flow_state::LOBBY, flow_state::GAME})) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    if (client->is_in_state({flow_state::GAME}) && lobby->is_started()){
        //Game is started
        response_header->set_status(status::MOVED_PERMANENTLY);
    } else{
        response_header->set_status(status::OK);
    }

    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    add_lobby_info(lobby, response_payload);

    return std::make_shared<message>(response_header, response_payload);
}


void
message_manager::add_lobby_info(const std::shared_ptr<lobby> &lobby, const std::shared_ptr<payload> &response_payload) {

    const auto lobby_players_payload = get_lobby_players(lobby);

    response_payload->set_value("host", lobby_players_payload->at(0));
    response_payload->set_value("lobby", std::make_shared<string>(lobby->get_name()));
    response_payload->set_value("players", lobby_players_payload);
}

std::shared_ptr<message> message_manager::process_start_the_game(const std::shared_ptr<message> &request,
                                                                 const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();
    const auto lobby = client->get_lobby();

    if (lobby == nullptr || !client->is_in_state({flow_state::LOBBY})) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    if (client != lobby->get_host()) {
        std::string msg = fmt::format("The client '{}' is not a host of the lobby.", client->get_username());
        client_logger->error(msg);
        return unauthorized(request, client, msg);
    }

    if (!lobby->start_game(this->_game_config)) {
        std::string msg = "Not enough players in the lobby.";
        client_logger->error(msg);
        return not_allowed(request, client, msg);
    }
    response_header->set_status(status::OK);
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    add_lobby_info(lobby, response_payload);


    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_get_game_state_game_over(
        const std::shared_ptr<message> &request,
        const std::shared_ptr<client> &client,
        const std::shared_ptr<objects_vector> &lobby_players_payload,
        const std::shared_ptr<player> &winner
) {
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();
    response_header->set_status(status::RESET);

    this->_lobby_manager->exit_lobby(client);

    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    response_payload->set_value("players", lobby_players_payload);
    response_payload->set_value("winner", std::make_shared<string>(winner->get_client()->get_username()));

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_get_game_state_terminated(
        const std::shared_ptr<message> &request,
        const std::shared_ptr<client> &client
) {

    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();
    response_header->set_status(status::MOVED_PERMANENTLY);

    this->_lobby_manager->exit_lobby(client);

    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    response_payload->set_value("msg",
                                std::make_shared<string>("The game session is terminated."));

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_get_game_state(const std::shared_ptr<message> &request,
                                                                 const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();
    const auto lobby = client->get_lobby();

    if (lobby == nullptr || !client->is_in_state({flow_state::GAME})) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    const auto game = lobby->get_game();

    if (game == nullptr) {
        return not_found(request, client, "The game for the client is not found.");
    }

    const auto game_winner = game->get_winner();

    if (lobby->is_terminated() && game_winner == nullptr) {
        client_logger->warn(fmt::format("The game in the lobby {} is terminated", lobby->get_name()));
        return process_get_game_state_terminated(
                request,
                client
        );
    }


    const auto opponent_client = game->get_opponent_client(client);

    if (opponent_client == nullptr) {
        return process_get_game_state_terminated(
                request,
                client
        );
    }

    const auto current_player_client = game->get_current_player_client();

    if (current_player_client == nullptr) {
        return not_found(request, client, "The current player for the game is not found.");
    }




    const auto lobby_players_payload = get_lobby_players(lobby);

    if (game_winner != nullptr) {
        //Game is over
        return process_get_game_state_game_over(
                request,
                client,
                lobby_players_payload,
                game_winner
        );
    }

    const auto lobby_player_codes_payload = get_lobby_player_codes(lobby);

    const auto last_move = game->get_last_move();

    const auto board_cells = convert_board_representation(game->get_board_representation());

    const auto board_size = std::make_shared<objects_vector>();
    board_size->push_back(std::make_shared<integer>(game->get_board()->get_cols()));
    board_size->push_back(std::make_shared<integer>(game->get_board()->get_rows()));

    response_header->set_status(status::OK);
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    response_payload->set_value("players", lobby_players_payload);
    response_payload->set_value("player_codes", lobby_player_codes_payload);
    response_payload->set_value("is_opponent_connected", std::make_shared<boolean>(opponent_client->is_connected()));
    response_payload->set_value("current_player", std::make_shared<string>(current_player_client->get_username()));
    response_payload->set_value("board", board_cells);
    response_payload->set_value("board_size", board_size);
    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_game_move(const std::shared_ptr<message> &request,
                                                            const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto client = client_connection->get_client();
    if (client == nullptr){
        return bad_request(request, client, "Client was not found!");
    }
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();
    const auto lobby = client->get_lobby();

    if (lobby == nullptr || !client->is_in_state({flow_state::GAME})) {
        return invalid_state(client->get_flow_state(), request, client, client_logger);
    }

    const auto game = lobby->get_game();

    if (game == nullptr) {
        return not_found(request, client, "The game for the client is not found.");
    }

    const auto move_x = request->get_payload()->get_integer("x");
    const auto move_y = request->get_payload()->get_integer("y");

    if (move_x == nullptr || move_y == nullptr){
        return bad_request(request, client, "Move coordinates are null!");
    }

    const auto move_result = game->process_move(move_x->value(), move_y->value(), client);

    switch(move_result){
        case move_result::INVALID_COORDINATES:
            return process_game_move_invalid_move(request, client);
        case SUCCESS:
            return process_game_move_success(request, game->get_current_player_client(), game);
        case INVALID_PLAYER:
            return process_game_move_invalid_player(request, client);
        case NO_PLAYER:
            return not_found(request, client, "The player is not found.");
        case GAME_OVER:
            return not_found(request, client, "The game is not found or is over.");
    }

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_game_move_invalid_move(const std::shared_ptr<message> &request, const std::shared_ptr<client>& client) {
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    response_header->set_status(status::NOT_ALLOWED);

    response_payload->set_value("msg", std::make_shared<string>("Invalid move coordinates."));
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_game_move_success(const std::shared_ptr<message> &request,
                                                                    const std::shared_ptr<client> &current_player, const std::shared_ptr<reversi_game>& game) {
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    const auto board_cells = convert_board_representation(game->get_board_representation());

    response_header->set_status(status::OK);
    response_payload->set_value("x", std::make_shared<integer>(game->get_last_move()->x));
    response_payload->set_value("y", std::make_shared<integer>(game->get_last_move()->y));
    response_payload->set_value("current_player", std::make_shared<string>(current_player->get_username()));
    response_payload->set_value("board", board_cells);

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::process_game_move_invalid_player(const std::shared_ptr<message> &request,
                                                                           const std::shared_ptr<client> &client) {
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    response_header->set_status(status::CONFLICT);

    response_payload->set_value("msg", std::make_shared<string>("Not the player move"));
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message>
message_manager::process_logout(const std::shared_ptr<message> &request,
                                const std::shared_ptr<client_connection> &client_connection) {
    const auto client_logger = client_connection->get_logger();
    const auto response_header = std::make_shared<header>(request->get_header());
    const auto response_payload = std::make_shared<payload>();

    client_logger->debug("Processing client logout.");
    this->_client_manager->logout_client(client_connection);

    response_header->set_status(status::OK);
    return std::make_shared<message>(response_header, response_payload);
}



std::shared_ptr<message>
message_manager::unauthorized(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client,
                              const std::string &msg) {
    const auto _header = std::make_shared<header>(request->get_header());
    const auto _payload = std::make_shared<payload>();
    _header->set_status(status::UNAUTHORIZED);
    _payload->set_value("msg", std::make_shared<string>(msg));
    if (client != nullptr) {
        _payload->set_value("state", std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    }

    return std::make_shared<message>(_header, _payload);
}

std::shared_ptr<message>
message_manager::unauthorized(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client) {
    return unauthorized(request, client, "Unauthorized");
}

std::shared_ptr<message>
message_manager::not_allowed(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client,
                             const std::string &msg) {
    const auto _header = std::make_shared<header>(request->get_header());
    const auto _payload = std::make_shared<payload>();
    _header->set_status(status::NOT_ALLOWED);
    _payload->set_value("msg", std::make_shared<string>(msg));
    if (client != nullptr) {
        _payload->set_value("state", std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    }

    return std::make_shared<message>(_header, _payload);
}

std::shared_ptr<message>
message_manager::not_allowed(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client) {
    return not_allowed(request, client, "Not allowed");
}


std::shared_ptr<message> message_manager::invalid_state(flow_state state, const std::shared_ptr<message> &request,
                                                        const std::shared_ptr<client> &client,
                                                        const std::shared_ptr<log4cxx::Logger> &client_logger) {
    std::string msg = fmt::format("Client is in invalid state ({}).", flow_state_mapper::get_string(state));
    client_logger->error(msg);
    return bad_request(request, client, msg);
}

std::shared_ptr<objects_vector> message_manager::get_lobby_players(const std::shared_ptr<lobby> &lobby) {
    const auto lobby_players = lobby->get_players();
    auto lobby_players_payload = std::make_shared<objects_vector>();
    for (const auto &player: *lobby_players) {
        if (player != nullptr) {
            lobby_players_payload->push_back(std::make_shared<string>(player->get_username()));
        }
    }

    return lobby_players_payload;
}

std::shared_ptr<message>
message_manager::not_found(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client,
                           const std::string &msg) {
    const auto _header = std::make_shared<header>(request->get_header());
    const auto _payload = std::make_shared<payload>();
    _header->set_status(status::NOT_FOUND);
    _payload->set_value("msg", std::make_shared<string>(msg));
    if (client != nullptr) {
        _payload->set_value("state", std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    }

    return std::make_shared<message>(_header, _payload);
}

std::shared_ptr<message>
message_manager::not_found(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client) {
    return not_found(request, client, "Not Found");
}

std::shared_ptr<objects_vector> message_manager::get_lobby_player_codes(const std::shared_ptr<lobby> &lobby) {
    auto lobby_player_codes_payload = std::make_shared<objects_vector>();
    const auto game = lobby->get_game();
    if (game == nullptr){
        return lobby_player_codes_payload;
    }
    const auto lobby_players = lobby->get_players();

    for (const auto &player: *lobby_players) {
        if (player != nullptr) {
            lobby_player_codes_payload->push_back(std::make_shared<string>(player_code_mapper::get_string(game->get_client_player(player)->get_player_code())));
        }
    }

    return lobby_player_codes_payload;
}

std::shared_ptr<message>
message_manager::moved_permanently(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client,
                                   const std::string &msg) {
    const auto _header = std::make_shared<header>(request->get_header());
    const auto _payload = std::make_shared<payload>();
    _header->set_status(status::MOVED_PERMANENTLY);
    _payload->set_value("msg", std::make_shared<string>(msg));
    if (client != nullptr) {
        _payload->set_value("state", std::make_shared<string>(flow_state_mapper::get_string(client->get_flow_state())));
    }

    return std::make_shared<message>(_header, _payload);
}

std::shared_ptr<message>
message_manager::moved_permanently(const std::shared_ptr<message> &request, const std::shared_ptr<client> &client) {
    return moved_permanently(request, client, "Moved Permanently");
}

std::shared_ptr<objects_vector> message_manager::convert_board_representation(const std::shared_ptr<std::vector<int>>& board_cells) {
    const auto converted = std::make_shared<objects_vector>();
    for (const auto cell_value: *board_cells){
        converted->push_back(std::make_shared<integer>(cell_value));
    }
    return converted;
}










