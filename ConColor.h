#ifndef __CONSOLE_COLOR_H__
#define __CONSOLE_COLOR_H__
#include <Windows.h>
#include <iostream>

enum FG_COLOR
{
    FG_BLACK,
    FG_GRAY,

    FG_LORED,
    FG_LOGREEN,
    FG_LOBLUE,
    FG_LOCYAN,
    FG_LOMAGENTA,
    FG_LOYELLOW,
    FG_LOWHITE,

    FG_HIRED,
    FG_HIGREEN,
    FG_HIBLUE,
    FG_HICYAN,
    FG_HIMAGENTA,
    FG_HIYELLOW,
    FG_HIWHITE,

    FG_PREVIOUS /*not changing the color*/
};

enum BG_COLOR
{
    BG_BLACK,
    BG_GRAY,

    BG_LORED,
    BG_LOGREEN,
    BG_LOBLUE,
    BG_LOCYAN,
    BG_LOMAGENTA,
    BG_LOYELLOW,
    BG_LOWHITE,

    BG_HIRED,
    BG_HIGREEN,
    BG_HIBLUE,
    BG_HICYAN,
    BG_HIMAGENTA,
    BG_HIYELLOW,
    BG_HIWHITE,

    BG_PREVIOUS /*not changing the color*/
};

#define FOREGROUND_MASK (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY)
#define BACKGROUND_MASK (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)
class ConColor
{
    WORD     m_wPackedColor;
    HANDLE   m_hConsole;
public:
    ConColor(FG_COLOR ForeGround, BG_COLOR Background)
    {
        m_wPackedColor = _ConvertColor(ForeGround, Background);
        m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    ConColor(WORD wPackedColor)
        : m_wPackedColor(wPackedColor)
    {
        m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    ConColor()
    {
        m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        _GetConsoleColor();
    }
    ~ConColor()
    {

    }

    // the magic is done here.
    template <typename _Elem, typename _Traits> inline friend
        std::basic_ostream<_Elem,_Traits>& operator<< (
        std::basic_ostream<_Elem,_Traits>& _Ostr,
        const ConColor& manip )
    {
        // flush anything in buffer
        _Ostr.flush();
        // set the color
        _SetColor(&manip);
        return _Ostr;
    };

private:
    VOID _GetConsoleColor()
    {
        CONSOLE_SCREEN_BUFFER_INFO ConInfo;
        GetConsoleScreenBufferInfo(m_hConsole, &ConInfo);
        m_wPackedColor = (ConInfo.wAttributes & ((BACKGROUND_MASK) | (FOREGROUND_MASK)));
    }
    WORD _ConvertColor(FG_COLOR ForeGround, BG_COLOR Background)
    {
        WORD wColor = 0;
        switch ( Background )
        {
        /*case BG_BLACK:*/
        case BG_PREVIOUS:
            wColor |= BACKGROUND_MASK;
            break;
        case BG_GRAY:
            wColor |= BACKGROUND_INTENSITY;
            break;

        case BG_HIRED:
            wColor |= BACKGROUND_INTENSITY;
        case BG_LORED:
            wColor |= BACKGROUND_RED;
            break;

        case BG_HIGREEN:
            wColor |= BACKGROUND_INTENSITY;
        case BG_LOGREEN:
            wColor |= BACKGROUND_GREEN;
            break;

        case BG_HIBLUE:
            wColor |= BACKGROUND_INTENSITY;
        case BG_LOBLUE:
            wColor |= BACKGROUND_BLUE;
            break;

        case BG_HICYAN:
            wColor |= BACKGROUND_INTENSITY;
        case BG_LOCYAN:
            wColor |= (BACKGROUND_GREEN | BACKGROUND_BLUE);
            break;

        case BG_HIMAGENTA:
            wColor |= BACKGROUND_INTENSITY;
        case BG_LOMAGENTA:
            wColor |= (BACKGROUND_RED | BACKGROUND_BLUE);
            break;

        case BG_HIYELLOW:
            wColor |= BACKGROUND_INTENSITY;
        case BG_LOYELLOW:
            wColor |= (BACKGROUND_RED | BACKGROUND_GREEN);
            break;

        case BG_HIWHITE:
            wColor |= BACKGROUND_INTENSITY;
        case BG_LOWHITE:
            wColor |= (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
            break;
        }

        switch ( ForeGround )
        {
        /*case FG_BLACK:*/
        case FG_PREVIOUS:
            wColor |= FOREGROUND_MASK;
            break;
        case FG_GRAY:
            wColor |= FOREGROUND_INTENSITY;
            break;

        case FG_HIRED:
            wColor |= FOREGROUND_INTENSITY;
        case FG_LORED:
            wColor |= FOREGROUND_RED;
            break;

        case FG_HIGREEN:
            wColor |= FOREGROUND_INTENSITY;
        case FG_LOGREEN:
            wColor |= FOREGROUND_GREEN;
            break;

        case FG_HIBLUE:
            wColor |= FOREGROUND_INTENSITY;
        case FG_LOBLUE:
            wColor |= FOREGROUND_BLUE;
            break;

        case FG_HICYAN:
            wColor |= FOREGROUND_INTENSITY;
        case FG_LOCYAN:
            wColor |= (FOREGROUND_BLUE | FOREGROUND_GREEN);
            break;

        case FG_HIMAGENTA:
            wColor |= FOREGROUND_INTENSITY;
        case FG_LOMAGENTA:
            wColor |= (FOREGROUND_RED | FOREGROUND_BLUE);
            break;

        case FG_HIYELLOW:
            wColor |= FOREGROUND_INTENSITY;
        case FG_LOYELLOW:
            wColor |= (FOREGROUND_RED | FOREGROUND_GREEN);
            break;

        case FG_HIWHITE:
            wColor |= FOREGROUND_INTENSITY;
        case FG_LOWHITE:
            wColor |= (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
            break;
        }
        return wColor;
    }

    static void _SetColor(const ConColor* manip)
    {
        ConColor* pThis = const_cast<ConColor*>(manip);
        SetConsoleTextAttribute(pThis->m_hConsole, pThis->m_wPackedColor);
    }
};

/*
int main()
{
    std::cout << " CMD Color demo."<< std::endl;

    std::cout << " Saving original color." << std::endl;
    ConColor OriginalColor;

    std::cout << ConColor(0x0A) << " Green on Black; using the same as color command parameter in CMD." << std::endl;
    std::cout << ConColor(FG_HIRED, BG_BLACK) << " Red on Black." << std::endl;
    std::cout << ConColor(FG_HIMAGENTA, BG_BLACK) << " Magenta on Black." << std::endl;

    std::cout << OriginalColor << " Resetting the color back." << std::endl;

    return 0;
}
*/
#endif