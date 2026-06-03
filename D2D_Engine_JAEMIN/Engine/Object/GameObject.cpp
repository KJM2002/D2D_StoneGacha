#include "GameObject.h"

GameObject::GameObject(const D2D1_RECT_F& bounds)
    : m_bounds(bounds)
{
}

void GameObject::SetBounds(const D2D1_RECT_F& bounds)
{
    m_bounds = bounds;
}

const D2D1_RECT_F& GameObject::GetBounds() const
{
    return m_bounds;
}

bool GameObject::Contains(float x, float y) const
{
    return x >= m_bounds.left && x <= m_bounds.right && y >= m_bounds.top && y <= m_bounds.bottom;
}

ButtonObject::ButtonObject(const D2D1_RECT_F& bounds, const std::wstring& text)
    : GameObject(bounds)
    , m_text(text)
{
}

const std::wstring& ButtonObject::GetText() const
{
    return m_text;
}
