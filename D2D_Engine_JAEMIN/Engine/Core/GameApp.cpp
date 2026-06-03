#include "GameApp.h"

#include <utility>

HRESULT GameApp::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;
    return m_renderer.Initialize(hWnd);
}

void GameApp::Update(float deltaTime)
{
    if (m_currentScene)
    {
        m_currentScene->Update(deltaTime);
    }
}

void GameApp::Render()
{
    if (!m_renderer.BeginDraw())
    {
        return;
    }

    if (m_currentScene)
    {
        m_currentScene->Render(m_renderer);
    }
    else
    {
        // 등록된 씬이 아직 없을 때도 화면은 깨끗하게 유지합니다.
        m_renderer.Clear(D2D1::ColorF(0.08f, 0.08f, 0.08f));
    }

    m_renderer.EndDraw();
}

void GameApp::OnResize(UINT width, UINT height)
{
    m_renderer.Resize(width, height);
}

void GameApp::OnMouseClick(float x, float y)
{
    if (m_currentScene)
    {
        m_currentScene->OnMouseClick(x, y);
    }
}

void GameApp::RegisterScene(const std::wstring& sceneName, SceneFactory factory)
{
    // 같은 이름을 다시 등록하면 덮어씁니다.
    // 이 방식은 테스트용 씬 교체나 컨텐츠 리로드를 붙일 때도 단순합니다.
    m_sceneFactories[sceneName] = std::move(factory);
}

bool GameApp::ChangeScene(const std::wstring& sceneName)
{
    const auto iter = m_sceneFactories.find(sceneName);
    if (iter == m_sceneFactories.end())
    {
        return false;
    }

    m_currentSceneName = sceneName;
    m_currentScene = iter->second(*this);

    if (m_currentScene)
    {
        m_currentScene->OnEnter();
    }

    return true;
}

D2DRenderer& GameApp::GetRenderer()
{
    return m_renderer;
}

const D2DRenderer& GameApp::GetRenderer() const
{
    return m_renderer;
}
