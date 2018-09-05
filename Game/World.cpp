#include "World.hpp"

World::World(sf::RenderTarget& target, TextureHolder& textures, FontHolder& fonts)
    : mTarget(target),
      mTextures(textures),
      mFonts(fonts)
{
    mTextures.loadFromFile(Textures::Background, "Resources/Background.png");
    sf::Texture& background = mTextures.get(Textures::Background);
    background.setRepeated(true);
    mBackground.setTexture(background);
    mBackground.setTextureRect(sf::IntRect(0, 0, 1024, 768));
    mBackground.setPosition(0.f, 0.f);
}

void World::update()
{
}

void World::draw()
{
    mTarget.draw(mBackground);
}
