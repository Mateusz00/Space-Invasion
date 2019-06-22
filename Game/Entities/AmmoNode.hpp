#ifndef AMMONODE_HPP
#define AMMONODE_HPP

#include "../SceneNode.hpp"
#include "../ResourcesID.hpp"
#include "Spaceship.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class AmmoNode : public SceneNode
{
    public:
        enum Align{Left, Right};
                        AmmoNode(const Spaceship&, const TextureHolder&, const FontHolder&, const sf::View&);
        void            setMargin(float percent);
        void            align(Align);

    private:
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        const Spaceship&    mObject;
        const sf::View&     mView;
        sf::Sprite          mSprite;
        sf::Text            mAmount;
        float               mMargin;
        Align               mAlignment;
};

#endif // AMMONODE_HPP
