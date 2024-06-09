#include "playhand.h"

PlayHand::PlayHand() {}

PlayHand::PlayHand(const Cards &cards)
{
    // 1.对扑克牌的分类：1张的、2张、3张、4张各有多少种
    classify(cards);
    // 2.对牌型进行分类
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint pt, int extra)
{
    type_ = type;
    pt_ = pt;
    extra_ = extra;
}

bool PlayHand::canBeat(const PlayHand &other)
{
    // 我的牌型是未知的
    if(type_ == Hand_Unknown)
    {
        return false;
    }
    // 对方放弃出牌
    if(other.type_ == Hand_Pass)
    {
        return true;
    }
    // 我是王炸
    if(type_ == Hand_Bomb_Jokers)
    {
        return true;
    }
    if(type_ == Hand_Bomb && other.type_ >= Hand_Single && other.type_ <= Hand_Seq_Single)
    {
        return true;
    }
    // 双方的牌型一致
    if(type_ == other.type_)
    {
        if(type_ == Hand_Seq_Pair || type_ == Hand_Seq_Single)
        {
            return pt_ > other.pt_ && extra_ == other.extra_;
        }
        else
        {
            return pt_ > other.pt_;
        }
    }
    return false;
}

void PlayHand::classify(const Cards &cards)
{
    CardList list = cards.toCardList();
    int cardRecord[Card::Point_end];
    memset(cardRecord, 0, sizeof(int) * Card::Point_end);

    for (int i=0; i<list.size(); i++) {     // 统计每个点数的出现次数
        Card card = list[i];
        cardRecord[card.point()]++;
    }

    oneCard_.clear();
    twoCard_.clear();
    threeCard_.clear();
    fourCard_.clear();

    for (int i=0; i<Card::Point_end; i++) {
        if (cardRecord[i] == 1)
            oneCard_ << (Card::CardPoint)i;
        else if (cardRecord[i] == 2)
            twoCard_ << (Card::CardPoint)i;
        else if (cardRecord[i] == 3)
            threeCard_ << (Card::CardPoint)i;
        else if (cardRecord[i] == 4)
            fourCard_ << (Card::CardPoint)i;
    }
}

void PlayHand::judgeCardType()
{
    type_ = Hand_Unknown;
    pt_ = Card::Point_begin;
    extra_ = 0;

    if(isPass())
    {
        type_ = Hand_Pass;
    }
    if(isSingle())
    {
        type_ = Hand_Single;
        pt_ = oneCard_[0];
    }
    else if(isPair())
    {
        type_ = Hand_Pair;
        pt_ = twoCard_[0];
    }
    else if(isTriple())
    {
        type_ = Hand_Triple;
        pt_ = threeCard_[0];
    }
    else if(isTripleSingle())
    {
        type_ = Hand_Triple_Single;
        pt_ = threeCard_[0];
    }
    else if(isTriplePair())
    {
        type_ = Hand_Triple_Pair;
        pt_ = threeCard_[0];
    }
    else if(isPlane())
    {
        type_ = Hand_Plane;
        // 记录点数最小的牌
        pt_ = threeCard_[0];
    }
    else if(isPlaneTwoSingle())
    {
        type_ = Hand_Plane_Two_Single;
        // 记录点数最小的牌
        pt_ = threeCard_[0];
    }
    else if(isPlaneTwoPair())
    {
        type_ = Hand_Plane_Two_Pair;
        // 记录点数最小的牌
        pt_ = threeCard_[0];
    }
    else if(isSeqPair())
    {
        type_ = Hand_Seq_Pair;
        // 记录点数最小的牌
        pt_ = twoCard_[0];
        extra_ = twoCard_.size();
    }
    else if(isSeqSingle())
    {
        type_ = Hand_Seq_Single;
        // 记录点数最小的牌
        pt_ = oneCard_[0];
        extra_ = oneCard_.size();
    }
    else if(isBomb())
    {
        type_ = Hand_Bomb;
        // 记录点数最小的牌
        pt_ = fourCard_[0];
    }
    else if(isBombSingle())
    {
        type_ = Hand_Bomb_Single;
        // 记录点数最小的牌
        pt_ = fourCard_[0];
    }
    else if(isBombPair())
    {
        type_ = Hand_Bomb_Pair;
        // 记录点数最小的牌
        pt_ = fourCard_[0];
    }
    else if(isBombTwoSingle())
    {
        type_ = Hand_Bomb_Two_Single;
        // 记录点数最小的牌
        pt_ = fourCard_[0];
    }
    else if(isBombJokers())
    {
        type_ = Hand_Bomb_Jokers;
    }
    else if(isBombJokersSingle())
    {
        type_ = Hand_Bomb_Jokers_Single;
    }
    else if(isBombJokersPair())
    {
        type_ = Hand_Bomb_Jokers_Pair;
    }
    else if(isBombJokersTwoSingle())
    {
        type_ = Hand_Bomb_Jokers_Two_Single;
    }
}

bool PlayHand::isPass()
{
    if (oneCard_.empty() && twoCard_.empty() && threeCard_.empty() && fourCard_.empty())
        return true;
    return false;
}

bool PlayHand::isSingle()
{
    if (oneCard_.size() == 1 && twoCard_.empty() && threeCard_.empty() && fourCard_.empty())
        return true;
    return false;
}

bool PlayHand::isPair()
{
    if (oneCard_.empty() && twoCard_.size() == 1 && threeCard_.empty() && fourCard_.empty())
        return true;
    return false;
}

