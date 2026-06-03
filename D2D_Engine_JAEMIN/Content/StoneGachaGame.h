#pragma once

#include "Data/GameData.h"
#include "System/GachaSystem.h"

#include <string>
#include <vector>

class GameApp;

namespace StoneGachaScenes
{
    constexpr const wchar_t* Title = L"StoneGacha.Title";
    constexpr const wchar_t* Play = L"StoneGacha.Play";
    constexpr const wchar_t* Shop = L"StoneGacha.Shop";
}

// Content/StoneGachaGame
// 돌 파밍 가챠 게임의 컨텐츠 루트입니다.
// 엔진 GameApp에 씬을 등록하고, 이 게임만의 데이터와 규칙을 보관합니다.
class StoneGachaGame
{
public:
    void Initialize(GameApp& app);

    PlayerData& GetPlayerData();
    const PlayerData& GetPlayerData() const;

    GachaItem PickGachaItem();
    const std::vector<GachaItem>& GetGachaItems() const;

    void SetStatusMessage(const std::wstring& message);
    const std::wstring& GetStatusMessage() const;

private:
    PlayerData m_playerData;
    GachaSystem m_gachaSystem;
    std::wstring m_statusMessage;
};
