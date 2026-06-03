#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>

#include <string>

enum class TextStyle
{
    Title,
    Button,
    Left,
    Right,
    SmallCenter,
    SmallLeft
};

// Engine/Render/D2DRenderer
// Direct2D/DirectWrite 자원 생성과 기본 그리기 함수를 담당합니다.
// 컨텐츠는 "텍스트를 그려줘", "패널을 그려줘"처럼 요청만 하고 COM 자원 관리는 몰라도 됩니다.
class D2DRenderer
{
public:
    HRESULT Initialize(HWND hWnd);

    bool BeginDraw();
    void EndDraw();
    void Clear(const D2D1_COLOR_F& color);
    void Resize(UINT width, UINT height);

    D2D1_SIZE_F GetSize() const;

    void DrawTextBlock(const std::wstring& text, const D2D1_RECT_F& rect, TextStyle style, const D2D1_COLOR_F& color);
    void DrawRoundedPanel(const D2D1_RECT_F& rect, const D2D1_COLOR_F& fill, const D2D1_COLOR_F& stroke, float radius = 8.0f);
    void FillEllipse(const D2D1_POINT_2F& center, float radiusX, float radiusY, const D2D1_COLOR_F& color);
    void DrawEllipse(const D2D1_POINT_2F& center, float radiusX, float radiusY, const D2D1_COLOR_F& color, float strokeWidth = 1.0f);
    void DrawLine(const D2D1_POINT_2F& from, const D2D1_POINT_2F& to, const D2D1_COLOR_F& color, float strokeWidth = 1.0f);
    void FillRectangle(const D2D1_RECT_F& rect, const D2D1_COLOR_F& color);

private:
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();
    HRESULT CreateTextFormat(
        float fontSize,
        DWRITE_FONT_WEIGHT fontWeight,
        DWRITE_TEXT_ALIGNMENT textAlignment,
        DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment,
        Microsoft::WRL::ComPtr<IDWriteTextFormat>& textFormat);
    IDWriteTextFormat* GetTextFormat(TextStyle style) const;
    void SetBrushColor(const D2D1_COLOR_F& color);

private:
    HWND m_hWnd = nullptr;

    Microsoft::WRL::ComPtr<ID2D1Factory> m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_brush;
    Microsoft::WRL::ComPtr<IDWriteFactory> m_dwriteFactory;

    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_titleFormat;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_buttonFormat;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_leftFormat;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_rightFormat;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_smallCenterFormat;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_smallLeftFormat;
};
