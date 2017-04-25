#include "..\Sound\sSound.h"

#include "..\include\PlatformDefines.h"


SoundSource::~SoundSource()
{
}

class al_source : public SoundSource
{
public:
	al_source();
	virtual ~al_source() override {}

private:

};

al_source::al_source()
{
}
