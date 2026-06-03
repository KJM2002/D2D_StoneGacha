#pragma once

#include <string>

// Content/Data
// 이 파일의 데이터는 "돌 파밍 가챠 게임" 전용입니다.
// 엔진 폴더에서 이 파일을 include하면 엔진과 컨텐츠가 다시 섞이므로 주의합니다.
struct PlayerData
{
    int money = 0;
    int baseClickValue = 1;
    int itemBonusClickValue = 0;
    int upgradeBonusClickValue = 0;
    int gachaCost = 10;

    int GetFinalClickValue() const
    {
        return baseClickValue + itemBonusClickValue + upgradeBonusClickValue;
    }
};

enum class ItemGrade
{
    Common,
    Rare,
    Epic,
    Legendary
};

struct GachaItem
{
    int itemId = 0;
    std::wstring itemName;
    ItemGrade grade = ItemGrade::Common;
    float probability = 0.0f;
    int clickPowerBonus = 0;
};

inline std::wstring GradeToText(ItemGrade grade)
{
    switch (grade)
    {
    case ItemGrade::Common:
        return L"일반";
    case ItemGrade::Rare:
        return L"희귀";
    case ItemGrade::Epic:
        return L"영웅";
    case ItemGrade::Legendary:
        return L"전설";
    default:
        return L"알 수 없음";
    }
}
