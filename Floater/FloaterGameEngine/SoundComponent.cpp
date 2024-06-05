#include "./include/internal/SoundComponent.h"
#include "./include/internal/GameEngine.h"


flt::SoundComponent::SoundComponent() : 
	_soundEngine(GameEngine::Instance()->GetSoundEngine())
{

}
