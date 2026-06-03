#include "TitleScene.h"

#include "../../Engine/Core/GameApp.h"
#include "../../Engine/Render/D2DRenderer.h"
#include "../StoneGachaGame.h"

#include <cmath>

TitleScene::TitleScene(GameApp& app, StoneGachaGame& game)
    : Scene(app)
    , m_game(game)
{
}

void TitleScene::OnEnter()
{
    m_game.SetStatusMessage(L"시작 버튼을 눌러 돌 파밍을 시작하세요.");
}

void TitleScene::Update(float deltaTime)
{
    // 누적 시간에 sin을 적용하면 값이 -1~1 사이로 반복되어 부유 애니메이션을 만들 수 있습니다.
    m_elapsedTime += deltaTime;
}

void TitleScene::Render(D2DRenderer& renderer)
{
    const D2D1_SIZE_F size = renderer.GetSize();
    renderer.Clear(D2D1::ColorF(0.14f, 0.19f, 0.24f));

    const float floatingY = 92.0f + std::sin(m_elapsedTime * 2.0f) * 12.0f;
    const D2D1_RECT_F titleRect = D2D1::RectF(40.0f, floatingY, size.width - 40.0f, floatingY + 120.0f);

    renderer.DrawTextBlock(L"돌 파밍으로 돈을 모아\n가챠로 성공", titleRect, TextStyle::Title, D2D1::ColorF(0.98f, 0.88f, 0.35f));
    renderer.DrawTextBlock(
        L"클릭해서 돈을 벌고, 상점에서 돌 가챠를 뽑으세요.",
        D2D1::RectF(40.0f, floatingY + 118.0f, size.width - 40.0f, floatingY + 162.0f),
        TextStyle::SmallCenter,
        D2D1::ColorF(0.88f, 0.93f, 0.95f));

    DrawDecorativePebbles(renderer, size.width, size.height);
    DrawButton(renderer, MakeStartButton(size.width, size.height));

    renderer.DrawTextBlock(
        m_game.GetStatusMessage(),
        D2D1::RectF(40.0f, size.height - 88.0f, size.width - 40.0f, size.height - 34.0f),
        TextStyle::SmallCenter,
        D2D1::ColorF(0.90f, 0.92f, 0.94f));
}

void TitleScene::OnMouseClick(float x, float y)
{
    const D2D1_SIZE_F size = App().GetRenderer().GetSize();

    if (MakeStartButton(size.width, size.height).Contains(x, y))
    {
        App().ChangeScene(StoneGachaScenes::Play);
    }
}

ButtonObject TitleScene::MakeStartButton(float width, float height) const
{
    return ButtonObject(
        D2D1::RectF(width * 0.5f - 110.0f, height * 0.62f, width * 0.5f + 110.0f, height * 0.62f + 58.0f),
        L"시작");
}

void TitleScene::DrawButton(D2DRenderer& renderer, const ButtonObject& button) const
{
    renderer.DrawRoundedPanel(button.GetBounds(), D2D1::ColorF(0.95f, 0.73f, 0.25f), D2D1::ColorF(0.18f, 0.12f, 0.04f));
    renderer.DrawTextBlock(button.GetText(), button.GetBounds(), TextStyle::Button, D2D1::ColorF(0.08f, 0.10f, 0.12f));
}

void TitleScene::DrawDecorativePebbles(D2DRenderer& renderer, float width, float height) const
{
    renderer.FillEllipse(D2D1::Point2F(width * 0.18f, height * 0.78f), 46.0f, 30.0f, D2D1::ColorF(0.42f, 0.49f, 0.52f));
    renderer.FillEllipse(D2D1::Point2F(width * 0.79f, height * 0.75f), 58.0f, 34.0f, D2D1::ColorF(0.58f, 0.64f, 0.66f));
    renderer.FillEllipse(D2D1::Point2F(width * 0.68f, height * 0.86f), 30.0f, 20.0f, D2D1::ColorF(0.35f, 0.40f, 0.42f));
}
