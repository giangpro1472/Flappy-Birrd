#include "Pipe.h"
#include "Definitions.h"

USING_NS_CC;

Pipe::Pipe()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

void Pipe::SpawnPipe(cocos2d::Scene* scene)
{
	gameScene = scene;

	topPipe = Sprite::create("topPipe.png");
	lowerPipe = Sprite::create("lowerPipe.png");

	auto bg = Sprite::create("bg.png");

	topPipe->setScale(1.2, 1.5);
	lowerPipe->setScale(1.2, 1.7);

	auto topPipeBody = PhysicsBody::createBox(topPipe->getContentSize());
	auto lowerPipeBody = PhysicsBody::createBox(lowerPipe->getContentSize());

	topPipeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	topPipeBody->setContactTestBitmask(true);

	lowerPipeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	lowerPipeBody->setContactTestBitmask(true);

	auto random = CCRANDOM_0_1();

	if (random < LOWER_SCREEN_PIPE_THRESHOLD)
	{
		random = LOWER_SCREEN_PIPE_THRESHOLD;
	}
	else if (random > UPPER_SCREEN_PIPE_THRESHOLD)
	{
		random = UPPER_SCREEN_PIPE_THRESHOLD;
	}

	auto topPipePosition = (random * (visibleSize.height + 500)) + (bg->getContentSize().height / 2);

	topPipeBody->setDynamic(false);
	lowerPipeBody->setDynamic(false);

	topPipe->setPhysicsBody(topPipeBody);
	lowerPipe->setPhysicsBody(lowerPipeBody);

	topPipe->setPosition(Point(visibleSize.width + topPipe->getContentSize().width + origin.x, topPipePosition));
	lowerPipe->setPosition(Point(topPipe->getPosition().x,
		topPipePosition - (Sprite::create("bird.png")->getContentSize().height) * PIPE_GAP - topPipe->getContentSize().height));
	scene->addChild(topPipe);
	scene->addChild(lowerPipe);

	auto topPipeAction = MoveBy::create(PIPE_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width * 1.5, 0));
	auto lowerPipeAction = MoveBy::create(PIPE_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width * 1.5, 0));

	auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(Pipe::PipeMoveFinished, this));
	auto pointMoveDone = CallFuncN::create(CC_CALLBACK_1(Pipe::PointMoveFinished, this));
	

	topPipe->runAction(Sequence::create(topPipeAction, actionMoveDone, nullptr));
	lowerPipe->runAction(Sequence::create(lowerPipeAction, actionMoveDone, nullptr));

	pointNode = Node::create();
	auto pointBody = PhysicsBody::createBox(Size(1, Sprite::create("bird.png")->getContentSize().height * PIPE_GAP));
	pointBody->setDynamic(false);
	pointBody->setCollisionBitmask(POINT_COLLISION_BITMASK);
	pointBody->setContactTestBitmask(true);

	pointNode->setPhysicsBody(pointBody);
	pointNode->setPosition(Point(topPipe->getPositionX(), 
		topPipe->getPositionY() - (topPipe->getContentSize().height /2)
		- ((Sprite::create("bird.png")->getContentSize().height) * PIPE_GAP) / 2));
	scene->addChild(pointNode);

	auto pointAction = MoveBy::create(PIPE_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width * 1.5, 0));
	pointNode->runAction(Sequence::create(pointAction, pointMoveDone, nullptr));

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Pipe::onContactBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, scene);

	
}

void Pipe::PipeMoveFinished(cocos2d::Node* sender)
{
	auto sprite = (Sprite*)sender;
	gameScene->removeChild(sprite, true);
	
}

void Pipe::PointMoveFinished(cocos2d::Node* sender)
{
	gameScene->removeChild(sender, true);

}

void Pipe::gameOver()
{
	this->topPipe->stopAllActions();
	this->lowerPipe->stopAllActions();
	this->pointNode->stopAllActions();
    
}

bool Pipe::onContactBegan(cocos2d::PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())
		|| (BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
	{
		gameOver();
	}
	return true;
}