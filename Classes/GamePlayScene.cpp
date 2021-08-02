#include "GamePlayScene.h"
#include "HelloWorldScene.h"
#include "Definitions.h"


USING_NS_CC;
using namespace ui;
using namespace std;

unsigned int currentScore;

Scene* GamePlay::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

    scene->addChild(GamePlay::create());

    return scene;
}

bool GamePlay::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    score = 0;

    background();

    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask(WORLD_COLLISION_BITMASK);
    edgeBody->setContactTestBitmask(true);

    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);

    play();
    
    return true;
}


//GamePlay
void GamePlay::SpawnPipe(float dt)
{
    pipe.SpawnPipe(this);
}

bool GamePlay::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();

    if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())
        || (BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(GamePlay::SpawnPipe));
        Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
        Director::getInstance()->getEventDispatcher()->removeEventListener(contactListener);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Hit.mp3");
        GameOver();   
    }
    else if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && POINT_COLLISION_BITMASK == b->getCollisionBitmask())
        || (BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && POINT_COLLISION_BITMASK == a->getCollisionBitmask()))
    {
        
        this->score++;
        stringstream ss;
        ss << score;
        string str = ss.str();
        scoreLabel->setString(str);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Point.mp3");
        currentScore = score;
       
    }
    //else if ((DELETE_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())
     //   || (DELETE_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
   // {
      //  pipe.DeletePipe(this);
   // }
    return true;
}

bool GamePlay::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    bird->Fly();
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(GamePlay::stopFlying, this), BIRD_FLYING_DURATION);
    return true;
}

void GamePlay::stopFlying(float dt)
{
    bird->stopFly();
}

void GamePlay::update(float dt)
{
    bird->Fall();
}

void GamePlay::play()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    bird = new Bird(this);
   
    this->schedule(CC_SCHEDULE_SELECTOR(GamePlay::SpawnPipe), PIPE_SPAWN_FREQUENCY * visibleSize.width);

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    this->scheduleUpdate();


    contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GamePlay::onContactBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    stringstream ss;
    ss << score;
    string str = ss.str();

    scoreLabel = Label::createWithTTF(str, "fonts/flappy.ttf", 45);
    scoreLabel->enableOutline(Color4B::BLACK, 3);
    scoreLabel->enableShadow();
    scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - 50));
    this->addChild(scoreLabel, 3);
   
}

