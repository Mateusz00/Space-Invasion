#include "Utility.hpp"
#include <cmath>
#include <ctime>
#include <random>

namespace
{
    auto seed = static_cast<unsigned int>(std::time(nullptr));
    auto RandomEngine = std::default_random_engine(seed);
}

int randomInt(int minNumber, int maxNumberIncluded)
{
    std::uniform_int_distribution<int> distribution(minNumber, maxNumberIncluded);
    return distribution(RandomEngine);
}

float vectorLength(sf::Vector2f dist)
{
    return std::sqrt(dist.x * dist.x + dist.y * dist.y);
}

sf::Vector2f unitVector(sf::Vector2f distance)
{
    if(distance == sf::Vector2f(0.f, 0.f))
        throw std::logic_error("unitVector: Wrong length of vector!");

    return distance / vectorLength(distance); // Returns vector of length 1
}

sf::Vector2f unitVector(float x, float y)
{
    unitVector(sf::Vector2f(x, y));
}

float toRadian(float degree)
{
    return 3.1415f / 180.f * degree;
}

float toDegree(float rad)
{
    return 180.f / 3.1415f * rad;
}

std::string toString(sf::Keyboard::Key key)
{
    #define KEYTOSTRING_CASE(KEY) case sf::Keyboard::KEY: return #KEY;
    switch(key)
    {
        KEYTOSTRING_CASE(Unknown)
        KEYTOSTRING_CASE(A)
        KEYTOSTRING_CASE(B)
        KEYTOSTRING_CASE(C)
        KEYTOSTRING_CASE(D)
        KEYTOSTRING_CASE(E)
        KEYTOSTRING_CASE(F)
        KEYTOSTRING_CASE(G)
        KEYTOSTRING_CASE(H)
        KEYTOSTRING_CASE(I)
        KEYTOSTRING_CASE(J)
        KEYTOSTRING_CASE(K)
        KEYTOSTRING_CASE(L)
        KEYTOSTRING_CASE(M)
        KEYTOSTRING_CASE(N)
        KEYTOSTRING_CASE(O)
        KEYTOSTRING_CASE(P)
        KEYTOSTRING_CASE(Q)
        KEYTOSTRING_CASE(R)
        KEYTOSTRING_CASE(S)
        KEYTOSTRING_CASE(T)
        KEYTOSTRING_CASE(U)
        KEYTOSTRING_CASE(V)
        KEYTOSTRING_CASE(W)
        KEYTOSTRING_CASE(X)
        KEYTOSTRING_CASE(Y)
        KEYTOSTRING_CASE(Z)
        KEYTOSTRING_CASE(Num0)
        KEYTOSTRING_CASE(Num1)
        KEYTOSTRING_CASE(Num2)
        KEYTOSTRING_CASE(Num3)
        KEYTOSTRING_CASE(Num4)
        KEYTOSTRING_CASE(Num5)
        KEYTOSTRING_CASE(Num6)
        KEYTOSTRING_CASE(Num7)
        KEYTOSTRING_CASE(Num8)
        KEYTOSTRING_CASE(Num9)
        KEYTOSTRING_CASE(Escape)
        KEYTOSTRING_CASE(LControl)
        KEYTOSTRING_CASE(LShift)
        KEYTOSTRING_CASE(LAlt)
        KEYTOSTRING_CASE(LSystem)
        KEYTOSTRING_CASE(RControl)
        KEYTOSTRING_CASE(RShift)
        KEYTOSTRING_CASE(RAlt)
        KEYTOSTRING_CASE(RSystem)
        KEYTOSTRING_CASE(Menu)
        KEYTOSTRING_CASE(LBracket)
        KEYTOSTRING_CASE(RBracket)
        KEYTOSTRING_CASE(SemiColon)
        KEYTOSTRING_CASE(Comma)
        KEYTOSTRING_CASE(Period)
        KEYTOSTRING_CASE(Quote)
        KEYTOSTRING_CASE(Slash)
        KEYTOSTRING_CASE(BackSlash)
        KEYTOSTRING_CASE(Tilde)
        KEYTOSTRING_CASE(Equal)
        KEYTOSTRING_CASE(Dash)
        KEYTOSTRING_CASE(Space)
        KEYTOSTRING_CASE(Return)
        KEYTOSTRING_CASE(BackSpace)
        KEYTOSTRING_CASE(Tab)
        KEYTOSTRING_CASE(PageUp)
        KEYTOSTRING_CASE(PageDown)
        KEYTOSTRING_CASE(End)
        KEYTOSTRING_CASE(Home)
        KEYTOSTRING_CASE(Insert)
        KEYTOSTRING_CASE(Delete)
        KEYTOSTRING_CASE(Add)
        KEYTOSTRING_CASE(Subtract)
        KEYTOSTRING_CASE(Multiply)
        KEYTOSTRING_CASE(Divide)
        KEYTOSTRING_CASE(Left)
        KEYTOSTRING_CASE(Right)
        KEYTOSTRING_CASE(Up)
        KEYTOSTRING_CASE(Down)
        KEYTOSTRING_CASE(Numpad0)
        KEYTOSTRING_CASE(Numpad1)
        KEYTOSTRING_CASE(Numpad2)
        KEYTOSTRING_CASE(Numpad3)
        KEYTOSTRING_CASE(Numpad4)
        KEYTOSTRING_CASE(Numpad5)
        KEYTOSTRING_CASE(Numpad6)
        KEYTOSTRING_CASE(Numpad7)
        KEYTOSTRING_CASE(Numpad8)
        KEYTOSTRING_CASE(Numpad9)
        KEYTOSTRING_CASE(F1)
        KEYTOSTRING_CASE(F2)
        KEYTOSTRING_CASE(F3)
        KEYTOSTRING_CASE(F4)
        KEYTOSTRING_CASE(F5)
        KEYTOSTRING_CASE(F6)
        KEYTOSTRING_CASE(F7)
        KEYTOSTRING_CASE(F8)
        KEYTOSTRING_CASE(F9)
        KEYTOSTRING_CASE(F10)
        KEYTOSTRING_CASE(F11)
        KEYTOSTRING_CASE(F12)
        default:
            return "???";
    }
}

