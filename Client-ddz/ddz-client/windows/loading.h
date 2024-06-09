#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading : public QWidget
{
    Q_OBJECT
public:
    explicit Loading(QWidget *parent = nullptr);

signals:

private:
    QPixmap backGround_;
    QPixmap progress_;
    int length_{ 0 };

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // LOADING_H
