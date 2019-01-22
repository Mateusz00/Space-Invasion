#ifndef LEVELSTATE_HPP
#define LEVELSTATE_HPP

#include "State.hpp"
#include "../GUI/GUIContainer.hpp"
#include "../GUI/LevelButton.hpp"
#include "../GUI/TextBar.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
class Profile;

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
        void            createLine(sf::Vector2f, sf::Vector2f, float width);
        void            updateLevelStates();

        sf::Sprite                              mBackground;
        GUIContainer                            mButtons;
        sf::VertexArray                         mLines;
        Profile&                                mProfile;
        TextBar                                 mErrorMsg;
        std::unordered_map<int, LevelButton*>   mLevelButtons;
};

#endif // LEVELSTATE_HPP
