#include "Math/Vectors.hpp"
#include "Renderer/SceneNode.hpp"
#include "ChcSceneManager.hpp"
#include "System/Engine.hpp"
#include "Util/OpenGL.hpp"
#include "System/Window.hpp"
#include "System/Input.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/FpsCameraControl.hpp"
#include "Renderer/Mesh.hpp"
#include "AssetManager/AssetManager.hpp"
#include "AssetManager/ImageAsset.hpp"

#include "World.hpp"
#include "Math/AlignedBox.hpp"

#include <fstream>
#include <cstdlib>

using namespace cge;
using namespace cge::game;

#define log getLog() << "Game: "

class Game;

class CameraControl: public FpsCameraControl {
public:
	explicit CameraControl( Game& game );

protected:
	virtual void checkTranslation( vec3& t );

private:
	bool uncollide( vec3& t, const vec3& bp, const vec3& pos );
	
	vec3f	safeDist_;
	World&	w_;
};

class BlockPlacer: public Entity, public InputListener {
public:
	BlockPlacer( World &w ): w_(w), pos_(8.5f,98.5f,6.5f) { }

	void onAttached() {
		getNode().setRenderable( true );
		getNode().setVisibility( true );
	}

	void onRendered() {
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glTranslatef( pos_.x, pos_.y, pos_.z );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_TEXTURE_2D );
		glColor3f(0.0f,0.0f,1.0f);
		debugCube( 1.0f, true );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_TEXTURE_2D );
		glColor3f(1.0f,1.0f,1.0f);
		glPopMatrix();
	}

	void onKeyPressed( KeyEvent& ev ) {
		if( ev.getCode() == SDLK_i )
			pos_.z -= 1.0f;
		else if( ev.getCode() == SDLK_k )
			pos_.z += 1.0f;
		else if( ev.getCode() == SDLK_j )
			pos_.x -= 1.0f;
		else if( ev.getCode() == SDLK_l )
			pos_.x += 1.0f;
		else if( ev.getCode() == SDLK_SEMICOLON )
			pos_.y += 1.0f;
		else if( ev.getCode() == SDLK_SLASH )
			pos_.y -= 1.0f;
		else if( ev.getCode() == SDLK_p ) {
			Block b = w_.getBlockAt( pos_ );
			if( b.getBlockDefId() == 0x0 )
				b.setDef( 0x1 );
			else
				b.setDef( 0x0 );
		}
			
	}

private:
	World& w_;
	vec3 pos_;
};

class Game: public Engine, public InputListener, public Task {
	friend class DebugEntity;
public:
	Game() {
		getLog().addDevice( new StreamLogDevice(std::cout) );

		win_.reset		( new Window( *this ) );
		inp_.reset		( new Input( *this ) );
		scene_.reset	( new ChcSceneManager(*this) );
		asmg_.reset		( new AssetManager( *this ) );

		asmg_->addContentFactory< ImageContent >( "img" );
		auto vms = VideoMode::getAvailable();
		
		std::sort( vms.begin(), vms.end(),
			[]( VideoMode &a, VideoMode &b ) {
				return a.getWidth() < b.getWidth();
			}
		);
		VideoMode vm = vms.back();
		vm.setFullscreen( true );
		//vm.setSize( 1920, 800 );
		vm.setVSync(true);
		vm.setDecorated( true );
		vm.setBpp( 32 );
		
		win_->setVideoMode( vm );
		
		addTask( *win_ );
		addTask( *inp_ );

		win_->addTask( *scene_ );
		
		inp_->addInputListener(*this);

		setupGL();
		loadTexture();
		createCamera();
		createLevel();

		ctrl_.reset( new CameraControl( *this ) );
		ctrl_->connect();
		

		BlockPlacer *pl = new BlockPlacer( *world_ );
		
		inp_->addInputListener( *pl );
		scene_->getRoot().createChild().attachEntity( *pl );		
	}

	Input&	getInput()	{ return *inp_; }
	Camera&	getCamera() { return *cam_; }
	World&	getWorld()	{ return *world_; }

