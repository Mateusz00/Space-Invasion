#ifndef AMMONODE_HPP
#define AMMONODE_HPP

#include "../SceneNode.hpp"
#include "../ResourcesID.hpp"
#include "Aircraft.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class AmmoNode : public SceneNode
{
    public:
                        AmmoNode(const Aircraft&, const TextureHolder&, const FontHolder&, const sf::View&);

    private:
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        const Aircraft&     mObject;
        const sf::View&     mView;
        sf::Sprite          mSprite;
        sf::Text            mAmount;
};

#endif // AMMONODE_HPP
