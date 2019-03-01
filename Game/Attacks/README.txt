[Pattern IDs]
StraightLine - 0
Guided - 1
Wave - 2
Orbiting - 3
Barrier - 4

[Chargetime]
If set to 0 then it means that this attack is instant

[Repeat]
1. cooldown - How much time has to pass between firing projectiles again. If set to 0 then this attack is used
2. times - How many times attack repeats itself until cooldown is applied(cooldown tag not attribute of repeat!)

[Projectile]
1. direction - direction vector
2. speed - speed of projectile, when projectile's pattern is  Oribiting/Barrier then it is interpreted as angular speed
3. aimed - boolean value. When true, projectile is directed at closest target(when there is no target then direction is read from projectile's attributes)
4. gravityCenterID - use only for orbiting projectiles
5. amplitude - use only for projectiles with wave pattern
5. waveLength - use only for projectiles with wave pattern
