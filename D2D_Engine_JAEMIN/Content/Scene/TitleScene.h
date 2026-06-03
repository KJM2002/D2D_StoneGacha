#pragma once

#include "../../Engine/Object/GameObject.h"
#include "../../Engine/Scene/Scene.h"

class StoneGachaGame;

class TitleScene : public Scene
{
public:
    TitleScene(GameApp& app, StoneGachaGame& game);

    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(D2DRenderer& renderer) override;
    void OnMouseClick(float x, float y) override;

private:
    ButtonObject MakeStartButton(float width, float height) const;
    void DrawButton(D2DRenderer& renderer, const ButtonObject& button) const;
    void DrawDecorativePebbles(D2DRenderer& renderer, float width, float height) const;

private:
    StoneGachaGame& m_game;
    float m_elapsedTime = 0.0f;
};
