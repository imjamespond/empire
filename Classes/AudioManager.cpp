#include "AudioManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace codechiev;

AudioManager::AudioManager()
{
}


AudioManager::~AudioManager()
{
}

//In this example we preload two WAV sound effects, a gun cocking and a gun shot.  
//Playing a sound effect is as simple as calling playEffect() passing in the file name.  
//Of course, be certain to copy the appropriate sound files to your project¡¯s resource folder before running this example.  
//Next this example queues up a lambda method to be called 1.5 seconds of the gun cocking sound is played to play our gun shot sound.  
//At this point we are done with our gun cocking effect so we unload it from memory using unloadEffect().  
//You can still call playEffect with that file in the future, but it will result in the file being loaded again. 

void
AudioManager::play(AudioEnum audio)
{
	assert(audio<AD_SIZE);
	auto simpleAudio = CocosDenshion::SimpleAudioEngine::getInstance();
	simpleAudio->playBackgroundMusic(gAudioNames[audio], true);
}

bool
AudioManager::isPlaying()
{
	auto simpleAudio = CocosDenshion::SimpleAudioEngine::getInstance();
	return simpleAudio->isBackgroundMusicPlaying();
}

void
AudioManager::stop(AudioEnum audio)
{
	assert(audio<AD_SIZE);
	auto simpleAudio = CocosDenshion::SimpleAudioEngine::getInstance();
	simpleAudio->stopBackgroundMusic();
}

void
AudioManager::pause()
{
	auto simpleAudio = CocosDenshion::SimpleAudioEngine::getInstance();
	simpleAudio->pauseBackgroundMusic();
}

void
AudioManager::resume()
{
	auto simpleAudio = CocosDenshion::SimpleAudioEngine::getInstance();
	simpleAudio->resumeBackgroundMusic();
}

bool
AudioManager::paused_(false);
void
AudioManager::toggle()
{
	if (paused_)
	{
		resume();
	}
	else
	{
		pause();
	}
}

void
AudioManager::setVolumn(float val)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(val);
}
float
AudioManager::getVolumn()
{
    return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}



