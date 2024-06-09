#include "gamepanel.h"
#include "datamanager.h"
#include "endingpanel.h"
#include "ui_gamepanel.h"

#include <QCloseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QPropertyAnimation>

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);

    // 1. 背景图
    int random = QRandomGenerator::global()->bounded(1, 11);    // 生成1~10的随机数
    bkImage_.load(QString(":/image/images/background-%1.png").arg(random));

    // 2. 窗口标题的大小
    setWindowTitle("斗地主");
    setFixedSize(1000, 650);

    // 3. 实例化游戏控制类对象
    gameControlInit();

    // 4. 玩家得分
    updatePlayerScore();

    // 5. 切割游戏图片
    initCardMap();

    // 6. 初始化游戏中的按钮
    initButtonGroup();

    // 7. 初始化玩家在窗口中的上下文环境
    initPlayerContext();

    // 8. 扑克牌场景初始化
    initGameScene();

    // 9. 倒计时初始化
    initCountDown();

    // 实例化定时器
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &GamePanel::onDispatchCard);

    animation_ = new AnimationWindow(this);

    bgm_ = new BGMControl(this);

    Communication *comm = DataManager::getInstance()->communication();
    connect(comm, &Communication::startGame, this, [=](QByteArray msg){
        initGamePanel(msg);
        gameStatusPrecess(GameControl::DispatchCard);
        bgm_->startBGM(80);     // 播放背景音乐
    });
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameControlInit()
{
    gameCtl_ = new GameControl(this);
    gameCtl_->playerInit();
    // 得到三个玩家的实例
    playerList_ << gameCtl_->robotLeft() << gameCtl_->robotRight() << gameCtl_->user();

    connect(gameCtl_, &GameControl::playerStatusChange, this, &GamePanel::onPlayerStatusChange);
    connect(gameCtl_, &GameControl::notifyGrabLordBet, this, &GamePanel::onGrabLordBet);
    connect(gameCtl_, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
    connect(gameCtl_, &GameControl::notifyPlayHand, this, &GamePanel::onDisposePlayHand);

    connect(gameCtl_->robotLeft(), &Player::notifyPickCards, this, &GamePanel::disposCard);
    connect(gameCtl_->robotRight(), &Player::notifyPickCards, this, &GamePanel::disposCard);
    connect(gameCtl_->user(), &Player::notifyPickCards, this, &GamePanel::disposCard);

    nameList_ << gameCtl_->robotLeft()->name().toUtf8()
              << gameCtl_->user()->name().toUtf8()
              << gameCtl_->robotRight()->name().toUtf8();
}

void GamePanel::updatePlayerScore()
{
    ui->scorePanel->setScore(playerList_[0]->score(),
                             playerList_[1]->score(),
                             playerList_[2]->score());
}

void GamePanel::initCardMap()
{
    // 加载大图
    QPixmap pixmap(":/image/images/card.png");
    // 计算每张图片大小
    cardSize_.setWidth(pixmap.width() / 13);
    cardSize_.setHeight(pixmap.height() / 5);

    // 卡背图片
    cardBackImage_ = pixmap.copy(cardSize_.width() * 2, cardSize_.height() * 4,
                                 cardSize_.width(), cardSize_.height());
    // 正常花色
    for (int i=0, suit=Card::Suit_begin+1; suit<Card::Suit_end; ++suit, i++) {
        for (int j=0, pt=Card::Point_begin+1; pt<Card::Card_SJ; j++, pt++) {
            Card tmp((Card::CardPoint)pt, (Card::CardSuit)suit);
            cropImage(pixmap, j*cardSize_.width(), i*cardSize_.height(), tmp);
        }
    }
    // 大小王
    Card card(Card::Card_SJ, Card::Suit_begin);
    cropImage(pixmap, 0, cardSize_.height() * 4, card);                     // 裁剪小王
    card.setPoint(Card::Card_BJ);
    cropImage(pixmap, cardSize_.width(), cardSize_.height() * 4, card);     // 裁剪大王
}

void GamePanel::cropImage(QPixmap &pix, int x, int y, Card &card)
{
    // 从大图pix上裁剪某卡牌的正面图片
    QPixmap sub = pix.copy(x, y, cardSize_.width(), cardSize_.height());
    CardPanel *panel = new CardPanel(this);
    panel->setImage(sub, cardBackImage_);
    panel->setCard(card);
    panel->hide();                  // 暂时不需要显示
    cardMap_.insert(card, panel);   // 保存到map中
    connect(panel, &CardPanel::cardSelected, this, &GamePanel::onCardSelected);
}

void GamePanel::initButtonGroup()
{
    ui->btnGroup->initButton();
    ui->btnGroup->selectPanel(ButtonGroup::Panel::Start, 0);

    void (GamePanel::*startGame)() = &GamePanel::startGame;
    connect(ui->btnGroup, &ButtonGroup::startGame, this, startGame);
    connect(ui->btnGroup, &ButtonGroup::playHand, this, &GamePanel::onUserPlayHand);
    connect(ui->btnGroup, &ButtonGroup::pass, this, &GamePanel::onUserPass);
    connect(ui->btnGroup, &ButtonGroup::betPoint, this, [=](int bet){
        gameCtl_->user()->grabLoadBet(bet);
        ui->btnGroup->selectPanel(ButtonGroup::Empty, 0);
        if (DataManager::getInstance()->mode() == DataManager::Network) {
            Message msg;
            msg.userName = DataManager::getInstance()->userName();
            msg.roomName = DataManager::getInstance()->roomName();
            msg.data1 = QByteArray::number(bet);
            msg.requestCode = RequestCode::GrabLoad;
            DataManager::getInstance()->communication()->sendMessage(&msg, true);
        }
    });
}

void GamePanel::initPlayerContext()
{
    // 放置玩家手牌的区域 (x, y, width, height)
    QRect cardsRect[] = {
        QRect(90, 130, 100, height() - 200),                    // 左侧人机
        QRect(rect().right() - 190, 130, 100, height() - 200),  // 右侧人机
        QRect(250, rect().bottom() - 120, width() - 500, 100)   // 玩家
    };

    // 玩家出牌的区域
    QRect playHandRect[] = {
        QRect(260, 150, 100, 100),                              // 左侧人机
        QRect(rect().right() - 360, 150, 100, 100),             // 右侧人机
        QRect(150, rect().bottom() - 290, width() - 300, 100)   // 玩家
    };

    // 玩家头像显示位置
    QPoint roleImgPos[] = {
        QPoint(cardsRect[0].left()-80, cardsRect[0].height()/2+20),     // 左侧人机
        QPoint(cardsRect[1].right()+10, cardsRect[1].height()/2+20),    // 右侧人机
        QPoint(cardsRect[2].left()-10, cardsRect[2].top()-10)           // 玩家
    };

    for (int i=0; i<playerList_.size(); i++) {
        PlayerContext context;
        context.align = i==2? CardAlign::Horizontal : CardAlign::Vertical;
        context.isFrontSide = i == 2;
        context.cardRect = cardsRect[i];
        context.playHandRect = playHandRect[i];
        // 提示信息
        context.info = new QLabel(this);
        context.info->resize(160, 98);
        context.info->hide();
        QPoint pt(playHandRect[i].left() + (playHandRect[i].width() - context.info->width()) / 2,
                  playHandRect[i].top() + (playHandRect[i].height() - context.info->height()) / 2);
        context.info->move(pt);
        // 头像
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84, 120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        contextMap_.insert(playerList_[i], context);
    }
}

void GamePanel::initGameScene()
{
    // 1. 发牌区的扑克
    baseCard_ = new CardPanel(this);
    baseCard_->setImage(cardBackImage_, cardBackImage_);
    baseCard_->setFrontSide(false);
    // 2. 发牌过程中移动的卡牌
    // moveCard_ = new CardPanel(this);
    // moveCard_->setImage(cardBackImage_, cardBackImage_);
    // moveCard_->setFrontSide(false);
    // 3. 最后的3张地主牌
    for (int i=0; i<3; i++) {
        auto temp = new CardPanel(this);
        temp->setImage(cardBackImage_, cardBackImage_);
        temp->hide();
        last3Card_.push_back(temp);
    }

    // 牌堆位置
    baseCardPos_ = QPoint((width() - cardSize_.width()) / 2,
                          height() / 2 - 100);
    baseCard_->move(baseCardPos_);
    // moveCard_->move(baseCardPos_);

    // 地主牌位置
    int baseX = (width() - 3*cardSize_.width() - 20) / 2;
    for (int i=0; i<3; i++) {
        last3Card_[i]->move(baseX + i*(cardSize_.width()+10), 20);
    }
}

void GamePanel::initCountDown()
{
    countDown_ = new CountDown(this);
    countDown_->move((width() - countDown_->width())/2, (height() - countDown_->height())/2 + 30);
    auto user = gameCtl_->user();

    connect(countDown_, &CountDown::notMuchTime, this, [=](){
        // 音乐，提示所剩时间不多
        bgm_->playAssistMusic(BGMControl::Alert);
    });
    connect(countDown_, &CountDown::timeout, this, &GamePanel::onUserPass);     // 强制玩家跳过
    connect(user, &UserPlayer::startCountDown, this, [=](){
        if (gameCtl_->pendPlayer() && user != gameCtl_->pendPlayer()) {
            countDown_->showCountDown();
        }
    });
}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
    // 记录游戏状态
    gameStatus_ = status;
    // 处理游戏状态
    switch (gameStatus_) {
    case GameControl::DispatchCard: {
        startDispatchCard();
        break;
    }
    case GameControl::CallingLord: {
        CardList list = gameCtl_->getSurplusCards().toCardList();   // 获取剩下的3张牌
        for (int i=0; i<list.size(); i++) {
            QPixmap front = cardMap_[list[i]]->getImage();
            last3Card_[i]->setImage(front, bkImage_);
            last3Card_[i]->hide();
        }
        gameCtl_->startLordCall();      // 从玩家开始叫地主(user默认是当前玩家)
        break;
    }
    case GameControl::PlayingHand: {
        baseCard_->hide();
        updatePlayerCards(gameCtl_->curPlayer());
        for (int i = 0; i < last3Card_.size(); i++) {
            last3Card_[i]->show();
        }
        for (int i=0; i<playerList_.size(); i++) {
            Player *player = playerList_[i];
            PlayerContext &context = contextMap_[player];
            context.info->hide();
            QPixmap pixmap = loadRoleImage(player->sex(), player->direction(), player->role());
            context.roleImg->setPixmap(pixmap);
            context.roleImg->show();
        }
        break;
    }
    default:
        break;
    }
}

