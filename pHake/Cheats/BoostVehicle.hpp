#ifndef _BOOSTVEHICLE_HPP_
#define _BOOSTVEHICLE_HPP_

#include "../Cheat.hpp"

class BoostVehicle : public Cheat
{
public:
	BoostVehicle(std::shared_ptr<pHake> phake);

	void Execute();
	void Restore();
};
#endif