	void loadTexture() {
		ImageAsset img = asmg_->getAsset<ImageAsset>("../data/img_terrain.png");
		glGenTextures( 1, &tex_ );
		glBindTexture( GL_TEXTURE_2D, tex_ );
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, img->getWidth(), img->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img->getRawPixels() );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img->getWidth(), img->getHeight(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, img->getRawPixels() );
	}


	void setupGL() {
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable( GL_TEXTURE_2D );
		glEnable(GL_CULL_FACE);
		glCullFace( GL_BACK );
		vec4 skyColour( 0.607f, 0.77f, 1.0f, 1.0f );
		glClearColor( skyColour.w, skyColour.x, skyColour.y, skyColour.z);
		glFogi( GL_FOG_MODE, GL_LINEAR );
		glFogfv( GL_FOG_COLOR, skyColour );
		glFogf( GL_FOG_DENSITY, 0.35f );
		glHint( GL_FOG_HINT, GL_DONT_CARE );
		glFogf( GL_FOG_START, 9.50f * 16.0f );
		glFogf( GL_FOG_END, 11.0f * 16.0f );
		glEnable(GL_FOG);
	}

	void createCamera() {
		cam_.reset( new Camera() );
		
		cam_->setProjection( 45.0, (double)win_->getWidth() / win_->getHeight(), 0.05, 11.0 * 16.0 );
		scene_->getRoot().createChild().attachEntity( *cam_ );
		scene_->setCamera(*cam_);
		cam_->getNode().setPosition( vec3(8.0f,90.0f,8.0f) );
		
	}

	void createLevel() {
		world_.reset( new game::World( *cam_ ) );
		scene_->getRoot().createChild().attachEntity( *world_ );
		
	}

	void runTask() {
		
	}

	void onClose() {
		die("User exit.");
	}

	void onKeyPressed( KeyEvent &ev ) {
		if( ev.getCode() == 27 )
			die("User exit.");
	}


public:
	std::auto_ptr<SceneManager>     scene_;
	std::auto_ptr<Input>            inp_;
	std::auto_ptr<Window>           win_;
	std::auto_ptr<Camera>           cam_;
	std::auto_ptr<CameraControl>	ctrl_;
	std::auto_ptr<AssetManager>		asmg_;
	std::auto_ptr<game::World>		world_;
	GLuint							tex_;
};

CameraControl::CameraControl( Game& game )
	: FpsCameraControl( game.getInput(), game.getCamera() ), w_( game.getWorld() )  {
	safeDist_ = vec3(0.5f);
}

void CameraControl::checkTranslation( vec3& t ) {
	auto oldPos = getCamera().getNode().getPosition();
	oldPos.y -= 0.2f;
	vec3 newPos[2];
	newPos[0] = newPos[1] = oldPos + t;
	newPos[1].y -= ctBlockSize;
	vec3i v;

	for(v.x = -1; v.x < 2; ++v.x) {
		for(v.y = -1; v.y < 2; ++v.y) {
			for(v.z = -1; v.z < 2; ++v.z) {
				vec3 d( v );
				for( int i=0; i<2; ++i ) {
					vec3 bp = vec3(0.5f) + (oldPos - i*ctBlockSize*ctVec::y + d).map<float (float)>(floor);
					Block b = w_.getBlockAt( bp );
					if( b.isObstacle() ) {
						vec3 correction( ctBlockRadius );
						correction += safeDist_ - (bp - newPos[i]).map<float (float)>(fabs);
						if( correction >= ctVec::zero ) {
							t -= correction.minArg().modulate( d );
							newPos[0] = newPos[1] = oldPos + t;
							newPos[1].y -= ctBlockSize;
						}
					}
				}		
			}
		}
	}
}


int main(int argc, char *argv[]) {
	try {
		Game g;
		g.start();
	} catch( std::exception& e ) {
		std::cerr << e.what() << std::endl;
	}
	std::cin.get();
	return 0;
}
