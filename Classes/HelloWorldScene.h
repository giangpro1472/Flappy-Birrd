
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    
    void menuCloseCallback(cocos2d::Ref* pSender);

    void bird();
    void logo();

    void playButton();
    void score();
    void play();


    void background();
    
    CREATE_FUNC(HelloWorld);
};

#endif 
