#include "robot.h"
#include "strategy.h"
#include "cards.h"
#include "robotgraplord.h"
#include "robotplayhand.h"
#include <QVector>
#include <QThreadPool>
#include <DataManager.h>
#include <taskqueue.h>

Robot::Robot(QObject *parent)
    : Player{parent}
{
    type_ = Player::Robot;
}

void Robot::prepareCallLord()
{
    RobotGrapLord *task = new RobotGrapLord(this);
    QThreadPool::globalInstance()->start(task);
    // connect(subThread, &QThread::finished, this, [=](){
    //     subThread->deleteLater();
    // });
    // subThread->start();
}

void Robot::preparePlayHand()
{
    RobotPlayHand *task = new RobotPlayHand(this);
    QThreadPool::globalInstance()->start(task);
    // connect(subThread, &QThread::finished, this, [=](){
    //     subThread->deleteLater();
    // });
    // subThread->start();
}

void Robot::thinkCallLord()
{
    // 网络模式
    if (DataManager::getInstance()->mode() == DataManager::Network) {
        Task t = TaskQueue::getInstance()->take();
        grabLoadBet(t.bet);
        return;
    }

    // 单机模式
    int weight = 0;
    Strategy st(this, cards_);
    weight += st.getRangeCards(Card::Card_SJ, Card::Card_BJ).cardCount() * 6;

    QVector<Cards> optSeq = st.pickOptimalSeqSingles();
    weight += optSeq.size() * 5;

    QVector<Cards> booms = st.findCardsByCount(4);
    weight += booms.size() * 5;

    weight += cards_.pointCount(Card::Card_2) * 3;

    Cards tmp = cards_;
    tmp.remove(optSeq);
    tmp.remove(booms);
    Cards cards2 = st.getRangeCards(Card::Card_2, Card::Card_2);
    tmp.remove(cards2);
    QVector<Cards> triples = Strategy(this, tmp).findCardsByCount(3);
    weight += triples.size() * 4;

    tmp.remove(triples);
    QVector<Cards> doubles = Strategy(this, tmp).findCardsByCount(2);
    weight += doubles.size();

    if (weight >= 22) {
        grabLoadBet(3);
    }
    else if (weight < 22 && weight >= 18) {
        grabLoadBet(2);
    }
    else if (weight < 18 && weight >= 10) {
        grabLoadBet(1);
    }
    else {
        grabLoadBet(0);
    }
}

void Robot::thinkPlayHand()
{
    if (DataManager::getInstance()->mode() == DataManager::Network) {
        Task t = TaskQueue::getInstance()->take();
        playHand(t.cs);
        return;
    }
    Strategy st(this, cards_);
    Cards cs = st.makeStrategy();
    playHand(cs);
}
