#pragma once

#include "../Data/GameData.h"

#include <random>
#include <vector>

// Content/System/GachaSystem
// 가챠 확률 데이터와 랜덤 추첨은 돌 가챠 게임 전용 규칙이므로 컨텐츠 시스템으로 분리합니다.
class GachaSystem
{
public:
    void LoadDefaultData();
    GachaItem PickItem();
    const std::vector<GachaItem>& GetItems() const;

private:
    std::vector<GachaItem> m_items;
    std::mt19937 m_randomEngine{ std::random_device{}() };
};
