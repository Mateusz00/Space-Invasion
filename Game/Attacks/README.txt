[Behavior IDs]
StraightLine - 0
Guided - 1
Spiral - 2
Orbiting - 3
Barrier - 4

[Chargetime]
If set to 0 then it means that this attack is instant

[Repeat]
cooldown - How much time has to pass between firing projectiles again. If set to 0 then this attack is used
times - How many times attack repeats itself until cooldown is applied(cooldown tag not attribute of repeat!)

[Projectile]
direction - direction vector
speed - speed of projectile, when projectile's behavior is Oribiting/Barrier then it is used to determine speed of origin
angularSpeed - Used when projectile's behavior is Oribiting or Barrier.
aimed - boolean value. When true, projectile is directed at closest target(when there is no target then direction is read from projectile's attributes)