//GameOver 
void GamePlay::GameOver()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int tempScore;
    tempScore = score;

    auto fadeOut = FadeOut::create(0);
    auto fadeIn = FadeIn::create(0);
    auto delay = DelayTime::create(1);
    auto delay2 = DelayTime::create(1.5);
    auto delay3 = DelayTime::create(1.7);

    scoreLabel->runAction(fadeOut);

    auto gameOver = Sprite::create("gameover.png");
    auto bg = Sprite::create("bg.png");

    gameOver->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 210));
    gameOver->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    gameOver->setOpacity(0);

    auto seq = Sequence::create(delay, fadeIn, nullptr);
    gameOver->runAction(seq);
    this->addChild(gameOver);

    auto best = Sprite::create("bestScore.png");
    best->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
    best->setScale(1.5);
    best->setOpacity(0);
    auto moveBy = MoveBy::create(0.2, Vec2(0, 100));

    auto seq2 = Sequence::create(delay2, fadeIn, moveBy, nullptr);

    best->runAction(seq2);
    this->addChild(best);

    stringstream ss;
    ss << tempScore;
    string str = ss.str();

    auto yourScore = Label::createWithTTF(str, "fonts/flappy.ttf", 42);
    yourScore->enableOutline(Color4B::BLACK, 3);
    yourScore->enableShadow();
    yourScore->setPosition(Vec2(origin.x + visibleSize.width / 2 + 165, origin.y + visibleSize.height / 2 + 32));
    yourScore->setOpacity(0);

    auto seq3 = Sequence::create(delay3, fadeIn, nullptr);

    yourScore->runAction(seq3);
    this->addChild(yourScore, 3);

    UserDefault* def = UserDefault::getInstance();
    auto highScore = def->getIntegerForKey("HighScore", 0);

    if (currentScore > highScore)
    {
        highScore = currentScore;
        def->getIntegerForKey("HighScore", highScore);
    }
    def->flush();

    stringstream ss2;
    ss2 << highScore;
    string str2 = ss2.str();

    auto highScoreLable = Label::createWithTTF(str2, "fonts/flappy.ttf", 42);
    highScoreLable->enableOutline(Color4B::BLACK, 3);
    highScoreLable->enableShadow();
    highScoreLable->setPosition(Vec2(origin.x + visibleSize.width / 2 + 165, origin.y + visibleSize.height / 2 - 62));
    highScoreLable->setOpacity(0);

    auto seq4 = Sequence::create(delay3, fadeIn, nullptr);
    highScoreLable->runAction(seq4);
    this->addChild(highScoreLable, 3);

    if (score >= 10 && score < 20)
    {
        auto medal = Sprite::create("cooper.png");
        medal->setPosition(Vec2(origin.x + visibleSize.width / 2 - 150, origin.y + visibleSize.height / 2 - 20));
        medal->setScale(1.5);
        medal->setOpacity(0);
        auto delay4 = DelayTime::create(1.8);
        auto seq5 = Sequence::create(delay4, fadeIn, nullptr);
        medal->runAction(seq5);
        this->addChild(medal, 10);
    }
    else if (score >= 20 && score < 30)
    {
        auto medal = Sprite::create("silver.png");
        medal->setPosition(Vec2(origin.x + visibleSize.width / 2 - 150, origin.y + visibleSize.height / 2 - 20));
        medal->setScale(1.5);
        medal->setOpacity(0);
        auto delay4 = DelayTime::create(1.8);
        auto seq5 = Sequence::create(delay4, fadeIn, nullptr);
        medal->runAction(seq5);
        this->addChild(medal, 10);
    }
    else if (score >= 30 && score < 40)
    {
        auto medal = Sprite::create("gold.png");
        medal->setPosition(Vec2(origin.x + visibleSize.width / 2 - 150, origin.y + visibleSize.height / 2 - 20));
        medal->setScale(1.5);
        medal->setOpacity(0);
        auto delay4 = DelayTime::create(1.8);
        auto seq5 = Sequence::create(delay4, fadeIn, nullptr);
        medal->runAction(seq5);
        this->addChild(medal, 10);
    }
    else if (score >= 40)
    {
        auto medal = Sprite::create("platinum.png");
        medal->setPosition(Vec2(origin.x + visibleSize.width / 2 - 150, origin.y + visibleSize.height / 2 - 20));
        medal->setScale(1.5);
        medal->setOpacity(0);
        auto delay4 = DelayTime::create(1.8);
        auto seq5 = Sequence::create(delay4, fadeIn, nullptr);
        medal->runAction(seq5);
        this->addChild(medal, 10);
    }
    MenuButton();
    ShareButton();
}

void GamePlay::MenuButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto button = Button::create("menu.png");
    button->setPosition(Vec2(origin.x + visibleSize.width / 2 - 180, origin.y + visibleSize.height / 2 - 270));
    button->setScale(1.4);
    button->setOpacity(0);

    auto fadeIn = FadeIn::create(0);
    auto delay = DelayTime::create(1.8);
    auto seq = Sequence::create(delay, fadeIn, nullptr);

    button->runAction(seq);

    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
        {
            switch (type)
            {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                ReturnMenu();
                break;
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
            }
        });
    this->addChild(button);
}

void GamePlay::ReturnMenu()
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void GamePlay::ShareButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto button = Button::create("share.png");
    button->setPosition(Vec2(origin.x + visibleSize.width / 2 + 180, origin.y + visibleSize.height / 2 - 270));
    button->setScale(1.4);
    button->setOpacity(0);

    auto fadeIn = FadeIn::create(0);
    auto delay = DelayTime::create(1.8);
    auto seq = Sequence::create(delay, fadeIn, nullptr);

    button->runAction(seq);

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



//BackGround
void GamePlay::background()
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

    auto groundBody = PhysicsBody::createBox(ground->getContentSize());
    groundBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
    groundBody->setContactTestBitmask(true);
    groundBody->setDynamic(false);

    ground->setPhysicsBody(groundBody);
    this->addChild(ground, 1);

    //auto endNode = Node::create();
    //auto endBody = PhysicsBody::createBox(Size(1, Sprite::create("bird.png")->getContentSize().height * (PIPE_GAP + 20)));
    //endBody->setDynamic(false);
    //endBody->setCollisionBitmask(DELETE_COLLISION_BITMASK);
   // endBody->setContactTestBitmask(true);

    //endNode->setPhysicsBody(endBody);
  //  endNode->setPosition(Vec2(visibleSize.width / 2 - 500, visibleSize.height / 2 + 300));
   // this->addChild(endNode);
}

void GamePlay::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}