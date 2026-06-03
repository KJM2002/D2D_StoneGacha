#include "D2DRenderer.h"

using Microsoft::WRL::ComPtr;

HRESULT D2DRenderer::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_d2dFactory.GetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(m_dwriteFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        return hr;
    }

    return CreateDeviceResources();
}

bool D2DRenderer::BeginDraw()
{
    if (FAILED(CreateDeviceResources()) || !m_renderTarget)
    {
        return false;
    }

    m_renderTarget->BeginDraw();
    return true;
}

void D2DRenderer::EndDraw()
{
    if (!m_renderTarget)
    {
        return;
    }

    HRESULT hr = m_renderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        DiscardDeviceResources();
    }
}

void D2DRenderer::Clear(const D2D1_COLOR_F& color)
{
    if (m_renderTarget)
    {
        m_renderTarget->Clear(color);
    }
}

void D2DRenderer::Resize(UINT width, UINT height)
{
    if (m_renderTarget)
    {
        m_renderTarget->Resize(D2D1::SizeU(width, height));
    }
}

D2D1_SIZE_F D2DRenderer::GetSize() const
{
    if (m_renderTarget)
    {
        return m_renderTarget->GetSize();
    }

    RECT rc{};
    if (m_hWnd)
    {
        GetClientRect(m_hWnd, &rc);
    }

    return D2D1::SizeF(static_cast<float>(rc.right - rc.left), static_cast<float>(rc.bottom - rc.top));
}

void D2DRenderer::DrawTextBlock(const std::wstring& text, const D2D1_RECT_F& rect, TextStyle style, const D2D1_COLOR_F& color)
{
    IDWriteTextFormat* format = GetTextFormat(style);
    if (!m_renderTarget || !format)
    {
        return;
    }

    SetBrushColor(color);
    m_renderTarget->DrawTextW(text.c_str(), static_cast<UINT32>(text.size()), format, rect, m_brush.Get());
}

void D2DRenderer::DrawRoundedPanel(const D2D1_RECT_F& rect, const D2D1_COLOR_F& fill, const D2D1_COLOR_F& stroke, float radius)
{
    if (!m_renderTarget)
    {
        return;
    }

    const D2D1_ROUNDED_RECT rounded = D2D1::RoundedRect(rect, radius, radius);
    SetBrushColor(fill);
    m_renderTarget->FillRoundedRectangle(rounded, m_brush.Get());
    SetBrushColor(stroke);
    m_renderTarget->DrawRoundedRectangle(rounded, m_brush.Get(), 2.0f);
}

void D2DRenderer::FillEllipse(const D2D1_POINT_2F& center, float radiusX, float radiusY, const D2D1_COLOR_F& color)
{
    if (!m_renderTarget)
    {
        return;
    }

    SetBrushColor(color);
    m_renderTarget->FillEllipse(D2D1::Ellipse(center, radiusX, radiusY), m_brush.Get());
}

void D2DRenderer::DrawEllipse(const D2D1_POINT_2F& center, float radiusX, float radiusY, const D2D1_COLOR_F& color, float strokeWidth)
{
    if (!m_renderTarget)
    {
        return;
    }

    SetBrushColor(color);
    m_renderTarget->DrawEllipse(D2D1::Ellipse(center, radiusX, radiusY), m_brush.Get(), strokeWidth);
}

void D2DRenderer::DrawLine(const D2D1_POINT_2F& from, const D2D1_POINT_2F& to, const D2D1_COLOR_F& color, float strokeWidth)
{
    if (!m_renderTarget)
    {
        return;
    }

    SetBrushColor(color);
    m_renderTarget->DrawLine(from, to, m_brush.Get(), strokeWidth);
}

void D2DRenderer::FillRectangle(const D2D1_RECT_F& rect, const D2D1_COLOR_F& color)
{
    if (!m_renderTarget)
    {
        return;
    }

    SetBrushColor(color);
    m_renderTarget->FillRectangle(rect, m_brush.Get());
}

HRESULT D2DRenderer::CreateDeviceResources()
{
    if (m_renderTarget)
    {
        return S_OK;
    }

    RECT rc{};
    GetClientRect(m_hWnd, &rc);

    HRESULT hr = m_d2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            m_hWnd,
            D2D1::SizeU(static_cast<UINT>(rc.right - rc.left), static_cast<UINT>(rc.bottom - rc.top))),
        m_renderTarget.GetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_brush.GetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    CreateTextFormat(44.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, m_titleFormat);
    CreateTextFormat(20.0f, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, m_buttonFormat);
    CreateTextFormat(22.0f, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, m_leftFormat);
    CreateTextFormat(22.0f, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, m_rightFormat);
    CreateTextFormat(17.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, m_smallCenterFormat);
    CreateTextFormat(16.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, m_smallLeftFormat);

    return S_OK;
}

void D2DRenderer::DiscardDeviceResources()
{
    m_brush.Reset();
    m_titleFormat.Reset();
    m_buttonFormat.Reset();
    m_leftFormat.Reset();
    m_rightFormat.Reset();
    m_smallCenterFormat.Reset();
    m_smallLeftFormat.Reset();
    m_renderTarget.Reset();
}

HRESULT D2DRenderer::CreateTextFormat(
    float fontSize,
    DWRITE_FONT_WEIGHT fontWeight,
    DWRITE_TEXT_ALIGNMENT textAlignment,
    DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment,
    ComPtr<IDWriteTextFormat>& textFormat)
{
    HRESULT hr = m_dwriteFactory->CreateTextFormat(
        L"Malgun Gothic",
        nullptr,
        fontWeight,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"ko-kr",
        textFormat.GetAddressOf());

    if (SUCCEEDED(hr) && textFormat)
    {
        textFormat->SetTextAlignment(textAlignment);
        textFormat->SetParagraphAlignment(paragraphAlignment);
    }

    return hr;
}

IDWriteTextFormat* D2DRenderer::GetTextFormat(TextStyle style) const
{
    switch (style)
    {
    case TextStyle::Title:
        return m_titleFormat.Get();
    case TextStyle::Button:
        return m_buttonFormat.Get();
    case TextStyle::Left:
        return m_leftFormat.Get();
    case TextStyle::Right:
        return m_rightFormat.Get();
    case TextStyle::SmallCenter:
        return m_smallCenterFormat.Get();
    case TextStyle::SmallLeft:
        return m_smallLeftFormat.Get();
    default:
        return m_smallCenterFormat.Get();
    }
}

void D2DRenderer::SetBrushColor(const D2D1_COLOR_F& color)
{
    if (m_brush)
    {
        m_brush->SetColor(color);
    }
}
