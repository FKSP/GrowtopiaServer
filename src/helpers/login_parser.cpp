//
// Created by -server- on 11/6/2018.
//

#include "login_parser.h"

using namespace std;

login_data* parse_login(std::string data) {
    stringstream ss(data);
    std::string to;
    auto ldata = new login_data;

    while (std::getline(ss, to, '\n')) {
        string key = to.substr(0, to.find('|'));
        string value = to.substr(to.find('|') + 1, to.length() - to.find('|') - 1);

        if (key == "tankIDName")
            ldata->tank_name = value;
        else if(key == "tankIDPass")
            ldata->tank_password = value;
        else if(key == "requestedName")
            ldata->requested_name = value;
        else if (key == "country")
            ldata->country = value;
        else if (key == "protocol")
            ldata->protocol = (int16_t) std::stoi(value);
        else if (key == "game_version")
            ldata->game_version = std::stof(value);
        else if (key == "mac")
            ldata->mac = value;
    }

    return ldata;
}