#ifndef ANIMATIONNODE_HPP
#define ANIMATIONNODE_HPP

#include "SpriteNode.hpp"
#include "ResourcesID.hpp"
#include <SFML/System/Time.hpp>

class AnimationNode : public SpriteNode
{
    public:
        enum class AnimationType{Forward, ForwardAndBackward};

                        AnimationNode(Animation::ID, sf::Time interval, const TextureHolder&);
        void            setRepeating(bool);
        void            setAnimationType(AnimationType);
        bool            isMarkedForRemoval() const override;

    private:
        void            updateCurrent(sf::Time, CommandQueue&) override;
        void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        Animation::ID            mType;
        AnimationType            mAnimation;
        sf::Time                 mInterval;
        sf::Time                 mAccumulatedTime;
        int                      mCurrentFrame;
        int                      mIncrement;
        bool                     mIsRepeating;
        mutable bool             mNeedsUpdate;
};

#endif // ANIMATIONNODE_HPP
