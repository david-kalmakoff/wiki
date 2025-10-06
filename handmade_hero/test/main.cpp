#include <cstdio>

void foo() { printf("here we are.\n"); }

struct projectile {
  char unsigned IsThisOnFire;
  int Damage;
  int ParticlePerSecond;
  short HowManyCooks;
};

int main(int argc, char *arg[]) {

  /*
  char SmallS;          // 8 bits [-128,127]
  char unsigned SmallU; // [0,255]

  short MediumS; // 16 bits
  short unsigned MediumU;

  int LargeS; // 32 bits
  int unsigned LargeU;
  */

  projectile Test;

  int Size = sizeof(Test);

  Test.IsThisOnFire = 1;
  Test.Damage = 2;
  Test.ParticlePerSecond = 3;
  Test.HowManyCooks = 4;

  short *Pointer = (short *)&Test;

  projectile Projectiles[40];

  int WhichProjectile = 4;
  Projectiles[WhichProjectile];

  projectile *ProjectilePointer = Projectiles;

  int x = 5;
  int y = 10;
  int z = 12;

  // Arithmetic
  x = -y + z * x + y / x - z;

  // Bitwise math
  x = 0xA;
  x = x << 4;
  x = x >> 4;

  x = 0;
  x = x | (1 << 4); // set the 4th value
  x = x | (1 << 8); // set the 8th value
  x = x & (1 << 4); // mask out the 4th bit
  x = ~x;

  x = (1 << 4) | (1 << 8);
  y = (1 << 8) | (1 << 16);
  x = x ^ y;
  x = x ^ y;

  x = 0;        // FALSE
  y = 1;        // TRUE
  z = 100;      // TRUE
  int w = -300; // TRUE

  // Things are either zero or they are not

  int less = (x < y);
  int greater = (x > y);
  int equal = (x == y);
  int notequal = (x != y);
}
