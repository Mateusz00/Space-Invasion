#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../SceneNode.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <list>
class World;

class Entity : public SceneNode
{
    public:
        using ListIterator = std::list<Entity*>::iterator;

        explicit                Entity(int hitpoints, bool isCollidable, World&);
        virtual                 ~Entity();
        Entity&                 operator=(const Entity& other);
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
        World&                  getWorld() const;
        virtual sf::FloatRect   getLocalBounds() const;
        virtual void            onCollision(Entity&);
        virtual void            removeEntity();
        virtual bool            isMarkedForRemoval() const override;

    protected:
        virtual void            updateCurrent(sf::Time dt, CommandQueue&) override;

    private:
        sf::Vector2f            mVelocity;
        int                     mHitpoints;
        World&                  mWorld;
        bool                    mIsCollidable;
};

bool collision(const Entity& lhs, const Entity& rhs);

#endif // ENTITY_HPP
