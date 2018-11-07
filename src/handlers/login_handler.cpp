#include "login_handler.h"

using namespace std;
using namespace growtopia;

void handlers::handle_login(objects::player* player, string data) {
    gamepacket* p;
    helpers::login_data ldata = helpers::parse_login(std::move(data));

    p = gamepacket::new_packet("OnSuperMainStartAcceptLogonHrdxs47254722215a")
            ->write(-703607114, false)
                    // Cache URL
            ->write("cdn.growtopiagame.com")
                    // Cache folder
            ->write("cache/")
                    // AntiCheat for android
            ->write("cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster")
                    // Some other settings
            ->write("proto=42|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|");

    player->send_packet(p);

    player->user_name = ldata.requested_name;
    if (!ldata.tank_name.empty())
        if (!player->is_password(ldata.tank_password)) {
            player->send_console_message("`oCould not login! please check your `#username `$or `#password`$!");
            enet_peer_disconnect_later(player->peer, 0);
            throw 0;
        }
    player->country = ldata.country;

    for (char c : player->display_name) if (c < 0x20 || c>0x7A) player->display_name = "Bad characters in name, remove them!";

    if (player->country.length() > 4)
        player->country = "us";

    p = gamepacket::new_packet("SetHasGrowID")
            ->write(!ldata.tank_name.empty())
            ->write(player->user_name)
            ->write(ldata.tank_password);

    player->send_packet(p);

    player->is_loggedin(true);
}