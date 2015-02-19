/*
 Copyright (c) 2014, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "cinder/app/AppBasic.h"

#if defined( __OBJC__ )
	@class AppImplCocoaBasic;
#else
	class AppImplCocoaBasic;
#endif

namespace cinder { namespace app {

class AppBasicMac : public AppBasic {
  public:
	typedef std::function<void ( Settings *settings )>	SettingsFn;

	AppBasicMac();
	virtual ~AppBasicMac();

	WindowRef	createWindow( const Window::Format &format = Window::Format() ) override;
	void		quit() override;

	float		getFrameRate() const override;
	void		setFrameRate( float frameRate ) override;
	void		disableFrameRate() override;
	bool		isFrameRateEnabled() const override;
	fs::path	getAppPath() const override;

	WindowRef	getWindow() const override;
	WindowRef	getWindowIndex( size_t index ) const override;
	size_t		getNumWindows() const override;

	WindowRef	getForegroundWindow() const override;

	void		hideCursor() override;
	void		showCursor() override;
	ivec2		getMousePos() const override;

	//! \cond
	// Called during application instanciation via CINDER_APP_BASIC_MAC macro
	template<typename AppT>
	static void main( const RendererRef &defaultRenderer, const char *title, int argc, char * const argv[], const SettingsFn &settingsFn = SettingsFn() );
	//! \endcond

  protected:
	void	launch( const char *title, int argc, char * const argv[] ) override;

  private:
	AppImplCocoaBasic*	mImpl;
};

template<typename AppT>
void AppBasicMac::main( const RendererRef &defaultRenderer, const char *title, int argc, char * const argv[], const SettingsFn &settingsFn )
{
	AppBase::prepareLaunch();

	Settings settings;
	AppBase::initialize( &settings, defaultRenderer, title, argc, argv );

	if( settingsFn )
		settingsFn( &settings );

	if( settings.getShouldQuit() )
		return;

	AppBasic *app = new AppT;
	#pragma unused( app )

	AppBase::executeLaunch( title, argc, argv );
	AppBase::cleanupLaunch();
}

#define CINDER_APP_BASIC_MAC( APP, RENDERER, ... )										\
int main( int argc, char * const argv[] )												\
{																						\
	cinder::app::RendererRef renderer( new RENDERER );									\
	cinder::app::AppBasicMac::main<APP>( renderer, #APP, argc, argv, ##__VA_ARGS__ );	\
	return 0;																			\
}

} } // namespace cinder::app