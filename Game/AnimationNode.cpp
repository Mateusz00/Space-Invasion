#include "AnimationNode.hpp"
#include "DataTable.hpp"
#include "Utility.hpp"
namespace
{
    const std::vector<AnimationData> table = initializeAnimationData();
}

AnimationNode::AnimationNode(Type type, sf::Time interval, const TextureHolder& textures)
    : mType(type),
      mAnimation(Animation::Forward),
      mInterval(interval),
      mSpriteSheet(textures.get(table[mType].spriteSheet)),
      mSprite(mSpriteSheet, sf::IntRect(table[mType].beginning, table[mType].frameSize)),
      mCurrentFrame(0),
      mIncrement(1),
      mIsRepeating(false),
      mNeedsUpdate(false)
{
    centerOrigin(mSprite);
}

void AnimationNode::setRepeating(bool flag)
{
    mIsRepeating = flag;
}

void AnimationNode::setAnimationType(Animation animation)
{
    mAnimation = animation;
}

bool AnimationNode::isMarkedForRemoval() const
{
    switch(mAnimation)
    {
        case Animation::Forward:
            if(!mIsRepeating && mCurrentFrame >= table[mType].frames)
                return true;
            else
                return false;

        case Animation::ForwardAndBackward:
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

        if(mAnimation == Animation::ForwardAndBackward)
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
        mSprite.setTextureRect(sf::IntRect(left, top, width, height));
        mNeedsUpdate = false;
    }

    target.draw(mSprite, states);
}
