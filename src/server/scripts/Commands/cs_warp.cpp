/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2011-2013 Apocalypse <http://www.github.com/Apocalypse> 
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: warp_commandscript
%Complete: 100
Comment: All warp related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "MapManager.h"

class warp_commandscript : public CommandScript
{
public:
    warp_commandscript() : CommandScript("warp_commandscript") { }

    ChatCommand* GetCommands() const OVERRIDE
    {
		static ChatCommand warpCommandTable[] =
        {
            { "warp",       SEC_ADMINISTRATOR,  true,  &HandleWarpCommand,                      "", NULL },
            { NULL,             0,                  false, NULL,                                "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "ashen",         SEC_ADMINISTRATOR,   true, NULL,                     "", warpCommandTable },
            { NULL,            0,                   false, NULL,                                "", NULL }
        };
        return commandTable;
	}

    static bool HandleWarpCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* player = handler->GetSession()->GetPlayer();

        char* arg1 = strtok((char*)args, " ");
        char* arg2 = strtok(NULL, " ");

        if (!arg1 || !arg2)
        return false;

        char dir = arg1[0];
        float value = float(atof(arg2));
        float x = player->GetPositionX();
        float y = player->GetPositionY();
        float z = player->GetPositionZ();
        float o = player->GetOrientation();
		 uint32 mapid = player->GetMapId();
        Map const* map = sMapMgr->CreateBaseMap(mapid);
        z = std::max(map->GetHeight(x, y, MAX_HEIGHT), map->GetWaterLevel(x, y));

    switch (dir)
    {
	case 'l': // left
        {
			x = x + cos(o+(M_PI/2))*value;
           y = y + sin(o+(M_PI/2))*value;

           player->TeleportTo(mapid, x, y, z, o);
        }
        break;
    case 'r': // right
        {
			x = x + cos(o-(M_PI/2))*value;
           y = y + sin(o-(M_PI/2))*value;

           player->TeleportTo(mapid, x, y, z, o);
        }
        break;
    case 'f': // forward
        {
			x = x + cosf(o)*value;
           y = y + sinf(o)*value;

           player->TeleportTo(mapid, x, y, z, o);
        }
        break;
    case 'u': // up
		{
           player->TeleportTo(mapid, x, y, z + value, o);
		}
        break;
	 case 'd': // down
		{
           player->TeleportTo(mapid, x, y, z - value, o);
		}
        break;
    case 'o': //orientation
        {
			o = Position::NormalizeOrientation((value * M_PI_F/180.0f)+ o);

           player->TeleportTo(mapid, x, y, z, o);
        }
        break;
    }
    return true;
  };
};
void AddSC_warp_commandscript()
{
    new warp_commandscript();
}
