#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../SceneNode.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class Entity : public SceneNode
{
    public:
        explicit                Entity(int hitpoints);
        int                     getHitpoints() const;
        void                    setHitpoints(int hitpoints);
        sf::Vector2f            getVelocity() const;
        void                    setVelocity(sf::Vector2f);
        void                    setVelocity(float x, float y);
        void                    accelerate(sf::Vector2f);
        void                    accelerate(float x, float y);
        void                    damage(int hitpoints);
        void                    repair(int hitpoints);
        void                    destroy();
        virtual sf::FloatRect   getLocalBounds() const;

    protected:
        virtual void    updateCurrent(sf::Time dt, CommandQueue&) override;

    private:
        sf::Vector2f    mVelocity;
        int             mHitpoints;
};

#endif // ENTITY_HPP