void GamePanel::startDispatchCard()
{
    // 重置每张卡牌的属性
    for (auto it=cardMap_.begin(); it!=cardMap_.end(); it++) {
        it.value()->setFrontSide(true);
        it.value()->setIsSelected(false);
        it.value()->hide();
    }
    // 开始下一局前隐藏地主牌
    for (int i=0; i<last3Card_.size(); i++) {
        last3Card_[i]->hide();
    }
    // 重置玩家的上下文信息
    for (int i=0; i<playerList_.size(); i++) {
        contextMap_.find(playerList_[i]).value().lastCards.clear();
        contextMap_.find(playerList_[i]).value().info->hide();
        contextMap_.find(playerList_[i]).value().roleImg->hide();
        contextMap_.find(playerList_[i]).value().isFrontSide = i==2? true : false;
    }
    // 重置所有玩家的手牌
    gameCtl_->resetCardData();
    // 显示牌堆
    baseCard_->show();
    // 发牌过程中隐藏buttonGroup
    ui->btnGroup->selectPanel(ButtonGroup::Empty, 0);
    // 启动定时器， 每10ms触发一次timeout
    timer_->start(40);
    // 播放bgm
    bgm_->playAssistMusic(BGMControl::Dispatch);
}

void GamePanel::onDispatchCard()
{// 用定时器来发牌
    auto player = gameCtl_->curPlayer();
    // 给当前玩家发一张牌
    Card card = gameCtl_->takeOneCard();        // 从牌堆中取一张牌
    player->storeDispatchCard(card);            // player存储这张牌
    // Cards tmp;
    // tmp << card;
    // disposCard(player, tmp);
    // 切换玩家
    gameCtl_->setCurPlayer(player->next());

    if (gameCtl_->getSurplusCards().cardCount() == 3) {     // 当只剩3张牌时停止定时器
        timer_->stop();                                     // 并切换到叫地主阶段
        gameStatusPrecess(GameControl::CallingLord);
        bgm_->stopAssistMusic();
    }
}

