#pragma once

#include "../Render/D2DRenderer.h"
#include "../Scene/Scene.h"

#include <Windows.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

// Engine/Core/GameApp
// 엔진의 중앙 실행 관리자입니다.
// 어떤 게임 컨텐츠가 들어오는지는 모르고, 등록된 씬을 실행/전환하는 일만 담당합니다.
class GameApp
{
public:
    using SceneFactory = std::function<std::unique_ptr<Scene>(GameApp&)>;

    HRESULT Initialize(HWND hWnd);

    void Update(float deltaTime);
    void Render();
    void OnResize(UINT width, UINT height);
    void OnMouseClick(float x, float y);

    void RegisterScene(const std::wstring& sceneName, SceneFactory factory);
    bool ChangeScene(const std::wstring& sceneName);

    D2DRenderer& GetRenderer();
    const D2DRenderer& GetRenderer() const;

private:
    HWND m_hWnd = nullptr;
    D2DRenderer m_renderer;
    std::unique_ptr<Scene> m_currentScene;
    std::wstring m_currentSceneName;
    std::unordered_map<std::wstring, SceneFactory> m_sceneFactories;
};
