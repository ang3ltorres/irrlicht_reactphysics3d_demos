#include "input.hpp"

Input::Input(void)
{
	for (unsigned int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++)
		key[i] = false;
}

bool Input::OnEvent(const irr::SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		key[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}