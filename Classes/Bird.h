#pragma once
#ifndef __BIRD_H__
#define __BIRD_H__

#include "cocos2d.h"
#include "Definitions.h"
#include "cocostudio\SimpleAudioEngine.h"

class Bird
{
public:
	Bird(cocos2d::Scene *scene);

	void Fall();
	void Fly() 
	{ 
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Wing.mp3");
		isFalling = false;
	};
	void stopFly() { isFalling = true; };
	cocos2d::Sprite* bird;
	
	

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	bool isFalling;

    

};

#endif // __BIRD_H__

