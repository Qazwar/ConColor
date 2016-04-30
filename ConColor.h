#ifndef __CONSOLE_COLOR_H__
#define __CONSOLE_COLOR_H__
#include <Windows.h>
#include <iostream>

namespace ConsoleColor
{
enum color : char
{
    Black,
    Gray,
    LoRed,
    LoGreen,
    LoBlue,
    LoCyan,
    LoMagenta,
    LoYellow,
    LoWhite,
    HiRed,
    HiGreen,
    HiBlue,
    HiCyan,
    HiMagenta,
    HiYellow,
    HiWhite,
    Previous /*keep previous color*/
};
}
#define FOREGROUND_MASK (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY)
#define BACKGROUND_MASK (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)

#define PREV_COLOR 0x0100
#define DELAY_CONV 0xFF00
namespace ConsoleColor
{
class Color
{
public:
    Color()
    {
        _Init();
        m_PackedColor = _PrevColor();
    }
    Color(color foreground, color background)
    {
        _Init();
        m_PackedColor = DELAY_CONV;
        m_ColorPair.set(foreground, background);
    }
    Color(WORD packedColor)
    {
        _Init();
        m_PackedColor = packedColor  & (BACKGROUND_MASK | FOREGROUND_MASK);
    }
    ~Color() {}

    // the magic is here.
    template <typename _Elem, typename _Traits> inline friend
        std::basic_ostream<_Elem, _Traits>& operator<< (
            std::basic_ostream<_Elem, _Traits>& _Ostr,
            Color& manip)
    {
        // flush anything in buffer
        _Ostr.flush();
        // set the color
        _SetColor(manip);
        return _Ostr;
    };
    static Color fg(color foreground)
    {
        return Color(foreground, color::Previous);

    }
    static Color bg(color background)
    {
        return Color(color::Previous, background);
    }
private:
    HANDLE m_hConsole;
    struct ColorPair
    {
        color m_fg;
        color m_bg;
        void set(color fg, color bg)
        {
            m_fg = fg;
            m_bg = bg;
        }
        color fg() { return m_fg; }
        color bg() { return m_bg; }
    };
    union
    {
        WORD  m_PrevColor;
        ColorPair m_ColorPair;
    };
    WORD   m_PackedColor;

    void _Init()
    {
        m_PrevColor = PREV_COLOR;
        m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    WORD _PrevColor()
    {
        if (m_PrevColor == PREV_COLOR)
        {
            CONSOLE_SCREEN_BUFFER_INFO ConInfo = { 0 };
            GetConsoleScreenBufferInfo(m_hConsole, &ConInfo);
            m_PrevColor = (ConInfo.wAttributes & ((BACKGROUND_MASK) | (FOREGROUND_MASK)));
        }
        return m_PrevColor;
    }
    void _PreProcess()
    {
        if (m_PackedColor == DELAY_CONV)
            m_PackedColor = _Convert(m_ColorPair.fg(), m_ColorPair.bg());
    }
    WORD _Convert(color foreground, color background)
    {
        if ((foreground == color::Previous)
            && (background == color::Previous))
            return PREV_COLOR;

        if (m_PackedColor == DELAY_CONV)
            m_PrevColor = PREV_COLOR;

        auto fg = _Convert(foreground);
        if (fg == PREV_COLOR)
            fg = (_PrevColor() & FOREGROUND_MASK);

        auto bg = _Convert(background);
        if (bg == PREV_COLOR)
            bg = (_PrevColor() & BACKGROUND_MASK);
        else
            bg <<= 4;

        return (fg | bg);
    }
    static WORD _Convert(color Color)
    {
        WORD PackedColor = 0;
        switch (Color)
        {
            case color::Black:
                break;
            case color::LoRed:
                PackedColor = FOREGROUND_RED;
                break;
            case color::LoGreen:
                PackedColor = FOREGROUND_GREEN;
                break;
            case color::LoBlue:
                PackedColor = FOREGROUND_BLUE;
                break;
            case color::LoCyan:
                PackedColor = FOREGROUND_BLUE | FOREGROUND_GREEN;
                break;
            case color::LoMagenta:
                PackedColor = FOREGROUND_RED | FOREGROUND_BLUE;
                break;
            case color::LoYellow:
                PackedColor = FOREGROUND_RED | FOREGROUND_GREEN;
                break;
            case color::LoWhite:
                PackedColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                break;

            case color::Gray:
            case color::HiRed:
            case color::HiGreen:
            case color::HiBlue:
            case color::HiCyan:
            case color::HiMagenta:
            case color::HiYellow:
            case color::HiWhite:
                PackedColor |= FOREGROUND_INTENSITY;
                PackedColor |= _Convert(_LoPart(Color));
                break;

            case color::Previous:
                PackedColor = PREV_COLOR;
                break;
        }
        return PackedColor;
    }
    static color _LoPart(color Color)
    {
        switch (Color)
        {
            case color::Gray:
                return color::Black;
            case color::HiRed:
                return color::LoRed;
            case color::HiGreen:
                return color::LoGreen;
            case color::HiBlue:
                return color::LoBlue;
            case color::HiCyan:
                return color::LoCyan;
            case color::HiMagenta:
                return color::LoMagenta;
            case color::HiYellow:
                return color::LoYellow;
            case color::HiWhite:
                return color::LoWhite;
        }
        return Color;
    }
    static const void _SetColor(Color& manip)
    {
        manip._PreProcess();
        if (manip.m_PackedColor != PREV_COLOR)
            SetConsoleTextAttribute(manip.m_hConsole, manip.m_PackedColor);
    }
};
}
#endif // __CONSOLE_COLOR_H__

/*
how to use
int main()
{
namespace con = ConsoleColor;
std::cout << " CMD Color demo." << std::endl;
std::cout << " Saving original color." << std::endl;
con::Color OriginalColor;
std::cout << con::Color(0x0A) << " Green on Black; using the same as color command parameter in CMD." << std::endl;
std::cout << con::Color(con::HiRed, con::Black) << " Red on Black." << std::endl;
std::cout << con::Color(con::HiMagenta, con::Previous) << " Magenta on Black." << std::endl;
std::cout << OriginalColor << " Resetting the color back." << std::endl;
return 0;
}
*/
