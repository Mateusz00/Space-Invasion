#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

class Particle
{
	public:
		enum Type
		{
			Propellant,
			Smoke,
			ParticleCount
		};

                        Particle(sf::Vector2f, Type);
		void            update(sf::Time);
		void            changeVelocity(sf::Vector2f);
		sf::Color       getColor() const;
		sf::Vector2f    getPosition() const;

	private:
		sf::Time 		mLifeSpan;
		sf::Vector2f 	mPosition;
		sf::Vector2f 	mVelocity;
		sf::Color 	    mColor;
		Type			mType;
};

#endif // PARTICLE_HPP
