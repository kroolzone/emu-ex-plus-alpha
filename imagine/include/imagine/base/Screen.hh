#pragma once

/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#include <imagine/config/defs.hh>
#include <imagine/base/baseDefs.hh>
#include <imagine/time/Time.hh>
#include <imagine/util/rectangle2.h>
#include <imagine/util/DelegateFuncSet.hh>
#include <imagine/util/typeTraits.hh>

#if defined CONFIG_BASE_X11
#include <imagine/base/x11/XScreen.hh>
#elif defined CONFIG_BASE_ANDROID
#include <imagine/base/android/AndroidScreen.hh>
#elif defined CONFIG_BASE_IOS
#include <imagine/base/iphone/IOSScreen.hh>
#elif defined CONFIG_BASE_MACOSX
#include <imagine/base/osx/CocoaScreen.hh>
#endif

#include <vector>
#include <memory>

namespace Base
{
using namespace IG;

class Screen : public ScreenImpl
{
public:
	using Change = ScreenChange;
	using ChangeDelegate = DelegateFunc<void (Screen &screen, ScreenChange change)>;

  static constexpr double DISPLAY_RATE_DEFAULT = 0;

	using ScreenImpl::ScreenImpl;
	Screen(const Screen &) = delete;
	Screen &operator=(const Screen &) = delete;
	static uint32_t screens();
	static Screen *screen(uint32_t idx);
	// Called when a screen addition/removal/change occurs
	static void setOnChange(ChangeDelegate del);
	int width();
	int height();
	bool isPosted();
	static bool screensArePosted();
	bool addOnFrame(OnFrameDelegate del, int priority = 0);
	bool removeOnFrame(OnFrameDelegate del);
	bool containsOnFrame(OnFrameDelegate del);
	uint32_t onFrameDelegates();
	bool runningOnFrameDelegates();
	FrameParams makeFrameParams(FrameTime timestamp) const;
	bool frameRateIsReliable() const;
	double frameRate() const;
	FloatSeconds frameTime() const;
	void setFrameRate(double rate);
	std::vector<double> supportedFrameRates();
	void setFrameInterval(int interval);
	static bool supportsFrameInterval();
	static bool supportsTimestamps();

	// for internal use
	static ChangeDelegate onChange;

	static Screen &addScreen(std::unique_ptr<Screen> s);
	void frameUpdate(FrameTime timestamp);
	void startDebugFrameStats(FrameTime timestamp);
	void endDebugFrameStats();
	void setActive(bool active);
	static void setActiveAll(bool active);

private:
	bool framePosted = false;
	bool inFrameHandler = false;
	bool isActive = true;
	// for debug frame stats
	IG_enableMemberIf(Config::DEBUG_BUILD, uint32_t, continuousFrames){};
	DelegateFuncSet<OnFrameDelegate> onFrameDelegate{};

	void runOnFrameDelegates(FrameTime timestamp);
	void postFrame();
	void unpostFrame();
};

}
