#include "StoneGachaGame.h"

#include "../Engine/Core/GameApp.h"
#include "Scene/PlayScene.h"
#include "Scene/ShopScene.h"
#include "Scene/TitleScene.h"

#include <memory>

void StoneGachaGame::Initialize(GameApp& app)
{
    m_gachaSystem.LoadDefaultData();

    // 컨텐츠가 자기 씬을 엔진에 등록합니다.
    // 이 덕분에 엔진 GameApp은 TitleScene/PlayScene/ShopScene 클래스를 전혀 알지 않아도 됩니다.
    app.RegisterScene(StoneGachaScenes::Title, [this](GameApp& gameApp)
        {
            return std::make_unique<TitleScene>(gameApp, *this);
        });

    app.RegisterScene(StoneGachaScenes::Play, [this](GameApp& gameApp)
        {
            return std::make_unique<PlayScene>(gameApp, *this);
        });

    app.RegisterScene(StoneGachaScenes::Shop, [this](GameApp& gameApp)
        {
            return std::make_unique<ShopScene>(gameApp, *this);
        });

    app.ChangeScene(StoneGachaScenes::Title);
}

PlayerData& StoneGachaGame::GetPlayerData()
{
    return m_playerData;
}

const PlayerData& StoneGachaGame::GetPlayerData() const
{
    return m_playerData;
}

GachaItem StoneGachaGame::PickGachaItem()
{
    return m_gachaSystem.PickItem();
}

const std::vector<GachaItem>& StoneGachaGame::GetGachaItems() const
{
    return m_gachaSystem.GetItems();
}

void StoneGachaGame::SetStatusMessage(const std::wstring& message)
{
    m_statusMessage = message;
}

const std::wstring& StoneGachaGame::GetStatusMessage() const
{
    return m_statusMessage;
}
