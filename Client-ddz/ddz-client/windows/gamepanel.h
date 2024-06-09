#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QLabel>
#include <QTimer>
#include "countdown.h"
#include "gamecontrol.h"
#include "cardpanel.h"
#include "animationwindow.h"
#include "bgmcontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    using orderMap = QMap<int, QPair<QByteArray, int>>;

    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    enum AnimationType {
        ShunZi,
        LianDui,
        Plane,
        JokerBomb,
        Bomb,
        Bet
    };

    void gameControlInit();                         // 实例化游戏控制类对象
    void updatePlayerScore();                       // 更新玩家分数
    void initCardMap();                             // 切割并存储图片
    void cropImage(QPixmap &pix, int x, int y, Card &card);     // 裁剪图片
    void initButtonGroup();                         // 给buttonGroup按钮的信号做connect
    void initPlayerContext();                       // 初始化玩家上下文信息
    void initGameScene();                           // 初始化游戏场景
    void initCountDown();                           // 初始化倒计时类

    void gameStatusPrecess(GameControl::GameStatus status);     // 更改游戏状态
    void startDispatchCard();                       // 处理发牌阶段

    void disposCard(Player *player, const Cards &cards);  // 玩家显示手牌
    void updatePlayerCards(Player *player);         // 更新卡牌在窗口的显示

    void showAnimation(AnimationType type, int bet = 0);    // 动画效果

    void hidePlayerDropCard(Player* player);        // 隐藏上次的出牌
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    void showEndingScorePanel();

    // 处理网络模式下玩家的发牌次序和分数
    void initGamePanel(QByteArray msg);
    // 更新用户名和分数
    void updatePlayerInfo(orderMap &info);
    // 开始游戏
    void startGame();
    void startGame(int index);

    // 通知服务器当前玩家出牌了
    void notifyOtherPlayHand(Cards &cs);

    // slot
    void onDispatchCard();                          // 定时器处理动作
    void onPlayerStatusChange(Player *player, GameControl::PlayerStatus status);
    void onGrabLordBet(Player *player, int bet, bool flag);
    void onDisposePlayHand(Player *player, const Cards &cards);   // 处理玩家打出去的牌
    void onCardSelected(Qt::MouseButton button);
    void onUserPlayHand();
    void onUserPass();

signals:
    void panelClose();

private:
    enum CardAlign {Horizontal, Vertical};
    struct PlayerContext
    {
        QRect cardRect;                 // 玩家手牌显示的最大区域(不是实际)
        QRect playHandRect;             // 出牌的区域
        CardAlign align;                // 手牌以垂直/水平显示
        bool isFrontSide;               // 显示正面or背面
        QLabel *info;                   // 提示
        QLabel *roleImg;                // 头像
        Cards lastCards;                // 刚打出的牌
    };

    Ui::GamePanel *ui;

    QPixmap bkImage_;                   // 背景图片
    GameControl *gameCtl_;              // 游戏控制类
    QVector<Player*> playerList_;       // 存储3个玩家的列表

    QMap<Card, CardPanel*> cardMap_;    // 卡牌及其对应窗口
    QSize cardSize_;                    // 在card.png中 单张卡牌的size
    QPixmap cardBackImage_;             // 卡背的图片

    QMap<Player*, PlayerContext> contextMap_;     // 玩家的上下文信息

    CardPanel *baseCard_;               // 牌堆
    // CardPanel *moveCard_;               // 发牌时表示发出去的那张
    QVector<CardPanel*> last3Card_;     // 最后3张地主牌
    QPoint baseCardPos_;                // 发牌时牌堆的位置

    GameControl::GameStatus gameStatus_;    // 游戏状态
    QTimer *timer_;                     // 定时器 用于处理游戏状态

    AnimationWindow *animation_;        // 特效

    CardPanel *curSelPanel_;
    QSet<CardPanel*> selectCards_;

    QRect cardsRect_;                   // 玩家手牌显示的实际区域
    QHash<CardPanel*, QRect> userCards_;    // 玩家每张手牌对应的区域

    CountDown *countDown_;

    BGMControl *bgm_;

    QByteArrayList nameList_;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
};


#endif // GAMEPANEL_H
