#pragma once

class D2DRenderer;
class GameApp;

// Engine/Scene/Scene
// 모든 씬이 지켜야 하는 엔진 공통 인터페이스입니다.
// 엔진은 씬 내부가 타이틀인지, 상점인지, 전투인지 알 필요 없이 이 함수들만 호출합니다.
class Scene
{
public:
    explicit Scene(GameApp& app)
        : m_app(app)
    {
    }

    virtual ~Scene() = default;

    virtual void OnEnter() {}
    virtual void Update(float deltaTime) { (void)deltaTime; }
    virtual void Render(D2DRenderer& renderer) = 0;
    virtual void OnMouseClick(float x, float y) = 0;

protected:
    GameApp& App()
    {
        return m_app;
    }

    const GameApp& App() const
    {
        return m_app;
    }

private:
    GameApp& m_app;
};
