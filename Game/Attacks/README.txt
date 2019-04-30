[Pattern IDs]
StraightLine - 0
Guided - 1
Wave - 2
Orbiting - 3
Barrier - 4

[Chargetime]
If set to 0 then it means that this attack is instant.

[AttackPhase](all attributes are optional)
1. linkedAttackID - id of attack that will be used as a phase.
2. repeatCooldown - How much time has to pass between using phase again.
3. repeats - How many times phase repeats itself until phaseCooldown is applied.
4. phaseCooldown - cool-down between phases.

[Projectile]
1. direction - angle
2. speed - speed of projectile, when projectile's pattern is  Oribiting/Barrier then it is interpreted as angular speed
3. aimed - boolean value. When true, projectile is directed at closest target(when there is no target then direction is read from projectile's attributes)
4. gravityCenterID - use only for orbiting projectiles
5. amplitude - use only for projectiles with wave pattern
6. waveLength - use only for projectiles with wave pattern
7. patternID