StringKeyMap::StringKeyMap()
{
    #define MAP_ENTRY(KEY) stringToKey.emplace(#KEY, sf::Keyboard::KEY);

    MAP_ENTRY(Unknown)
    MAP_ENTRY(A)
    MAP_ENTRY(B)
    MAP_ENTRY(C)
    MAP_ENTRY(D)
    MAP_ENTRY(E)
    MAP_ENTRY(F)
    MAP_ENTRY(G)
    MAP_ENTRY(H)
    MAP_ENTRY(I)
    MAP_ENTRY(J)
    MAP_ENTRY(K)
    MAP_ENTRY(L)
    MAP_ENTRY(M)
    MAP_ENTRY(N)
    MAP_ENTRY(O)
    MAP_ENTRY(P)
    MAP_ENTRY(Q)
    MAP_ENTRY(R)
    MAP_ENTRY(S)
    MAP_ENTRY(T)
    MAP_ENTRY(U)
    MAP_ENTRY(V)
    MAP_ENTRY(W)
    MAP_ENTRY(X)
    MAP_ENTRY(Y)
    MAP_ENTRY(Z)
    MAP_ENTRY(Num0)
    MAP_ENTRY(Num1)
    MAP_ENTRY(Num2)
    MAP_ENTRY(Num3)
    MAP_ENTRY(Num4)
    MAP_ENTRY(Num5)
    MAP_ENTRY(Num6)
    MAP_ENTRY(Num7)
    MAP_ENTRY(Num8)
    MAP_ENTRY(Num9)
    MAP_ENTRY(Escape)
    MAP_ENTRY(LControl)
    MAP_ENTRY(LShift)
    MAP_ENTRY(LAlt)
    MAP_ENTRY(LSystem)
    MAP_ENTRY(RControl)
    MAP_ENTRY(RShift)
    MAP_ENTRY(RAlt)
    MAP_ENTRY(RSystem)
    MAP_ENTRY(Menu)
    MAP_ENTRY(LBracket)
    MAP_ENTRY(RBracket)
    MAP_ENTRY(SemiColon)
    MAP_ENTRY(Comma)
    MAP_ENTRY(Period)
    MAP_ENTRY(Quote)
    MAP_ENTRY(Slash)
    MAP_ENTRY(BackSlash)
    MAP_ENTRY(Tilde)
    MAP_ENTRY(Equal)
    MAP_ENTRY(Dash)
    MAP_ENTRY(Space)
    MAP_ENTRY(Return)
    MAP_ENTRY(BackSpace)
    MAP_ENTRY(Tab)
    MAP_ENTRY(PageUp)
    MAP_ENTRY(PageDown)
    MAP_ENTRY(End)
    MAP_ENTRY(Home)
    MAP_ENTRY(Insert)
    MAP_ENTRY(Delete)
    MAP_ENTRY(Add)
    MAP_ENTRY(Subtract)
    MAP_ENTRY(Multiply)
    MAP_ENTRY(Divide)
    MAP_ENTRY(Left)
    MAP_ENTRY(Right)
    MAP_ENTRY(Up)
    MAP_ENTRY(Down)
    MAP_ENTRY(Numpad0)
    MAP_ENTRY(Numpad1)
    MAP_ENTRY(Numpad2)
    MAP_ENTRY(Numpad3)
    MAP_ENTRY(Numpad4)
    MAP_ENTRY(Numpad5)
    MAP_ENTRY(Numpad6)
    MAP_ENTRY(Numpad7)
    MAP_ENTRY(Numpad8)
    MAP_ENTRY(Numpad9)
    MAP_ENTRY(F1)
    MAP_ENTRY(F2)
    MAP_ENTRY(F3)
    MAP_ENTRY(F4)
    MAP_ENTRY(F5)
    MAP_ENTRY(F6)
    MAP_ENTRY(F7)
    MAP_ENTRY(F8)
    MAP_ENTRY(F9)
    MAP_ENTRY(F10)
    MAP_ENTRY(F11)
    MAP_ENTRY(F12)
}

sf::Keyboard::Key toKey(const std::string& keyString)
{
    static const StringKeyMap converter;

    auto found = converter.stringToKey.find(keyString);

    if(found != converter.stringToKey.end())
        return found->second;
    else
        return sf::Keyboard::Unknown;
}

bool isInRange(int low, int high, int number)
{
    return (((number - high) * (number - low)) <= 0);
}
