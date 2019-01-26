#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "Projectile.hpp"
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

class Attack : public sf::Transformable
{
    public:
        enum Behavior
        {
            StraightLine,///
            Guided,///
            Spiral,///
            Orbital///
        };

        Attack();
        void pushProjectile(Projectile*);
        void update(sf::Time);

    private:
        std::vector<Projectile*> mProjectiles;
        int mAttackID; /// Will be read from XML file
};

#endif // ATTACK_HPP
