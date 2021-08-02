#include "HelloWorldScene.h"
#include "GamePlayScene.h"

USING_NS_CC;
using namespace ui;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}



bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    background();
    playButton();
    score();

    
    return true;
}

void HelloWorld::playButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto button = Button::create("start.png");
    button->setPosition(Vec2(origin.x + visibleSize.width / 2 - 180, origin.y + visibleSize.height / 2 - 270));
    button->setScale(1.4);

    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
        {
            switch (type)
            {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                play();
                break;
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
            }
        });
    this->addChild(button);
}

void HelloWorld::score()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto button = Button::create("score.png");
    button->setPosition(Vec2(origin.x + visibleSize.width / 2 + 180, origin.y + visibleSize.height / 2 - 270));
    button->setScale(1.4);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
        {
            switch (type)
            {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                break;
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
            }
        });
    this->addChild(button);
}

void HelloWorld::play()
{
    auto scene = GamePlay::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void HelloWorld::background()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    this->addChild(bg);

    auto ground = Sprite::create("ground.png");
    ground->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 480));
    ground->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    this->addChild(ground);

    auto pub = Sprite::create("pub.png");
    pub->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 420));
    pub->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    this->addChild(pub, 3);

    logo();
    bird();
}

void HelloWorld::bird()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto bg = Sprite::create("bg.png");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("spritesheet.plist", "spritesheet.png");
    const int numberSprite = 3;

    auto gameSprite = Sprite::createWithSpriteFrameName("bird.png");
    gameSprite->setPosition(Vec2(origin.x + visibleSize.width / 2 + 240, origin.y + visibleSize.height / 2 + 245));
    gameSprite->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    this->addChild(gameSprite);

    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(numberSprite);
    animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird.png"));
    animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2.png"));
    animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird3.png"));
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
    Animate* animate = Animate::create(animation);

    auto move = MoveBy::create(0.5, Vec2(0, 25));
    auto move_back = move->reverse();
    auto seq = Sequence::create(move, move_back, nullptr);

    gameSprite->runAction(RepeatForever::create(seq));
    gameSprite->runAction(RepeatForever::create(animate));
}

void HelloWorld::logo()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto logo = Sprite::create("logo.png");
    logo->setPosition(Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 + 240));
    logo->setScale(1.5);
    this->addChild(logo);

    auto move = MoveBy::create(0.5, Vec2(0, 25));
    auto move_back = move->reverse();
    auto seq = Sequence::create(move, move_back, nullptr);

    logo->runAction(RepeatForever::create(seq));
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
