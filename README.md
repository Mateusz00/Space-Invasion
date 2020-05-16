# About
Space Invasion is a vertically scrolling shoot 'em up, written in C++ without using any existing game engine. This was my first bigger project and my main goal was to gain experience in OOP, which isn't possible with smaller applications. This also was main reason why I chose to avoid using ready-made solutions such as game engine, GUI library. Main goal of the game is to clear all levels and get highest score possible. Game can be downloaded <a href="https://github.com/Mateusz00/2D-Fighter-Jet-Game/releases/tag/v1.0">here</a>.

# Features
* Save & load system.
* Dynamic key binding system.
* Local co-op (2 players).
* Pause state.
* Highscores.
* Pixel perfect collision.
* 40 types of enemies and 6 levels and more can be easily added by modifying xml files.
* 6 types of attacks that enemies can use (StraightLine, Aimed, Homing, Spiral, Orbiting, Barrier).
* 4 types of power ups.

# Implementation details
* Collision detection is divided into two phases: 
	* Broad phase collision detection using AABB Tree, in this phase we check which objects can possibly collide against each other, this phase is meant to be fast and can give false positives.
	* Narrow phase collision detection using pixel perfect collision, in this phase we check which collision pairs are really colliding.
* Attacks consist of attack phases which can have other nested phases. Attack phases can be either implemented in the same file they are declared or referenced (To avoid duplicating common patterns).
* Most of the game data is loaded from xml files in order to allow users to easily modify game content to match their preferences.

# Demo
![Demo gameplay](https://github.com/Mateusz00/resources-for-repositories/blob/master/SpaceInvasionGameplay.gif)
![Demo menu](https://github.com/Mateusz00/resources-for-repositories/blob/master/SpaceInvasionMenu.gif) 

# Credits 
* Pickups, level background, buttons were done by myself so feel free to use them however you want.
* Main menu theme - <a href="https://johnbartmann.com/public-domain-music/interstellar-space-public-domain-music/">John Bartmann</a>.
* Battle theme - (Edited) <a href="https://freemusicarchive.org/music/Komiku/Captain_Glouglous_Incredible_Week_Soundtrack/pog">Battle of Pogs by Komiku</a>.
* Spaceships - (Edited) <a href="https://opengameart.org/content/shmup-ships">surt</a>, <a href="https://opengameart.org/content/retro-spaceships">Jerom</a>, <a href="https://opengameart.org/content/spaceships-1">wuhu</a>, (Edited) <a href="https://opengameart.org/content/biomech-dragon-cannon">MetaShinryu, Surt and Daniel Cook</a>.
* Projectiles - Master484 (<a href="https://opengameart.org/content/bullet-collection-1-m484">1</a>, <a href="https://opengameart.org/content/bullet-collection-2-m484-games">2</a>).
* Menu background - <a href="https://opengameart.org/content/space-backdrop">beren77</a>.
