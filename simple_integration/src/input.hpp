#ifndef INPUT_HPP
#define INPUT_HPP

#include <irrlicht/IEventReceiver.h>

class Input : public irr::IEventReceiver
{
public:
	Input(void);
	~Input(void) = default;

	virtual bool OnEvent(const irr::SEvent& event) override;

	bool key[irr::KEY_KEY_CODES_COUNT];
};

#endif