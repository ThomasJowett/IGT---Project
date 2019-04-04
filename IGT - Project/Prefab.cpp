#include "Prefab.h"

Factory<Prefab>::map_type * Factory<Prefab>::gLookUpTable = NULL;

DerivedRegister<ArrowPrefab> ArrowPrefab::reg("Arrow");

DerivedRegister<BarrelPrefab> BarrelPrefab::reg("Barrel");

DerivedRegister<ChestPrefab> ChestPrefab::reg("Chest");

DerivedRegister<CoinsPrefab> CoinsPrefab::reg("Coins");

DerivedRegister<LargeLootPrefab> LargeLootPrefab::reg("LargeLoot");

DerivedRegister<MediumLootPrefab> MediumLootPrefab::reg("MediumLoot");

DerivedRegister<SmallLootPrefab> SmallLootPrefab::reg("SmallLoot");

DerivedRegister<SlimePrefab> SlimePrefab::reg("Slime");

DerivedRegister<SnakePrefab> SnakePrefab::reg("Snake");

DerivedRegister<BatPrefab> BatPrefab::reg("Bat");

DerivedRegister<RatPrefab> RatPrefab::reg("Rat");

DerivedRegister<GoblinPrefab> GoblinPrefab::reg("Goblin");

DerivedRegister<BarbarianCharacterPrefab> BarbarianCharacterPrefab::reg("BarbarianCharacter");

DerivedRegister<ArcherCharacterPrefab> ArcherCharacterPrefab::reg("ArcherCharacter");