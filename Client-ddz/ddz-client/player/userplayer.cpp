#include "userplayer.h"

UserPlayer::UserPlayer(QObject *parent)
    : Player{parent}
{
    type_ = Player::User;
}

void UserPlayer::prepareCallLord()
{
}

void UserPlayer::preparePlayHand()
{
    emit startCountDown();
}