bool PlayHand::isTriple()
{
    if (oneCard_.empty() && twoCard_.empty() && threeCard_.size() == 1 && fourCard_.empty())
        return true;
    return false;
}

bool PlayHand::isTripleSingle()
{
    if (oneCard_.size() == 1 && twoCard_.empty() && threeCard_.size() == 1 && fourCard_.empty())
        return true;
    return false;
}

bool PlayHand::isTriplePair()
{
    if (oneCard_.empty() && twoCard_.size() == 1 && threeCard_.size() == 1 && fourCard_.empty())
        return true;
    return false;
}

bool PlayHand::isPlane()
{
    if (oneCard_.empty() && twoCard_.empty() && threeCard_.size() == 2 && fourCard_.empty()) {
        std::sort(threeCard_.begin(), threeCard_.end());
        if (threeCard_[1] - threeCard_[0] == 1 && threeCard_[1] < Card::Card_2)     // AAA 222不是飞机
            return true;
    }
    return false;
}

bool PlayHand::isPlaneTwoSingle()
{
    if (oneCard_.size() == 2 && twoCard_.empty() && threeCard_.size() == 2 && fourCard_.empty()) {
        std::sort(threeCard_.begin(), threeCard_.end());
        std::sort(oneCard_.begin(), oneCard_.end());
        if (threeCard_[1] - threeCard_[0] == 1 && threeCard_[1] < Card::Card_2 &&
            oneCard_[0] != Card::Card_SJ && oneCard_[1] != Card::Card_BJ)
            return true;
    }
    return false;
}

bool PlayHand::isPlaneTwoPair()
{
    if (oneCard_.empty() && twoCard_.size() == 2 && threeCard_.size() == 2 && fourCard_.empty()) {
        std::sort(threeCard_.begin(), threeCard_.end());
        if (threeCard_[1] - threeCard_[0] == 1 && threeCard_[1] < Card::Card_2)     // AAA 222不是飞机
            return true;
    }
    return false;
}

bool PlayHand::isSeqPair()
{
    if (oneCard_.empty() && twoCard_.size() >= 3 && threeCard_.empty() && fourCard_.empty()) {
        std::sort(twoCard_.begin(), twoCard_.end());
        if (twoCard_[twoCard_.size()-1] - twoCard_[0] == twoCard_.size() - 1 &&
            twoCard_[twoCard_.size()-1] < Card::Card_2 &&
            twoCard_[0] >= Card::Card_3)
            return true;
    }
    return false;
}

bool PlayHand::isSeqSingle()
{
    if (oneCard_.size() >= 5 && twoCard_.empty() && threeCard_.empty() && fourCard_.empty()) {
        std::sort(oneCard_.begin(), oneCard_.end());
        if (oneCard_[oneCard_.size()-1] - oneCard_[0] == oneCard_.size() - 1 &&
            oneCard_[oneCard_.size()-1] < Card::Card_2 &&
            oneCard_[0] >= Card::Card_3)
            return true;
    }
    return false;
}

bool PlayHand::isBomb()
{
    if (oneCard_.empty() && twoCard_.empty() && threeCard_.empty() && fourCard_.size() == 1)
        return true;
    return false;
}

bool PlayHand::isBombSingle()
{
    if (oneCard_.size() == 1 && twoCard_.empty() && threeCard_.empty() && fourCard_.size() == 1)
        return true;
    return false;
}

bool PlayHand::isBombPair()
{
    if (oneCard_.empty() && twoCard_.size() == 1 && threeCard_.empty() && fourCard_.size() == 1) {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoSingle()
{
    if (oneCard_.size() == 2 && twoCard_.empty() && threeCard_.empty() && fourCard_.size() == 1) {
        std::sort(oneCard_.begin(), oneCard_.end());
        if (oneCard_[0] != Card::Card_SJ && oneCard_[1] != Card::Card_BJ)
            return true;
    }
    return false;
}

bool PlayHand::isBombJokers()
{
    if (oneCard_.size() == 2 && twoCard_.empty() && threeCard_.empty() && fourCard_.empty()) {
        std::sort(oneCard_.begin(), oneCard_.end());
        if (oneCard_[0] == Card::Card_SJ && oneCard_[1] == Card::Card_BJ)
            return true;
    }
    return false;
}

bool PlayHand::isBombJokersSingle()
{
    if (oneCard_.size() == 3 && twoCard_.empty() && threeCard_.empty() && fourCard_.empty()) {
        std::sort(oneCard_.begin(), oneCard_.end());
        if (oneCard_[1] == Card::Card_SJ && oneCard_[2] == Card::Card_BJ)
            return true;
    }
    return false;
}

bool PlayHand::isBombJokersPair()
{
    if (oneCard_.size() == 2 && twoCard_.size() == 1 && threeCard_.empty() && fourCard_.empty()) {
        std::sort(oneCard_.begin(), oneCard_.end());
        if (oneCard_[0] == Card::Card_SJ && oneCard_[1] == Card::Card_BJ)
            return true;
    }
    return false;
}

bool PlayHand::isBombJokersTwoSingle()
{
    if (oneCard_.size() == 4 && twoCard_.empty() && threeCard_.empty() && fourCard_.empty()) {
        std::sort(oneCard_.begin(), oneCard_.end());
        if (oneCard_[2] == Card::Card_SJ && oneCard_[3] == Card::Card_BJ)
            return true;
    }
    return false;
}

PlayHand::HandType PlayHand::type() const
{
    return type_;
}

Card::CardPoint PlayHand::pt() const
{
    return pt_;
}

int PlayHand::extra() const
{
    return extra_;
}
