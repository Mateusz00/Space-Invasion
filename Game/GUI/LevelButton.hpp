#ifndef LEVELBUTTON_HPP
#define LEVELBUTTON_HPP

#include "GUIButton.hpp"
/// Add another sprite that will represent completed-mark
class LevelButton : public GUIButton
{
    public:
        enum LevelState
        {
            Normal,
            Selected,
            Pressed,
            Locked,
            Completed
        };

                LevelButton(State::Context, ButtonID, const std::string&, int levelID, bool locked = true);
        void    setCompleted();
};


#endif // LEVELBUTTON_HPP
