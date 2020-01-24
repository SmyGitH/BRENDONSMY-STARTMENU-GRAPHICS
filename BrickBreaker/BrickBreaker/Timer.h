#ifndef _TIMER_H
#define _TIMER_H
#include <SDL.h>

namespace SDLFramework {
	class Timer {
	private:
		static Timer* sInstance;
		unsigned int mStartTicks;
		unsigned int mElapsedTicks;
		float mDeltaTime;
		float mTimeScale;

	public:
		static Timer* Instance();
		static void Release();
		void Reset();
		float DeltaTime();
		void TimeScale(float);
		float TimeScale();

		void Update();

	private:
		Timer();
		~Timer();
	};
}

#endif // Timer