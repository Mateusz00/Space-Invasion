#include "AnimationNode.hpp"
#include "DataTable.hpp"
#include "Utility.hpp"
namespace
{
	const std::vector<AnimationData> table = initializeAnimationData();
}

AnimationNode::AnimationNode(Type type, sf::Time interval, const TextureHolder& textures)
	: mType(type),
	  mInterval(interval),
	  mSpriteSheet(textures.get(table[mType].spriteSheet)),
	  mSprite(mSpriteSheet, sf::IntRect(0, 0, table[mType].frameSize.x, table[mType].frameSize.y)),
	  mCurrentFrame(0),
	  mIsRepeating(false),
	  mNeedsUpdate(false)
{
    centerOrigin(mSprite);
}

void AnimationNode::setRepeating()
{
	mIsRepeating = true;
}

bool AnimationNode::isMarkedForRemoval() const
{
	if(!mIsRepeating && mCurrentFrame > table[mType].frames-1)
		return true;

	return false;
}

void AnimationNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue)
{
	mAccumulatedTime += dt;

	if(mAccumulatedTime >= mInterval)
    {
        if(!mIsRepeating)
            ++mCurrentFrame;
        else
            mCurrentFrame = mCurrentFrame % table[mType].frames;

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

		int left = (mCurrentFrame % table[mType].framesPerRow) * width;
		int top  = ((mCurrentFrame / table[mType].framesPerRow) % table[mType].rows) * height;
		mSprite.setTextureRect(sf::IntRect(left, top, width, height));
		mNeedsUpdate = false;
	}

    target.draw(mSprite, states);
}
