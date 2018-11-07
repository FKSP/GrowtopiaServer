#include "login_parser.h"

using namespace std;
using namespace growtopia;

const helpers::login_data helpers::parse_login(string data) {
    string to;
    login_data ldata = login_data();
    stringstream ss(data);

    while (getline(ss, to, '\n')) {
        string key = to.substr(0, to.find('|'));
        string value = to.substr(to.find('|') + 1, to.length() - to.find('|') - 1);

        if (key == "tankIDName")
            ldata.tank_name = value;
        else if(key == "tankIDPass")
            ldata.tank_password = value;
        else if(key == "requestedName")
            ldata.requested_name = value;
        else if (key == "country")
            ldata.country = value;
        else if (key == "protocol")
            ldata.protocol = (int16_t) stoi(value);
        else if (key == "game_version")
            ldata.game_version = stof(value);
        else if (key == "mac")
            ldata.mac = value;
    }

    return ldata;
}