#ifndef LEVELSTATE_HPP
#define LEVELSTATE_HPP

#include "State.hpp"
#include "../GUI/GUIContainer.hpp"
#include "../GUI/GUIButton.hpp"
#include "../GUI/LevelButton.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class LevelState : public State
{
    public:
                        LevelState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        void            createLevelButton(int i);
        void            createConnectionLines();
        void            loadSave();
        void            createLine(float point1, float point2, int width);

        sf::Sprite                  mBackground;
        GUIContainer                mButtons;
        sf::VertexArray             mLines;
        std::vector<LevelButton*>   mLevelButtons;
        int                         mLevel;
};

#endif // LEVELSTATE_HPP
