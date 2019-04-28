#include "AnimationNode.hpp"
#include "DataTable.hpp"
#include "Utility.hpp"
namespace
{
    const std::vector<AnimationData> table = initializeAnimationData();
}

AnimationNode::AnimationNode(Animation::ID type, sf::Time interval, const TextureHolder& textures, sf::Vector2f pos)
    : AnimationNode(type, interval, textures)
{
    setPosition(pos);
}

AnimationNode::AnimationNode(Animation::ID type, sf::Time interval, const TextureHolder& textures)
    : SpriteNode(textures.get(table[type].spriteSheet), sf::IntRect(table[type].beginning, table[type].frameSize)),
      mType(type),
      mAnimation(AnimationType::Forward),
      mInterval(interval),
      mCurrentFrame(0),
      mIncrement(1),
      mIsRepeating(false),
      mNeedsUpdate(false)
{
    SpriteNode::center();
}

void AnimationNode::setRepeating(bool flag)
{
    mIsRepeating = flag;
}

void AnimationNode::setAnimationType(AnimationType animation)
{
    mAnimation = animation;
}

bool AnimationNode::isMarkedForRemoval() const
{
    switch(mAnimation)
    {
        case AnimationType::Forward:
            if(!mIsRepeating && mCurrentFrame >= table[mType].frames)
                return true;
            else
                return false;

        case AnimationType::ForwardAndBackward:
            if(!mIsRepeating && mCurrentFrame < 0)
                return true;
            else
                return false;
    }
}

void AnimationNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue)
{
    mAccumulatedTime += dt;

    if(mAccumulatedTime >= mInterval)
    {
        if(!mIsRepeating)
            mCurrentFrame += mIncrement;
        else
            mCurrentFrame = (mCurrentFrame + mIncrement) % table[mType].frames;

        if(mAnimation == AnimationType::ForwardAndBackward)
        {
            if(mIsRepeating && (mCurrentFrame >= table[mType].frames-1 || mCurrentFrame <= 0))
                mIncrement = -mIncrement;
            else if(!mIsRepeating && mCurrentFrame >= table[mType].frames-1)
                mIncrement = -mIncrement;
        }

        mNeedsUpdate = true;
        mAccumulatedTime -= mInterval;
    }
}

void AnimationNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mNeedsUpdate)
    {
        int width  = table[mType].frameSize.x;
        int height = table[mType].frameSize.y;

        int left = ((mCurrentFrame % table[mType].framesPerRow) * width)  + table[mType].beginning.x;
        int top  = ((mCurrentFrame / table[mType].framesPerRow) * height) + table[mType].beginning.y;
        SpriteNode::setTextureRect(sf::IntRect(left, top, width, height));
        mNeedsUpdate = false;
    }

    SpriteNode::drawCurrent(target, states);
}
