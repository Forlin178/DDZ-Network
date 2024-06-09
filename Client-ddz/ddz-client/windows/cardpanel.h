#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "card.h"
#include "player.h"

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    void clicked();

    // set/get图片
    void setImage(QPixmap &front, QPixmap &back);
    QPixmap getImage();

    void setFrontSide(bool flag) { isFront_ = flag; }
    bool isFrontSide() { return isFront_; }

    bool isSelected() const { return isSelected_; }
    void setIsSelected(bool newIsSelected) { isSelected_=newIsSelected; }

    Card getCard() const;
    void setCard(const Card &newCard);

    Player* getOwner() { return owner_; }
    void setOwner(Player* owner) { owner_ = owner; }

signals:
    void cardSelected(Qt::MouseButton button);

private:
    QPixmap front_;         // 卡片正面的贴图
    QPixmap back_;          // 卡片背面的贴图
    bool isFront_;          // 该卡应该展示正/背面
    bool isSelected_;       // 卡片选中状态
    Card card_;             // 点数 花色
    Player* owner_;         // 卡牌所有者

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // CARDPANEL_H