void GamePanel::onPlayerStatusChange(Player *player, GameControl::PlayerStatus status)
{
    switch (status) {
    case GameControl::ThinkingForCallLord:
        if (player == gameCtl_->user())
            ui->btnGroup->selectPanel(ButtonGroup::CallLord, gameCtl_->getPlayerMaxBet());
        break;
    case GameControl::ThinkingForPlayHand: {
        hidePlayerDropCard(player);     // 隐藏上一轮打出的牌
        if (player == gameCtl_->user()) {
            if (player->pendPlayer() == nullptr || player == player->pendPlayer()) {
                ui->btnGroup->selectPanel(ButtonGroup::PlayCard, 0);
            }
            else{
                ui->btnGroup->selectPanel(ButtonGroup::PassOrPlay, 0);
            }
        }
        else {
            ui->btnGroup->selectPanel(ButtonGroup::Empty, 0);
        }
        break;
    }
    case GameControl::Winning: {
        bgm_->stopBGM();
        contextMap_[gameCtl_->robotLeft()].isFrontSide = true;
        contextMap_[gameCtl_->robotRight()].isFrontSide = true;
        updatePlayerCards(gameCtl_->robotLeft());
        updatePlayerCards(gameCtl_->robotRight());
        updatePlayerScore();
        gameCtl_->setCurPlayer(player);
        for (auto player : playerList_) {       // 重置pendInfo
            Cards temp;
            player->setPendingInfo(nullptr, temp);
        }
        showEndingScorePanel();

        // 更新积分 --服务器的数据库mysql/redis
        Message msg;
        msg.userName = DataManager::getInstance()->userName();
        msg.roomName = DataManager::getInstance()->roomName();
        msg.data1 = QByteArray::number(gameCtl_->user()->score());
        msg.requestCode = RequestCode::GameOver;
        DataManager::getInstance()->communication()->sendMessage(&msg, true);
        break;
    }
    default:
        break;
    }
}

