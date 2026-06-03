#include "PlayScene.h"

#include "../../Engine/Core/GameApp.h"
#include "../../Engine/Render/D2DRenderer.h"
#include "../StoneGachaGame.h"

PlayScene::PlayScene(GameApp& app, StoneGachaGame& game)
    : Scene(app)
    , m_game(game)
{
}

void PlayScene::OnEnter()
{
    m_game.SetStatusMessage(L"화면을 클릭해서 돈을 모으세요.");
}

void PlayScene::Update(float deltaTime)
{
    if (m_clickPopTime > 0.0f)
    {
        m_clickPopTime -= deltaTime;
        if (m_clickPopTime < 0.0f)
        {
            m_clickPopTime = 0.0f;
        }
    }
}

void PlayScene::Render(D2DRenderer& renderer)
{
    const D2D1_SIZE_F size = renderer.GetSize();
    const PlayerData& player = m_game.GetPlayerData();

    renderer.Clear(D2D1::ColorF(0.70f, 0.86f, 0.72f));

    DrawMoneyUi(renderer, size.width);
    DrawButton(renderer, MakeShopButton(size.height));

    renderer.DrawTextBlock(
        L"클릭 수익: +" + std::to_wstring(player.GetFinalClickValue()) + L"원",
        D2D1::RectF(28.0f, 24.0f, 290.0f, 70.0f),
        TextStyle::Left,
        D2D1::ColorF(0.08f, 0.25f, 0.12f));

    DrawRock(renderer, size.width * 0.5f, size.height * 0.52f);

    renderer.DrawTextBlock(
        L"화면 아무 곳이나 클릭하면 돈이 증가합니다.",
        D2D1::RectF(30.0f, size.height - 96.0f, size.width - 30.0f, size.height - 58.0f),
        TextStyle::SmallCenter,
        D2D1::ColorF(0.08f, 0.18f, 0.10f));
    renderer.DrawTextBlock(
        m_game.GetStatusMessage(),
        D2D1::RectF(30.0f, size.height - 58.0f, size.width - 30.0f, size.height - 22.0f),
        TextStyle::SmallCenter,
        D2D1::ColorF(0.10f, 0.13f, 0.15f));
}

void PlayScene::OnMouseClick(float x, float y)
{
    const D2D1_SIZE_F size = App().GetRenderer().GetSize();

    // 컨텐츠 규칙: 상점 버튼이 화면 전체 파밍 클릭보다 우선입니다.
    if (MakeShopButton(size.height).Contains(x, y))
    {
        App().ChangeScene(StoneGachaScenes::Shop);
        return;
    }

    PlayerData& player = m_game.GetPlayerData();
    const int earnedMoney = player.GetFinalClickValue();

    player.money += earnedMoney;
    m_clickPopTime = 0.14f;
    m_game.SetStatusMessage(L"+" + std::to_wstring(earnedMoney) + L"원 획득!");
}

ButtonObject PlayScene::MakeShopButton(float height) const
{
    return ButtonObject(D2D1::RectF(28.0f, height * 0.5f - 32.0f, 158.0f, height * 0.5f + 32.0f), L"상점");
}

void PlayScene::DrawButton(D2DRenderer& renderer, const ButtonObject& button) const
{
    renderer.DrawRoundedPanel(button.GetBounds(), D2D1::ColorF(0.55f, 0.78f, 0.96f), D2D1::ColorF(0.08f, 0.28f, 0.46f));
    renderer.DrawTextBlock(button.GetText(), button.GetBounds(), TextStyle::Button, D2D1::ColorF(0.08f, 0.10f, 0.12f));
}

void PlayScene::DrawMoneyUi(D2DRenderer& renderer, float width) const
{
    const PlayerData& player = m_game.GetPlayerData();
    const float left = width > 305.0f ? width - 280.0f : 24.0f;
    const D2D1_RECT_F moneyPanel = D2D1::RectF(left, 22.0f, width - 24.0f, 72.0f);

    renderer.DrawRoundedPanel(moneyPanel, D2D1::ColorF(0.98f, 0.96f, 0.88f), D2D1::ColorF(0.65f, 0.55f, 0.25f));
    renderer.DrawTextBlock(
        L"보유 돈: " + std::to_wstring(player.money) + L"원",
        D2D1::RectF(moneyPanel.left + 12.0f, moneyPanel.top, moneyPanel.right - 12.0f, moneyPanel.bottom),
        TextStyle::Right,
        D2D1::ColorF(0.14f, 0.12f, 0.08f));
}

void PlayScene::DrawRock(D2DRenderer& renderer, float centerX, float centerY) const
{
    // 클릭 직후에는 잠깐 커지게 해서 플레이어에게 즉각적인 피드백을 줍니다.
    const float popScale = 1.0f + (m_clickPopTime / 0.14f) * 0.10f;
    const float radiusX = 125.0f * popScale;
    const float radiusY = 92.0f * popScale;

    renderer.FillEllipse(D2D1::Point2F(centerX, centerY), radiusX, radiusY, D2D1::ColorF(0.47f, 0.52f, 0.54f));
    renderer.DrawEllipse(D2D1::Point2F(centerX, centerY), radiusX, radiusY, D2D1::ColorF(0.20f, 0.24f, 0.26f), 4.0f);

    renderer.DrawLine(D2D1::Point2F(centerX - 42.0f, centerY - 28.0f), D2D1::Point2F(centerX - 10.0f, centerY + 12.0f), D2D1::ColorF(0.20f, 0.24f, 0.26f), 3.0f);
    renderer.DrawLine(D2D1::Point2F(centerX - 10.0f, centerY + 12.0f), D2D1::Point2F(centerX + 34.0f, centerY - 8.0f), D2D1::ColorF(0.20f, 0.24f, 0.26f), 3.0f);
    renderer.DrawLine(D2D1::Point2F(centerX + 12.0f, centerY + 26.0f), D2D1::Point2F(centerX + 58.0f, centerY + 42.0f), D2D1::ColorF(0.20f, 0.24f, 0.26f), 2.0f);

    renderer.DrawTextBlock(
        L"돌",
        D2D1::RectF(centerX - 70.0f, centerY - 38.0f, centerX + 70.0f, centerY + 38.0f),
        TextStyle::Title,
        D2D1::ColorF(0.93f, 0.94f, 0.92f));
}
