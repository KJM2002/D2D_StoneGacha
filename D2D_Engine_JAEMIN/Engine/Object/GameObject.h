#pragma once

#include <d2d1.h>
#include <string>

// Engine/Object/GameObject
// 엔진 공통 오브젝트입니다. 특정 게임의 돌, 상점 주인 같은 의미는 알지 못합니다.
class GameObject
{
public:
    GameObject() = default;
    explicit GameObject(const D2D1_RECT_F& bounds);
    virtual ~GameObject() = default;

    void SetBounds(const D2D1_RECT_F& bounds);
    const D2D1_RECT_F& GetBounds() const;

    bool Contains(float x, float y) const;

private:
    D2D1_RECT_F m_bounds{};
};

class ButtonObject : public GameObject
{
public:
    ButtonObject() = default;
    ButtonObject(const D2D1_RECT_F& bounds, const std::wstring& text);

    const std::wstring& GetText() const;

private:
    std::wstring m_text;
};