void GamePanel::onGrabLordBet(Player *player, int bet, bool flag)
{
    // 显示抢地主的信息提示
    auto context = contextMap_[player];
    if (bet == 0) {
        context.info->setPixmap(QPixmap(":/image/images/buqinag.png"));
    }
    else {
        if (flag) {     // 叫地主
            context.info->setPixmap(QPixmap(":/image/images/jiaodizhu.png"));
        }
        else {          // 抢地主
            context.info->setPixmap(QPixmap(":/image/images/qiangdizhu.png"));
        }
        showAnimation(Bet, bet);
    }
    context.info->show();

    bgm_->playerRobLordMusic(bet, (BGMControl::RoleSex)player->sex(), flag);
}

void GamePanel::onDisposePlayHand(Player *player, const Cards &cards)
{
    // 根据牌型播放特效
    auto context = contextMap_.find(player);
    context->lastCards = cards;

    class PlayHand hand(cards);
    PlayHand::HandType type = hand.type();
    if (type == PlayHand::Hand_Plane ||
        type == PlayHand::Hand_Plane_Two_Pair ||
        type == PlayHand::Hand_Plane_Two_Single) {
        showAnimation(AnimationType::Plane);
    }
    else if (type == PlayHand::Hand_Seq_Pair) {
        showAnimation(AnimationType::LianDui);
    }
    else if (type == PlayHand::Hand_Seq_Single) {
        showAnimation(AnimationType::ShunZi);
    }
    else if (type == PlayHand::Hand_Bomb) {
        showAnimation(AnimationType::Bomb);
    }
    else if (type == PlayHand::Hand_Bomb_Jokers) {
        showAnimation(AnimationType::JokerBomb);
    }
    if (cards.isEmpty()) {
        context->info->setPixmap(QPixmap(":/image/images/pass.png"));
        context->info->show();
        bgm_->playPassMusic((BGMControl::RoleSex)player->sex());
    }
    else {
        if (gameCtl_->pendPlayer() == nullptr || gameCtl_->pendPlayer() == player) {
            bgm_->playCardMusic(cards, true, (BGMControl::RoleSex)player->sex());
        }
        else {
            bgm_->playCardMusic(cards, false, (BGMControl::RoleSex)player->sex());
        }
    }
    // 更新玩家剩余的牌
    updatePlayerCards(player);
    // 播放提示音乐
    if (!cards.isEmpty() && player->cards().cardCount() == 2)
        bgm_->playLastMusic(BGMControl::Last2, (BGMControl::RoleSex)player->sex());
    else if (!cards.isEmpty() && player->cards().cardCount() == 1)
        bgm_->playLastMusic(BGMControl::Last1, (BGMControl::RoleSex)player->sex());
}

