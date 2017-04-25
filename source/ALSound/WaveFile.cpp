#include "..\Sound\WaveFile.h"

WaveFileLoader::~WaveFileLoader()
{
}

class al_waves : public WaveFileLoader
{
public:
	al_waves();
	virtual ~al_waves() override { close(); }

	virtual void close(void) override;

	virtual void* openWaveFromFile(const char* const filename) final;

private:

};

al_waves::al_waves()
{
}

void al_waves::close(void)
{
}

void* al_waves::openWaveFromFile(const char* const filename)
{
	return nullptr;
}
