#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../SceneNode.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class Entity : public SceneNode
{
    public:
        Entity(int hitpoints);
        int getHitpoints() const;
        void setHitpoints(int hitpoints);
        sf::Vector2f getVelocity() const;
        void setVelocity(sf::Vector2f);
        void setVelocity(float x, float y);

    protected:
        virtual void updateCurrent(sf::Time dt) override;

    private:
        sf::Vector2f mVelocity;
        int mHitpoints;
};

#endif // ENTITY_HPP
