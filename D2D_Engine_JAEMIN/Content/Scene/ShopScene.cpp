#include "ShopScene.h"

#include "../../Engine/Core/GameApp.h"
#include "../../Engine/Render/D2DRenderer.h"
#include "../StoneGachaGame.h"

ShopScene::ShopScene(GameApp& app, StoneGachaGame& game)
    : Scene(app)
    , m_game(game)
{
}

void ShopScene::OnEnter()
{
    m_game.SetStatusMessage(L"상점 주인을 클릭해서 가챠를 해보세요.");
}

void ShopScene::Render(D2DRenderer& renderer)
{
    const D2D1_SIZE_F size = renderer.GetSize();
    const PlayerData& player = m_game.GetPlayerData();

    renderer.Clear(D2D1::ColorF(0.28f, 0.22f, 0.30f));

    DrawMoneyUi(renderer, size.width);
    DrawButton(renderer, MakeBackButton(size.height), D2D1::ColorF(0.90f, 0.90f, 0.88f), D2D1::ColorF(0.26f, 0.26f, 0.26f));
    DrawButton(renderer, MakeProbabilityButton(size.width, size.height), D2D1::ColorF(0.76f, 0.90f, 0.62f), D2D1::ColorF(0.22f, 0.42f, 0.12f));

    DrawShopOwner(renderer, MakeShopOwner(size.width, size.height));

    renderer.DrawTextBlock(
        L"상점 주인을 클릭하면 10원으로 돌 가챠를 진행합니다.",
        D2D1::RectF(30.0f, 78.0f, size.width - 30.0f, 118.0f),
        TextStyle::SmallCenter,
        D2D1::ColorF(0.94f, 0.92f, 0.86f));
    renderer.DrawTextBlock(
        L"클릭 수익: +" + std::to_wstring(player.GetFinalClickValue()) + L"원",
        D2D1::RectF(30.0f, 118.0f, size.width - 30.0f, 156.0f),
        TextStyle::SmallCenter,
        D2D1::ColorF(0.94f, 0.84f, 0.48f));
    renderer.DrawTextBlock(
        m_game.GetStatusMessage(),
        D2D1::RectF(50.0f, size.height - 165.0f, size.width - 50.0f, size.height - 110.0f),
        TextStyle::Left,
        D2D1::ColorF(0.98f, 0.96f, 0.88f));

    if (m_showProbability)
    {
        DrawProbabilityPanel(renderer, size.width, size.height);
    }
}

void ShopScene::OnMouseClick(float x, float y)
{
    const D2D1_SIZE_F size = App().GetRenderer().GetSize();

    if (MakeBackButton(size.height).Contains(x, y))
    {
        App().ChangeScene(StoneGachaScenes::Play);
        return;
    }

    if (MakeProbabilityButton(size.width, size.height).Contains(x, y))
    {
        m_showProbability = !m_showProbability;
        m_game.SetStatusMessage(m_showProbability ? L"가챠 확률 정보를 표시합니다." : L"확률 정보 창을 닫았습니다.");
        return;
    }

    if (MakeShopOwner(size.width, size.height).Contains(x, y))
    {
        RunGacha();
    }
}

ButtonObject ShopScene::MakeBackButton(float height) const
{
    return ButtonObject(D2D1::RectF(28.0f, height - 94.0f, 158.0f, height - 38.0f), L"돌아가기");
}

ButtonObject ShopScene::MakeProbabilityButton(float width, float height) const
{
    return ButtonObject(D2D1::RectF(width - 190.0f, height - 94.0f, width - 28.0f, height - 38.0f), L"확률 정보");
}

GameObject ShopScene::MakeShopOwner(float width, float height) const
{
    return GameObject(D2D1::RectF(width * 0.5f - 135.0f, height * 0.5f - 150.0f, width * 0.5f + 135.0f, height * 0.5f + 105.0f));
}

void ShopScene::DrawButton(D2DRenderer& renderer, const ButtonObject& button, const D2D1_COLOR_F& fill, const D2D1_COLOR_F& stroke) const
{
    renderer.DrawRoundedPanel(button.GetBounds(), fill, stroke);
    renderer.DrawTextBlock(button.GetText(), button.GetBounds(), TextStyle::Button, D2D1::ColorF(0.08f, 0.10f, 0.12f));
}

void ShopScene::DrawMoneyUi(D2DRenderer& renderer, float width) const
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

