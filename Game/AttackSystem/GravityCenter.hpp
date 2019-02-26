#ifndef GRAVITYCENTER_HPP
#define GRAVITYCENTER_HPP

#include "AttackPattern.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
class CommandQueue;

class GravityCenter : public sf::Transformable
{
    public:
                                        GravityCenter(float speed, AttackPattern::ID, AttackPattern::PatternData);
        float                           getSpeed() const;
        void                            setSpeed(float);
        AttackPattern::ID               getPatternID() const;
        AttackPattern::PatternData      getPatternData() const;
        sf::Vector2f                    getVelocity() const;
        void                            setVelocity(sf::Vector2f);
        void                            setVelocity(float x, float y);
        void                            update(sf::Time, CommandQueue&);

    private:
        AttackPattern::ID               mPattern;
        AttackPattern::PatternData      mPatternData;
        float                           mSpeed;
        sf::Vector2f                    mVelocity;
};

#endif // GRAVITYCENTER_HPP
