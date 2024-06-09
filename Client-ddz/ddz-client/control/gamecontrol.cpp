#include "datamanager.h"
#include "gamecontrol.h"
#include <QRandomGenerator>
#include <QTimer>

GameControl::GameControl(QObject *parent)
    : QObject{parent}
{}

void GameControl::playerInit()
{
    robotLeft_ = new Robot("Robot A", this);            // 初始化
    robotRight_ = new Robot("Robot B", this);           // 玩家Type已在构造时完成初始化
    user_ = new UserPlayer("User", this);

    robotLeft_->setDirection(Player::Left);             // 设置方向
    robotRight_->setDirection(Player::Right);
    user_->setDirection(Player::Right);

    Player::Sex sex;                                    // 设置性别
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);       // 生成 0 1 随机数
    robotLeft_->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    robotRight_->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    user_->setSex(sex);

    user_->setPrev(robotLeft_);                         // 设置出牌顺序 上下家关系
    user_->setNext(robotRight_);
    robotLeft_->setPrev(robotRight_);
    robotLeft_->setNext(user_);
    robotRight_->setPrev(user_);
    robotRight_->setNext(robotLeft_);

    // curPlayer_ = user_;                                 // 指定当前玩家(最先叫地主)

    connect(user_, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(robotLeft_, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(robotRight_, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);

    connect(this, &GameControl::pendingInfo, robotLeft_, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, robotRight_, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, user_, &UserPlayer::storePendingInfo);

    connect(robotLeft_, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(robotRight_, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(user_, &UserPlayer::notifyPlayHand, this, &GameControl::onPlayHand);
}

void GameControl::initAllCards()
{
    if (DataManager::getInstance()->mode() == DataManager::Network) {
        allCards_ = DataManager::getInstance()->cards();
        allCards_ << DataManager::getInstance()->last();
        return;
    }
    allCards_.clear();
    for (int p=Card::Point_begin+1; p<Card::Card_SJ; p++) {
        for (int s=Card::Suit_begin+1; s<Card::Suit_end; s++) {
            Card c((Card::CardPoint)p, (Card::CardSuit)s);
            allCards_.add(c);
        }
    }
    allCards_.add(Card(Card::Card_SJ, Card::Suit_begin));
    allCards_.add(Card(Card::Card_BJ, Card::Suit_begin));
}

Card GameControl::takeOneCard()
{
    return allCards_.takeRandomCard();
}

Cards GameControl::getSurplusCards()
{// 发到最后就剩下地主牌了
    return allCards_;
}

void GameControl::resetCardData()
{
    initAllCards();
    robotLeft_->clearCards();
    robotRight_->clearCards();
    user_->clearCards();

    pendPlayer_ = nullptr;
    pendCards_.clear();
}

void GameControl::startLordCall()
{
    curPlayer_->prepareCallLord();
    emit playerStatusChange(curPlayer_, PlayerStatus::ThinkingForCallLord);
}

void GameControl::becomeLord(Player *player, int bet)
{
    curBet_ = bet;
    player->setRole(Player::Lord);
    player->prev()->setRole(Player::Farmer);
    player->next()->setRole(Player::Farmer);

    curPlayer_ = player;                        // 优先出牌
    player->storeDispatchCard(allCards_);       // 发完牌之后的3张就是地主牌

    QTimer::singleShot(1000, this, [=](){
        emit gameStatusChanged(PlayingHand);
        emit playerStatusChange(player, ThinkingForPlayHand);
        curPlayer_->preparePlayHand();              // 正式开始游戏
    });
}

void GameControl::clearPlayerScore()
{
    robotLeft_->setScore(0);
    robotRight_->setScore(0);
    user_->setScore(0);
}

int GameControl::getPlayerMaxBet()
{
    return betRecord_.bet;
}

void GameControl::setCurrentPlayer(int index)
{
    if (index == 1)
        curPlayer_ = user_;
    else if (index == 2)
        curPlayer_ = robotLeft_;
    else if (index == 3)
        curPlayer_ = robotRight_;
}

void GameControl::onGrabBet(Player *player, int bet)
{
    // 1.通知主界面玩家叫地主(更新提示信息)
    if (bet == 0 || betRecord_.bet >= bet) {    // 说明是无效分数，不可能抢到地主
        emit notifyGrabLordBet(player, 0, false);
    }
    else if (bet > 0 && betRecord_.bet == 0) {  // 第一个叫地主（音效）
        emit notifyGrabLordBet(player, bet, true);
    }
    else {                                      // 抢地主（音效）
        emit notifyGrabLordBet(player, bet, false);
    }
    // 2.判断玩家下注是不是3分，是3分直接结束
    if (bet == 3) {
        becomeLord(player, bet);         // 玩家直接成为地主
        betRecord_.reset();         // 清空record
        return;
    }
    // 3.不够3分则继续，分数最高的当地主
    if (betRecord_.bet < bet) {
        betRecord_.bet = bet;
        betRecord_.player = player;
    }
    betRecord_.times++;
    if (betRecord_.times == 3) {
        if (betRecord_.bet == 0) {  // 无人抢地主，重新发牌
            emit gameStatusChanged(DispatchCard);
        }
        else {
            becomeLord(betRecord_.player, betRecord_.bet);
        }
        betRecord_.reset();
        return;
    }
    // 4.切换玩家，通知下一个玩家抢地主
    curPlayer_ = player->next();
    emit playerStatusChange(curPlayer_, ThinkingForCallLord);
    curPlayer_->prepareCallLord();
}

void GameControl::onPlayHand(Player *player, const Cards &cards)
{
    // 将玩家出牌的信号转发给主界面
    emit notifyPlayHand(player, cards);
    // 如果不是空牌，给其他玩家发信号，保存出牌玩家对象和打出的牌
    if (!cards.isEmpty()) {
        pendCards_ = cards;
        pendPlayer_ = player;
        emit pendingInfo(player, cards);
    }

    // 如果有炸弹，底分翻倍
    auto type = PlayHand(cards).type();
    if (type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers) {
        curBet_ = curBet_ * 2;
    }

    // 如果牌出完了，计算本剧游戏的总分
    if (player->cards().isEmpty()) {
        Player *prev = player->prev();
        Player *next = player->next();
        if (player->role() == Player::Lord) {
            player->setScore(player->score() + 2 * curBet_);
            prev->setScore(prev->score() - curBet_);
            next->setScore(next->score() - curBet_);
            player->setIsWin(true);
            prev->setIsWin(false);
            next->setIsWin(false);
        }
        else {
            player->setIsWin(true);
            player->setScore(player->score() + curBet_);
            if (prev->role() == Player::Lord) {
                prev->setScore(prev->score() - 2 * curBet_);
                next->setScore(next->score() + curBet_);
                prev->setIsWin(false);
                next->setIsWin(true);
            }
            else {
                prev->setScore(prev->score() + curBet_);
                next->setScore(next->score() - 2 * curBet_);
                prev->setIsWin(true);
                next->setIsWin(false);
            }
        }
        emit playerStatusChange(player, PlayerStatus::Winning);
        return;
    }
    // 牌没有出完，通知下一个玩家继续出牌
    curPlayer_ = player->next();
    curPlayer_->preparePlayHand();
    emit playerStatusChange(curPlayer_, PlayerStatus::ThinkingForPlayHand);
}

Robot *GameControl::robotRight() const
{
    return robotRight_;
}

Robot *GameControl::robotLeft() const
{
    return robotLeft_;
}

UserPlayer *GameControl::user() const
{
    return user_;
}

Player *GameControl::curPlayer() const
{
    return curPlayer_;
}

void GameControl::setCurPlayer(Player *newCurPlayer)
{
    curPlayer_ = newCurPlayer;
}

Player *GameControl::pendPlayer() const
{
    return pendPlayer_;
}

Cards GameControl::pendCards() const
{
    return pendCards_;
}
