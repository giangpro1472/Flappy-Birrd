#include "Bird.h"
#include "Definitions.h"

USING_NS_CC;
Bird::Bird(cocos2d::Scene *scene)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("bg.png");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("spritesheet.plist", "spritesheet.png");
	const int numberSprite = 3;
	bird = Sprite::createWithSpriteFrameName("bird.png");
    bird->setScale(1.15);
    bird->setPosition(Vec2(visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 + 500));

    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(numberSprite);
    animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird.png"));
    animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2.png"));
    animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird3.png"));
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
    Animate* animate = Animate::create(animation);

    bird->runAction(RepeatForever::create(animate));

    auto birdBody = PhysicsBody::createCircle(bird->getContentSize().width / 2);
    birdBody->setCollisionBitmask(BIRD_COLLISION_BITMASK);
    birdBody->setContactTestBitmask(true);

    bird->setPhysicsBody(birdBody);

    isFalling = true;

    scene->addChild(bird, 2);
}



void Bird::Fall()
{
   if (isFalling == true)
   {
        bird->setPositionX(visibleSize.width / 2 - 100);
        bird->setPositionY(bird->getPositionY() - (BIRD_FALLING_SPEED * visibleSize.height));
        bird->setRotation(20.0);
   }
   else
   {
      bird->setPositionX(visibleSize.width / 2 - 100);
      bird->setPositionY(bird->getPositionY() + (BIRD_FLYING_SPEED * visibleSize.height));
      bird->setRotation(-20.0);
   }
}
