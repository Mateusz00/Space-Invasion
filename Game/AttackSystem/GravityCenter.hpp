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
        void                            setDirection(sf::Vector2f);
        sf::Vector2f                    getDirection() const;
        void                            setStartPos(sf::Vector2f);
        sf::Vector2f                    getStartPos() const;
        float                           activeTime() const;
        void                            updateTime(sf::Time dt);

    private:
        AttackPattern::ID               mPattern;
        AttackPattern::PatternData      mPatternData;
        float                           mSpeed;
        sf::Vector2f                    mVelocity;
        sf::Vector2f                    mDirection;
        sf::Vector2f                    mStartPos;
        sf::Time                        mTimeActive;
};

#endif // GRAVITYCENTER_HPP
