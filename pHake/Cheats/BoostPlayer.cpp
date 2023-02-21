#include "../pLib/pUi/pOverlay.hpp"
#include "../SDK/World.hpp"
#include "../Settings.hpp"

#include "BoostPlayer.hpp"

#include <array>


static const std::array<std::string, 3> player_modes = { "default", "fast", "max" };
static uint8_t curr_player_mode = 0;


BoostPlayer::BoostPlayer(std::shared_ptr<pOverlay> ui, std::shared_ptr<World> world)
{
	this->ui = ui;
	this->world = world;

	name_ = "BoostPlayer";
}

void BoostPlayer::Execute()
{
	curr_player_mode++;
	if (curr_player_mode > player_modes.size() - 1)
		curr_player_mode = 0;

	std::cout << world->localplayer.playerinfo.base();

	switch (curr_player_mode)
	{
	case 0:
		world->localplayer.playerinfo.walk_mp(1);
		world->localplayer.playerinfo.swim_mp(1);
		settings.noclip_speed = 0.05f;

		if (!settings.noclip)
			world->localplayer.ragdoll(0);
		break;
	case 1:
		world->localplayer.playerinfo.walk_mp(2);
		world->localplayer.playerinfo.swim_mp(2);
		world->localplayer.ragdoll(1);
		settings.noclip_speed = 0.2f;
		break;
	case 2:
		world->localplayer.playerinfo.walk_mp(2000);
		world->localplayer.playerinfo.swim_mp(5);
		world->localplayer.ragdoll(1);
		settings.noclip_speed = 0.5f;
		break;
	}

	ui->notification.Add("Player set to " + player_modes[curr_player_mode]);
}

void BoostPlayer::Restore()
{
	if (curr_player_mode != 0)
	{
		world->localplayer.playerinfo.walk_mp(1);
		world->localplayer.playerinfo.swim_mp(1);
		settings.noclip_speed = 0.05f;

		if (!settings.noclip)
			world->localplayer.ragdoll(0);
	}
}