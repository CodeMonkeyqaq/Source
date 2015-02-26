#include "Base.h"
#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("COC_PC");
		glview->setFrameSize(1280, 720);
		auto windowHandle = glview->getWin32Window();
		MoveWindow(windowHandle, 0, 0, 1280, 720, false);

		RECT windowRect;
		GetWindowRect(windowHandle, &windowRect);
		::ClipCursor(&windowRect);

        director->setOpenGLView(glview);
    }

    vector<string> searchPaths;
    searchPaths.push_back("Resources");
    searchPaths.push_back("Resources/building");
    searchPaths.push_back("Resources/map");
    searchPaths.push_back("Resources/fonts");
    FileUtils::getInstance()->setSearchPaths(searchPaths);

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

    auto scene = GameScene::createScene();
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
