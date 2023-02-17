#include "pHake.hpp"
#include "pCheat.hpp"
#include <iostream>

pHake::pHake() {  }

void pHake::Attach(LPCSTR Name, std::shared_ptr<pProcess> process)
{
	this->process = process;
	menu = std::make_unique<pOverlay>(); // initialize game UI
	menu->Create(Name);  // overlay gta window
}

void pHake::Add(std::shared_ptr<pCheatLoop> cheat)
{
	cheats_loop_.push_back(cheat);
	menu->list.AddBool(cheat->name_, *cheat->active);
}

void pHake::Add(std::shared_ptr<pCheat> cheat)
{
	cheats_.push_back(cheat);
	menu->list.AddFunction(cheat->name_, [=]() { cheat->Execute(); });
}
void pHake::Start()
{
	menu->list.AddFunction("Exit", [=]() {Stop(); });

	for (auto& i : this->cheats_loop_)
		this->threads_.push_back(pThread([&]() { i->Execute(); }, i->thread_intervals_));


	menu->Loop(); // main loop
}

void pHake::Stop()
{
	for (auto& i : this->threads_)
		i.Destroy();

	for (auto& i : this->cheats_)
		i->Restore();
		
	process->Close(); // close handle to attached process
	menu->Close(); // close UI
}