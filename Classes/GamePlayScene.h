#pragma once
#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "Pipe.h"
#include "Bird.h"
#include "cocostudio\SimpleAudioEngine.h"

class GamePlay : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void background();

    CREATE_FUNC(GamePlay);

private:
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };

    void ready();
    void play();
    void SpawnPipe(float dt);

    void MenuButton();
    void ShareButton();
    void ReturnMenu();


    Pipe pipe;
    Bird *bird;


    cocos2d::Label* scoreLabel;
    cocos2d::EventListenerTouchOneByOne* touchListener;
    cocos2d::EventListenerPhysicsContact* contactListener;

    unsigned int score;
   
    bool onContactBegin(cocos2d::PhysicsContact &contact);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
 
    void stopFlying(float dt);

    void update(float dt);

    void GameOver();

    cocos2d::PhysicsWorld* sceneWorld;
};

#endif 