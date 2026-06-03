#pragma once

#include "../../Engine/Object/GameObject.h"
#include "../../Engine/Scene/Scene.h"

class StoneGachaGame;

class PlayScene : public Scene
{
public:
    PlayScene(GameApp& app, StoneGachaGame& game);

    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(D2DRenderer& renderer) override;
    void OnMouseClick(float x, float y) override;

private:
    ButtonObject MakeShopButton(float height) const;
    void DrawButton(D2DRenderer& renderer, const ButtonObject& button) const;
    void DrawMoneyUi(D2DRenderer& renderer, float width) const;
    void DrawRock(D2DRenderer& renderer, float centerX, float centerY) const;

private:
    StoneGachaGame& m_game;
    float m_clickPopTime = 0.0f;
};
