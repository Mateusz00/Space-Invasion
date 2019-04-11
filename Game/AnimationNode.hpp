#ifndef ANIMATIONNODE_HPP
#define ANIMATIONNODE_HPP

#include "SceneNode.hpp"
#include "ResourcesID.hpp"
#include <SFML/System/Time.hpp>

class AnimationNode : public SceneNode
{
    public:
        enum Type{Explosion, Count};
        enum class Animation{Forward, ForwardAndBackward};

                        AnimationNode(Type, sf::Time interval, const TextureHolder&);
        void            setRepeating(bool);
        void            setAnimationType(Animation);
        virtual bool    isMarkedForRemoval() const override;

    private:
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        Type                     mType;
        Animation                mAnimation;
        sf::Time                 mInterval;
        sf::Time                 mAccumulatedTime;
        const sf::Texture&       mSpriteSheet;
        mutable sf::Sprite       mSprite;
        int                      mCurrentFrame;
        int                      mIncrement;
        bool                     mIsRepeating;
        mutable bool             mNeedsUpdate;
};

#endif // ANIMATIONNODE_HPP
