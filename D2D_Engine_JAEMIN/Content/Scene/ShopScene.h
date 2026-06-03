#pragma once

#include "../../Engine/Object/GameObject.h"
#include "../../Engine/Scene/Scene.h"

class StoneGachaGame;

class ShopScene : public Scene
{
public:
    ShopScene(GameApp& app, StoneGachaGame& game);

    void OnEnter() override;
    void Render(D2DRenderer& renderer) override;
    void OnMouseClick(float x, float y) override;

private:
    ButtonObject MakeBackButton(float height) const;
    ButtonObject MakeProbabilityButton(float width, float height) const;
    GameObject MakeShopOwner(float width, float height) const;

    void DrawButton(D2DRenderer& renderer, const ButtonObject& button, const D2D1_COLOR_F& fill, const D2D1_COLOR_F& stroke) const;
    void DrawMoneyUi(D2DRenderer& renderer, float width) const;
    void DrawShopOwner(D2DRenderer& renderer, const GameObject& owner) const;
    void DrawProbabilityPanel(D2DRenderer& renderer, float width, float height) const;
    void RunGacha();

private:
    StoneGachaGame& m_game;
    bool m_showProbability = false;
};