void GamePanel::onCardSelected(Qt::MouseButton button)
{
    // 判断是不是出牌状态
    if (gameStatus_ != GameControl::PlayingHand)
        return;

    // 判断发出信号的牌的所有者是不是当前玩家
    CardPanel *panel = static_cast<CardPanel*>(sender());
    if (panel->getOwner() != gameCtl_->curPlayer())
        return;

    // 保存当前被选中的牌的窗口对象
    curSelPanel_ = panel;
    if (button == Qt::LeftButton) {                     // 判断参数button是左键还是右键
        panel->setIsSelected(!panel->isSelected());     // 设置卡牌的选中状态
        updatePlayerCards(panel->getOwner());           // 更新卡牌在窗口的显示
        auto it = selectCards_.find(panel);             // 保存或删除卡牌窗口对象
        if (it == selectCards_.end())
            selectCards_.insert(panel);
        else
            selectCards_.erase(it);
        bgm_->playAssistMusic(BGMControl::SelectCard);
    }
    else if (button == Qt::RightButton) {
        // 调用出牌按钮槽函数
        onUserPlayHand();
    }
}

void GamePanel::onUserPlayHand()
{
    if (gameStatus_ != GameControl::GameStatus::PlayingHand ||  // 判断游戏状态
        gameCtl_->user() != gameCtl_->curPlayer() ||            // 判断玩家是不是用户
        selectCards_.empty())                                   // 判断要出的牌是否为空
        return;

    // 得到要打出的牌的牌型
    Cards cs;
    for (const auto& it : selectCards_) {
        cs << it->getCard();
    }
    PlayHand hand(cs);
    PlayHand::HandType type = hand.type();
    if (type == PlayHand::Hand_Unknown)
        return;

    // 判断当前玩家的牌型能否压上家的牌
    if (gameCtl_->pendPlayer() != gameCtl_->user()) {
        Cards cards = gameCtl_->pendCards();
        if (!hand.canBeat(PlayHand(cards)))
            return;
    }

    gameCtl_->user()->playHand(cs);         // 通过玩家对象出牌
    notifyOtherPlayHand(cs);
    selectCards_.clear();                   // 清空容器
    countDown_->stopCountDown();
}

void GamePanel::onUserPass()
{
    countDown_->stopCountDown();
    Player *user = gameCtl_->user();
    Player *curPlay = gameCtl_->curPlayer();
    Player *pendPlayer = gameCtl_->pendPlayer();
    if (user != curPlay ||              // 判断是不是用户
        pendPlayer == user ||           // 判断当前用户玩家是不是上次出牌的(仍是最大)
        pendPlayer == nullptr)
        return;

    // 打出一个空的Cards
    Cards empty;
    user->playHand(empty);
    notifyOtherPlayHand(empty);
    // 清空用户选择的牌(可能选了没打出)
    for (auto it: selectCards_) {
        it->setIsSelected(false);
    }
    selectCards_.clear();
    // 更新玩家手牌区域的牌
    updatePlayerCards(user);
}