void ShopScene::DrawShopOwner(D2DRenderer& renderer, const GameObject& owner) const
{
    const D2D1_RECT_F bounds = owner.GetBounds();
    const float centerX = (bounds.left + bounds.right) * 0.5f;
    const float top = bounds.top;

    renderer.DrawRoundedPanel(bounds, D2D1::ColorF(0.96f, 0.82f, 0.54f), D2D1::ColorF(0.20f, 0.12f, 0.08f));

    renderer.FillEllipse(D2D1::Point2F(centerX, top + 76.0f), 54.0f, 54.0f, D2D1::ColorF(0.92f, 0.70f, 0.55f));
    renderer.DrawEllipse(D2D1::Point2F(centerX, top + 76.0f), 54.0f, 54.0f, D2D1::ColorF(0.24f, 0.16f, 0.12f), 3.0f);

    renderer.FillEllipse(D2D1::Point2F(centerX - 18.0f, top + 68.0f), 5.0f, 5.0f, D2D1::ColorF(0.20f, 0.18f, 0.16f));
    renderer.FillEllipse(D2D1::Point2F(centerX + 18.0f, top + 68.0f), 5.0f, 5.0f, D2D1::ColorF(0.20f, 0.18f, 0.16f));
    renderer.DrawLine(D2D1::Point2F(centerX - 18.0f, top + 92.0f), D2D1::Point2F(centerX + 18.0f, top + 92.0f), D2D1::ColorF(0.20f, 0.18f, 0.16f), 3.0f);

    renderer.FillRectangle(D2D1::RectF(centerX - 70.0f, top + 134.0f, centerX + 70.0f, bounds.bottom - 38.0f), D2D1::ColorF(0.22f, 0.38f, 0.57f));
    renderer.DrawTextBlock(
        L"상점 주인",
        D2D1::RectF(bounds.left + 12.0f, bounds.bottom - 48.0f, bounds.right - 12.0f, bounds.bottom - 12.0f),
        TextStyle::Button,
        D2D1::ColorF(0.12f, 0.10f, 0.08f));
}

void ShopScene::DrawProbabilityPanel(D2DRenderer& renderer, float width, float height) const
{
    const D2D1_RECT_F panel = D2D1::RectF(width * 0.5f - 220.0f, height * 0.5f - 120.0f, width * 0.5f + 220.0f, height * 0.5f + 160.0f);
    renderer.DrawRoundedPanel(panel, D2D1::ColorF(0.98f, 0.96f, 0.90f), D2D1::ColorF(0.30f, 0.24f, 0.18f));

    renderer.DrawTextBlock(L"가챠 확률표", D2D1::RectF(panel.left, panel.top + 18.0f, panel.right, panel.top + 58.0f), TextStyle::Button, D2D1::ColorF(0.12f, 0.10f, 0.08f));

    const std::wstring text = L"일반: 60%  |  클릭 수익 +1\n"
        L"희귀: 30%  |  클릭 수익 +3\n"
        L"영웅: 9%   |  클릭 수익 +10\n"
        L"전설: 1%   |  클릭 수익 +100\n\n"
        L"같은 아이템을 여러 번 얻으면 보너스가 누적됩니다.";

    renderer.DrawTextBlock(text, D2D1::RectF(panel.left + 36.0f, panel.top + 74.0f, panel.right - 36.0f, panel.bottom - 26.0f), TextStyle::SmallLeft, D2D1::ColorF(0.10f, 0.12f, 0.14f));
}

void ShopScene::RunGacha()
{
    PlayerData& player = m_game.GetPlayerData();

    if (player.money < player.gachaCost)
    {
        m_game.SetStatusMessage(L"돈이 부족합니다. 가챠 비용은 " + std::to_wstring(player.gachaCost) + L"원입니다.");
        return;
    }

    player.money -= player.gachaCost;
    const GachaItem item = m_game.PickGachaItem();

    // 현재 버전에서는 아이템 효과를 즉시 클릭 수익에 더합니다.
    // 나중에 인벤토리를 만들면 "아이템 추가 -> 보너스 재계산" 구조로 바꾸면 됩니다.
    player.itemBonusClickValue += item.clickPowerBonus;

    m_game.SetStatusMessage(
        L"가챠 결과: [" + GradeToText(item.grade) + L"] " + item.itemName
        + L" 획득! 클릭 수익 +" + std::to_wstring(item.clickPowerBonus) + L"원");
}
