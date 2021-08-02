#pragma once
#ifndef __PIPE_H__
#define __PIPE_H__

#include "cocos2d.h"
#include "Definitions.h"

class Pipe
{
public:
	Pipe();
	void SpawnPipe(cocos2d::Scene *scene);
	void gameOver();
	

	void PipeMoveFinished(cocos2d::Node* sender);
	void PointMoveFinished(cocos2d::Node* sender);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	bool onContactBegan(cocos2d::PhysicsContact& contact);

	cocos2d::Scene* gameScene;

	cocos2d::Sprite* topPipe;
	cocos2d::Sprite* lowerPipe;
	cocos2d::Node* pointNode;
	
};

#endif // __PIPE_H__
