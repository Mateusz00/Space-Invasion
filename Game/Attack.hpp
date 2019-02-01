#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "Projectile.hpp"
#include "ResourcesID.hpp"
#include "Command.hpp"
#include <SFML/System/Time.hpp>
#include <vector>
class CommandQueue;

class Attack
{
    public:
                Attack(int id, const TextureHolder&);
        void    update(sf::Time, CommandQueue&);

    private:
        void    setActive(bool);
        void    pushProjectile(Projectile*);
        void    createProjectile(SceneNode& layer, int num);

        std::vector<Projectile*>    mProjectiles;
        int                         mAttackID;
        bool                        mIsActive;
        sf::Vector2f                mPosition;
        Command                     mAttackCommand;
};

#endif // ATTACK_HPP
