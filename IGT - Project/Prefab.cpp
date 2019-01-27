#include "Prefab.h"

Factory<Prefab>::map_type * Factory<Prefab>::gLookUpTable = NULL;

DerivedRegister<BarrelPrefab> BarrelPrefab::reg("Barrel");

DerivedRegister<ChestPrefab> ChestPrefab::reg("Chest");

DerivedRegister<CoinsPrefab> CoinsPrefab::reg("Coins");

DerivedRegister<LargeLootPrefab> LargeLootPrefab::reg("LargeLoot");

DerivedRegister<MediumLootPrefab> MediumLootPrefab::reg("MediumLoot");

DerivedRegister<SmallLootPrefab> SmallLootPrefab::reg("SmallLoot");