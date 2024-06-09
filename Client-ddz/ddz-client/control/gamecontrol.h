#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "userplayer.h"
#include "player.h"
#include "playhand.h"

struct BetRecord    // 记录玩家的下注信息
{
    BetRecord() {
        reset();
    }
    void reset() {
        player = nullptr;
        bet = 0;
        times = 0;
    }
    Player *player;
    int bet;
    int times;
};

class GameControl : public QObject
{
    Q_OBJECT
public:
    // 游戏状态
    enum GameStatus {
        DispatchCard,           // 发牌阶段
        CallingLord,            // 叫地主阶段
        PlayingHand             // 打牌阶段
    };

    // 玩家状态
    enum PlayerStatus {
        ThinkingForCallLord,    // 叫地主时间
        ThinkingForPlayHand,    // 出牌时间
        Winning
    };

    explicit GameControl(QObject *parent = nullptr);

    void playerInit();                  // 初始化玩家
    void initAllCards();                // 初始化一套牌

    Card takeOneCard();                 // 从一套牌里取一张(发牌)
    Cards getSurplusCards();            // 获取牌堆剩下的卡牌，也可用于获取三张底牌(地主牌)
    void resetCardData();               // 重置卡牌(游戏结束准备下一局)

    void startLordCall();               // 准备叫地主
    void becomeLord(Player *player, int bet);    // 成为地主(叫地主阶段结束)
    void clearPlayerScore();            // 清空玩家分数

    int getPlayerMaxBet();

    void setCurrentPlayer(int index);      // 设置当前玩家

    // slot
    void onGrabBet(Player *player, int bet);
    void onPlayHand(Player *player, const Cards& cards);

    // get / set
    Robot *robotRight() const;

    Robot *robotLeft() const;

    UserPlayer *user() const;

    Player *curPlayer() const;
    void setCurPlayer(Player *newCurPlayer);

    Player *pendPlayer() const;

    Cards pendCards() const;

signals:
    void playerStatusChange(Player *player, PlayerStatus status);
    // 通知玩家抢地主了
    void notifyGrabLordBet(Player *player, int bet, bool flag);
    // 游戏状态变化
    void gameStatusChanged(GameStatus status);
    // 出牌
    void notifyPlayHand(Player *player, const Cards &cards);
    // 给玩家传递出牌数据
    void pendingInfo(Player *player, const Cards &cards);

private:
    Robot *robotRight_;         // 人机 右
    Robot *robotLeft_;          // 人机 左
    UserPlayer *user_;          // 玩家

    Player *curPlayer_;         // 当前玩家(最先叫地主)
    Player *pendPlayer_;        // 当前出牌最大玩家
    Cards pendCards_;           // 当前最大牌

    Cards allCards_;            // 整套牌
    BetRecord betRecord_;        // 下注信息

    int curBet_;                // 当前下注的分数
};

#endif // GAMECONTROL_H
