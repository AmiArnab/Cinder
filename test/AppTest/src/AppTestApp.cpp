#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Log.h"
#include "cinder/CinderAssert.h"
#include "cinder/ImageIo.h"
#include "cinder/Text.h"

#include "Resources.h"

#include <boost/format.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;

void prepareSettings( App::Settings *settings )
{
	CI_LOG_I( "bang" );

	const auto &args = settings->getCommandLineArgs();
	if( ! args.empty() ) {
		CI_LOG_I( "command line args: " );
		for( size_t i = 0; i < args.size(); i++ )
			console() << "\t[" << i << "] " << args[i] << endl;
	}

	settings->setWindowPos( 50, 50 );
	settings->setWindowSize( 900, 500 );
	settings->enableHighDensityDisplay();
//	settings->setResizable( false );
//	settings->setBorderless();
//	settings->setAlwaysOnTop();
//	settings->enableMultiTouch();
//	settings->enableMultiTouch( false );
//	settings->disableFrameRate();
//	settings->setFrameRate( 20 );

//	settings->enableStatusBar( false ); // FIXME: status bar is always visible?
//	settings->setTitle( "Blarg" ); // FIXME: seems broken on mac, but did it ever work?
//	settings->setShouldQuit(); // FIXME: currently broken, since member objects continue to try to init

#if defined( CINDER_MSW )
	settings->enableConsoleWindow();
#endif
}

struct SomeMemberObj {
	gl::TextureRef mTex;

	SomeMemberObj()
	{
		CI_LOG_I( "bang" );

		auto resource = loadResource( RES_IMAGE );
		mTex = gl::Texture::create( loadImage( resource ) );

		CI_CHECK_GL();
	}
};

class AppTestApp : public App {
  public:
	AppTestApp();
	~AppTestApp();

	void setup() override;
	void keyDown( KeyEvent event ) override;
	void mouseDown( MouseEvent event ) override;
	void touchesBegan( TouchEvent event ) override;
	void touchesMoved( TouchEvent event ) override;
	void touchesEnded( TouchEvent event ) override;
	void resize() override;
	void update() override;
	void draw() override;

	void drawInfo();

	SomeMemberObj mSomeMemberObj;

	gl::TextureRef mTexStartup, mTexAsset, mTexResource;

};

AppTestApp::AppTestApp()
{
	CI_LOG_I( "bang" );

//	Surface8u surface( 140, 140, false );
//	Surface8u::Iter iter = surface.getIter();
//	while( iter.line() ) {
//		while( iter.pixel() ) {
//			iter.r() = 0;
//			iter.g() = iter.x();
//			iter.b() = iter.y();
//		}
//	}

//	mTexStartup = gl::Texture::create( surface );

	auto asset = loadAsset( "mustache-green.png" );
	mTexStartup = gl::Texture::create( loadImage( asset ) );

	CI_CHECK_GL();
}

AppTestApp::~AppTestApp()
{
	CI_LOG_I( "bang" );
}

void AppTestApp::setup()
{
	CI_LOG_I( "bang" );

	auto asset = loadAsset( "mustache-green.png" );
	mTexAsset = gl::Texture::create( loadImage( asset ) );

	auto resource = loadResource( RES_IMAGE );
	mTexResource = gl::Texture::create( loadImage( resource ) );

	gl::enableAlphaBlending();

	CI_LOG_I( "target fps: " << getFrameRate() );

	CI_CHECK_GL();
	CI_LOG_I( "complete" );
}

void AppTestApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' ) {
		setFullScreen( ! isFullScreen() );
	}
	else if( event.getChar() == 'o' ) {
		auto filePath = getOpenFilePath();
		CI_LOG_I( "result of getOpenFilePath(): " << filePath );
	}
	else if( event.getChar() == 'p' ) {
		auto filePath = getFolderPath();
		CI_LOG_I( "result of getFolderPath(): " << filePath );
	}
	else if( event.getChar() == 's' ) {
		auto filePath = getSaveFilePath();
		CI_LOG_I( "result of getSaveFilePath(): " << filePath );
	}
}

void AppTestApp::mouseDown( MouseEvent event )
{
	CI_LOG_I( "mouse pos: " << event.getPos() );
}

void AppTestApp::touchesBegan( TouchEvent event )
{
	const auto &touches = event.getTouches();
	CI_LOG_I( "num touches:" << touches.size() );

	for( size_t i = 0; i < touches.size(); i++ )
		console() << "\t\t[" << i << "] pos: " << touches.at( i ).getPos() << endl;
}

void AppTestApp::touchesMoved( TouchEvent event )
{
	const auto &touches = event.getTouches();
	CI_LOG_I( "num touches:" << touches.size() );

	for( size_t i = 0; i < touches.size(); i++ )
		console() << "\t\t[" << i << "] pos: " << touches.at( i ).getPos() << endl;
}

void AppTestApp::touchesEnded( TouchEvent event )
{
	const auto &touches = event.getTouches();
	CI_LOG_I( "num touches: " << touches.size() );

	for( size_t i = 0; i < touches.size(); i++ )
		console() << "\t\t[" << i << "] pos: " << touches.at( i ).getPos() << endl;
}

void AppTestApp::resize()
{
	CI_LOG_I( "window pos: " << getWindowPos() << ", size: " << getWindowSize() << ", pixel size: " << toPixels( getWindowSize() ) );
}

void AppTestApp::update()
{
}

void AppTestApp::draw()
{
	gl::clear( Color( 0.2f, 0.1f, 0 ) );

	// draw something animated
	{
		gl::ScopedColor colorScope( ColorA( 0, 0, 1, 0.5f ) );
		gl::ScopedModelMatrix modelScope;
		gl::translate( getWindowCenter() );
		gl::rotate( getElapsedSeconds() * 0.5, vec3( 0, 0, 1 ) );

		const float t = 200;
		gl::drawSolidRect( Rectf( -t, -t, t, t ) );
	}

	// draw test textures
	
	auto offset = vec2( 0 );

	if( mTexAsset ) {
		gl::draw( mTexAsset, offset );
	}

	if( mTexResource ) {
		offset.x += 256;
		gl::draw( mTexResource, offset );
	}

	if( mTexStartup ) {
		offset.x = 0;
		offset.y += 200;
		gl::draw( mTexStartup, offset );
	}

	if( mSomeMemberObj.mTex ) {
		offset.x += mSomeMemberObj.mTex->getWidth();
		gl::draw( mSomeMemberObj.mTex, offset );
	}

	drawInfo();

	CI_CHECK_GL();
}

void AppTestApp::drawInfo()
{
	TextLayout layout;
	layout.setFont( Font( "Arial", 16 ) );
	layout.setColor( ColorA::gray( 0.9f, 0.75f ) );

	auto fps = boost::format( "%0.2f" ) % getAverageFps();
	layout.addLine( "fps: " + fps.str() );
	layout.addLine( "v-sync enabled: " + string( gl::isVerticalSyncEnabled() ? "true" : "false" ) );

	auto tex = gl::Texture::create( layout.render( true ) );

	vec2 offset( 6, getWindowHeight() - tex->getHeight() - 6 ); // bottom left

	gl::color( Color::white() );
	gl::draw( tex, offset );
}

// no settings fn:
//CINDER_APP( AppTestApp, RendererGl )

// settings fn from top of file:
CINDER_APP( AppTestApp, RendererGl, prepareSettings )

// settings fn by lambda
//CINDER_APP( AppTestApp, RendererGl, []( AppTestApp::Settings *settings ) {
//	CI_LOG_I( "bang" );
//} )