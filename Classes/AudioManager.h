#pragma once

namespace codechiev
{
    enum AudioEnum
    {
        AD_BACKGROUND,
        AD_SIZE,
    };
    static const char* gAudioNames[] = {"GamePlayKorBefore.mp3"};
    
	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();



		static void play(AudioEnum audio);	
		static bool isPlaying();
		static void stop(AudioEnum audio);
		static void pause();
		static void resume();
		static void toggle();
        static void setVolumn(float);
        static float getVolumn();
	public:
		static const char** audioNames;
		static unsigned int* audioIds;
	private:
		static bool paused_;
	};
}


