//
// Created by -server- on 11/6/2018.
//

#ifndef GROWTOPIASERVER_LOGIN_PARSER_H
#define GROWTOPIASERVER_LOGIN_PARSER_H

#include <ostream>
#include <sstream>

struct login_data {
    std::string requested_name;
    std::string tank_name;
    std::string tank_password;
    int16_t protocol;
    float game_version;
    std::string country;
    std::string mac;
};

login_data* parse_login(std::string data);

#endif //GROWTOPIASERVER_LOGIN_PARSER_H