void GamePanel::disposCard(Player *player, const Cards &cards)
{
    CardList list = cards.toCardList();
    for (int i=0; i<list.size(); i++) {
        CardPanel *panel = cardMap_[list.at(i)];
        panel->setOwner(player);
    }
    // 更新卡牌在窗口的显示
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player *player)
{
    CardList list = player->cards().toCardList();
    cardsRect_ = QRect();
    userCards_.clear();
    QRect cardRect = contextMap_[player].cardRect;
    int cardSpace = 20;         // 每张卡只错开20
    for (int i=0; i<list.size(); i++) {
        CardPanel *panel = cardMap_[list[i]];
        panel->show();
        panel->raise();         // 将图层升至最上
        panel->setFrontSide(contextMap_[player].isFrontSide);

        // 水平 or 垂直
        if (contextMap_[player].align == CardAlign::Horizontal) {
            int leftX = cardRect.left() + (cardRect.width() - cardSpace * (list.size()-1) - panel->width()) / 2;
            int topY = cardRect.top() + (cardRect.height() - panel->height()) / 2;
            if (panel->isSelected())
                topY -= 10;
            panel->move(leftX + cardSpace * i, topY);

            // 有关鼠标框选卡牌的
            cardsRect_ = QRect(leftX, topY, cardSpace * i + cardSize_.width(), cardSize_.height());
            int curWidth = i == list.size()-1 ? cardSize_.width() : cardSpace;
            QRect temp(leftX + cardSpace * i, topY, curWidth, cardSize_.height());
            userCards_.insert(panel, temp);
        }
        else {
            int leftX = cardRect.left() + (cardRect.width() - panel->width()) / 2;
            int topY = cardRect.top() + (cardRect.height() - cardSpace * (list.size()-1) - panel->height()) / 2;
            panel->move(leftX, topY + cardSpace * i);
        }
    }

    // 出牌区的显示
    QRect playHandRect = contextMap_[player].playHandRect;
    Cards lastCards = contextMap_[player].lastCards;
    if (!lastCards.isEmpty()) {
        int playSpace = 24;
        CardList list = lastCards.toCardList();
        for (int i = 0; i < list.size(); i++) {
            const auto& iter = list[i];
            CardPanel *panel = cardMap_[iter];
            panel->setFrontSide(true);
            panel->raise();
            if (contextMap_[player].align == CardAlign::Horizontal) {
                int leftBase = playHandRect.left() +
                               (playHandRect.width() - playSpace * (list.size() - 1) - panel->width()) / 2;
                int top = playHandRect.top() + (playHandRect.height() - panel->height()) / 2;
                panel->move(leftBase + i * playSpace, top);
            }
            else {
                int left = playHandRect.left() + (playHandRect.width() - panel->width()) / 2;
                int topBase = playHandRect.top();
                panel->move(left, i * playSpace +topBase);
            }
            panel->show();
        }
    }
}

void GamePanel::showAnimation(AnimationType type, int bet)
{
    switch (type) {
    case ShunZi:
    case LianDui: {
        animation_->setFixedSize(250, 150);
        animation_->move((width()-animation_->width())/2, 200);
        AnimationWindow::Type shunZiOrLianDui = (type == ShunZi ? AnimationWindow::Sequence : AnimationWindow::Pair);
        animation_->showSequence(shunZiOrLianDui);
        break;
    }
    case Plane: {
        animation_->setFixedSize(800, 150);
        animation_->move((width()-animation_->width())/2, 200);
        animation_->showPlane();
        break;
    }
    case JokerBomb: {
        animation_->setFixedSize(250, 200);
        animation_->move((width()-animation_->width())/2, (height()-animation_->height())/2 - 70);
        animation_->showJokerBomb();
        break;
    }
    case Bomb: {
        animation_->setFixedSize(180, 200);
        animation_->move((width()-animation_->width())/2, (height()-animation_->height())/2 - 70);
        animation_->showBomb();
        break;
    }
    case Bet: {
        animation_->setFixedSize(160, 98);
        animation_->move((width()-animation_->width())/2, (height()-animation_->height())/2 - 140);
        animation_->showBetScore(bet);
        break;
    }
    default:
        break;
    }
    animation_->show();
}

void GamePanel::hidePlayerDropCard(Player *player)
{
    auto it = contextMap_.find(player);
    if (it != contextMap_.end()) {
        if (it->lastCards.isEmpty()) {      // 为空：上次没出牌，info里显示"不要"
            it->info->hide();               // 隐藏上次的信息
        }
        else {
            CardList list = it->lastCards.toCardList();
            for (const auto &iter : list) {
                cardMap_[iter]->hide();
            }
        }
        it->lastCards.clear();
    }
}

QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role)
{
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> farmerMan;
    QVector<QString> farmerWoman;
    lordMan << ":/image/images/lord_man_1.png" << ":/image/images/lord_man_2.png";
    lordWoman << ":/image/images/lord_woman_1.png" << ":/image/images/lord_woman_2.png";
    farmerMan << ":/image/images/farmer_man_1.png" << ":/image/images/farmer_man_2.png";
    farmerWoman << ":/image/images/farmer_woman_1.png" << ":/image/images/farmer_woman_2.png";

    QImage image;
    int random = QRandomGenerator::global()->bounded(2);    // 生成 0、1
    if (sex == Player::Man && role == Player::Lord) {
        image.load(lordMan[random]);
    }
    else if (sex == Player::Woman && role == Player::Lord) {
        image.load(lordWoman[random]);
    }
    else if (sex == Player::Man && role == Player::Farmer) {
        image.load(farmerMan[random]);
    }
    else if (sex == Player::Woman && role == Player::Farmer) {
        image.load(farmerWoman[random]);
    }

    QPixmap pixmap;
    if (direct == Player::Left) {
        pixmap = QPixmap::fromImage(image);
    }
    else {
        pixmap = QPixmap::fromImage(image.mirrored(true, false));
    }
    return pixmap;
}

void GamePanel::showEndingScorePanel()
{
    bool isLord = gameCtl_->user()->role() == Player::Lord;
    bool isWin = gameCtl_->user()->isWin();
    EndingPanel *panel = new EndingPanel(isLord, isWin, this);
    panel->show();
    panel->move((width()-panel->width())/2, -panel->height());      // 将ending窗口移出主窗口
    panel->setPlayerScore(gameCtl_->robotLeft()->score(),
                          gameCtl_->robotRight()->score(),
                          gameCtl_->user()->score());
    panel->setPlayerName(nameList_);

    if (isWin)
        bgm_->playEndingMusic(true);
    else
        bgm_->playEndingMusic(false);

    // 属性geometry -> (x,y,width,height) -> QRect -> setStartValue的QVariant参数用QRect
    QPropertyAnimation *animation = new QPropertyAnimation(panel, "geometry", this);
    animation->setDuration(1500);
    animation->setStartValue(QRect(panel->x(), panel->y(),                                      // 设置起始点
                                   panel->width(), panel->height()));
    animation->setEndValue(QRect((width()-panel->width())/2, (height()-panel->height())/2,      // 设置终点
                                 panel->width(), panel->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);                                         // 设置运动路线
    animation->start();                                                                         // 开始播放动画

    connect(panel, &EndingPanel::continueGame, this, [=](){         // 点击继续游戏的按钮
        panel->close();
        panel->deleteLater();
        animation->deleteLater();
        ui->btnGroup->selectPanel(ButtonGroup::Empty, 0);
        if (DataManager::getInstance()->mode() == DataManager::Network) {
            // 自动加入房间的处理
            if (DataManager::getInstance()->roomMode() == DataManager::Auto) {
                Message msg;
                msg.userName = DataManager::getInstance()->userName();
                msg.requestCode = RequestCode::AutoRoom;
                DataManager::getInstance()->communication()->sendMessage(&msg, true);
            } else {        // 手动加入房间的处理
                Message msg;
                msg.userName = DataManager::getInstance()->userName();
                msg.roomName = DataManager::getInstance()->roomName();
                msg.requestCode = RequestCode::Continue;
                DataManager::getInstance()->communication()->sendMessage(&msg, true);
            }
        } else {
            gameStatusPrecess(GameControl::DispatchCard);
            bgm_->startBGM(80);     // 播放背景音乐
        }
    });
}

void GamePanel::initGamePanel(QByteArray msg)
{
    // user-index-score#user-index-score#user-index-score#
    orderMap order;    // index - <user, score>
    int index = 1;
    auto lst = msg.left(msg.size() - 1).split('#');
    for (const auto& iter : lst) {
        auto sub = iter.split('-');
        order.insert(sub[1].toInt(), QPair(sub[0], sub[2].toInt()));
        if (sub[0] == DataManager::getInstance()->userName())
            index = sub[1].toInt();
    }
    updatePlayerInfo(order);
    startGame(index);
}

void GamePanel::updatePlayerInfo(orderMap &info)
{
    int lscore=0, rscore=0, mscore=0;
    QByteArray left, right, mid;
    QByteArray current = DataManager::getInstance()->userName();
    if (current == info.value(1).first) {           // 当前玩家对应的序号为1
        mid    = info.value(1).first;
        right  = info.value(2).first;
        left   = info.value(3).first;
        mscore = info.value(1).second;
        rscore = info.value(2).second;
        lscore = info.value(3).second;
    } else if (current == info.value(2).first) {    // 当前玩家对应的序号为2
        mid    = info.value(2).first;
        right  = info.value(3).first;
        left   = info.value(1).first;
        mscore = info.value(2).second;
        rscore = info.value(3).second;
        lscore = info.value(1).second;
    } else if (current == info.value(3).first) {    // 当前玩家对应的序号为3
        mid    = info.value(3).first;
        right  = info.value(1).first;
        left   = info.value(2).first;
        mscore = info.value(3).second;
        rscore = info.value(1).second;
        lscore = info.value(2).second;
    }

    ui->scorePanel->setPlayerName(left, mid, right);    // 设置分数面板的玩家名
    gameCtl_->robotLeft()->setScore(lscore);
    gameCtl_->robotRight()->setScore(rscore);
    gameCtl_->user()->setScore(mscore);
    gameCtl_->robotLeft()->setName(left);
    gameCtl_->robotRight()->setName(right);
    gameCtl_->user()->setName(mid);

    nameList_.clear();
    nameList_ << left << mid << right;
}

void GamePanel::startGame()
{
    int index = QRandomGenerator::global()->bounded(1,4);
    startGame(index);
}

void GamePanel::startGame(int index)
{
    gameCtl_->setCurrentPlayer(index);
    ui->btnGroup->selectPanel(ButtonGroup::Empty, 0);      // 发牌时隐藏按钮组
    // gameCtl_->clearPlayerScore();                       // 开始游戏后清除分数
    updatePlayerScore();
    gameStatusPrecess(GameControl::DispatchCard);       // 将游戏状态改为发牌，并进行相应处理
    bgm_->startBGM(80);
}

void GamePanel::notifyOtherPlayHand(Cards &cs)
{
    DataManager *instance = DataManager::getInstance();
    if (instance->mode() != DataManager::Network)
        return;

    // 准备数据
    Message msg;
    msg.requestCode = RequestCode::PlayAHand;
    msg.userName = instance->userName();
    msg.roomName = instance->roomName();
    msg.data1 = QByteArray::number(cs.cardCount());
    QDataStream stream(&msg.data2, QIODevice::ReadWrite);
    // 将cs转化为容器，方便遍历，data2为出牌信息
    CardList list = cs.toCardList();
    for (const auto& iter: list) {
        stream << iter;
    }

    // 发送数据给服务器
    instance->communication()->sendMessage(&msg, true);
}

void GamePanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), bkImage_);
}

void GamePanel::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint point = event->pos();
        if (!cardsRect_.contains(point)) {
            curSelPanel_ = nullptr;
        }
        else {
            auto list = userCards_.keys();              // 取出所有用户拥有的卡牌窗口
            for (int i = 0; i < list.size(); i++) {     // 并逐一检测鼠标是否划过窗口
                CardPanel * panel = list[i];
                if (userCards_[panel].contains(point) && curSelPanel_ != panel) {    // 划过窗口则选中
                    panel->clicked();
                    curSelPanel_ = panel;
                }
            }
        }
    }
}


void GamePanel::closeEvent(QCloseEvent *event)
{
    // 一般情况调用父类事件处理函数(QWidget::event)更方便
    // 但只有closeEvent必须要用accept/ignore
    emit panelClose();
    if (DataManager::getInstance()->mode() == DataManager::Network) {
        Message msg;
        msg.requestCode = RequestCode::LeaveRoom;
        msg.userName = DataManager::getInstance()->userName();
        msg.roomName = DataManager::getInstance()->roomName();
        DataManager::getInstance()->communication()->sendMessage(&msg, true);
    }
    event->accept();    // accept之后Qt会默认隐藏窗口以减少bug
    deleteLater();      // 立即关闭
}
