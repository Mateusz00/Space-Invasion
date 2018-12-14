#ifndef LEVELSTATE_HPP
#define LEVELSTATE_HPP

#include "State.hpp"
#include "../GUI/GUIContainer.hpp"
#include "../GUI/GUIButton.hpp"
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
        class LevelButton : public GUIButton
        {
            public:
                    LevelButton(State::Context, ButtonID, const std::string&, int levelID, bool locked = true);
                int getLevelID() const;

            private:
                int mLevelID;
        };

        void            createLevelButton(int index);
        void            createConnectionLines();
        void            createLine(sf::FloatRect ObjBox1, sf::FloatRect ObjBox2, int width);

        sf::Sprite                  mBackground;
        GUIContainer                mButtons;
        sf::VertexArray             mLines;
        std::vector<LevelButton*>   mLevelButtons;
        static const int            mLevelNumber;
        int                         mLevel;
};

#endif // LEVELSTATE_HPP
