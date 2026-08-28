# Current Units — Shooter, Lancer, Hunter

Descriptions of implemented units, their assets, and **current parameters** from Blueprint CDO snapshots.

> Parameters were exported with `Scripts/ExportUnitBlueprintValues.py`. To refresh:  
> Unreal Editor → **Tools → Execute Python Script** → `Scripts/ExportUnitBlueprintValues.py`  
> or from the editor: `Scripts/ExportUnitBlueprintValues.py`  
> Full machine-readable dump: [Units-BlueprintValues.json](./Units-BlueprintValues.json)

---

## Summary

| | Shooter | Lancer | Hunter |
|---|---------|--------|--------|
| **Role** | Light ranged unit | Medium turret tank | Heavy walker, gun + rockets |
| **Unit size** | — | Medium | Huge |
| **Unit cost** | 5 | 10 | — |
| **Max HP** | 85 | 120 | 255 |
| **Defence type** | Light | — | — |
| **Weapons** | 1× hitscan | 1× plasma (AOE) | 1× hitscan + 2× rocket launcher |
| **Rotated weapon** | No | Yes | Yes (launchers) |
| **Unit animation** | — | — | `UHunterAnimation` |
| **Weapon animation** | — | `ULancerCannonAnimation` | — |

---

## Shooter

### Description

Light tracked ground unit. Single weapon — fast-firing hitscan from the barrel. The whole body turns toward the target; the gun mesh is static. Minimal reference for new simple units.

### Assets

**Blueprints**

| File | Purpose |
|------|---------|
| `Content/Blueprint/Units/Shooter/BP_Shooter.uasset` | Main prefab |
| `Content/Blueprint/Units/Shooter/BP_ShooterProjectile.uasset` | Hitscan projectile |
| `Content/Blueprint/Units/Shooter/BP_ShooterProjectileImpact.uasset` | Impact VFX |
| `Content/Blueprint/Units/Shooter/BP_ShooterShotParticle.uasset` | Muzzle VFX |

**Models**

| Path | Contents |
|------|----------|
| `Content/Models/Units/Shooter/Meshes/` | Body, tracks, gun static meshes (`SM_Shooter_*`) |
| `Content/Models/Units/Shooter/Materials/M_Shooter.uasset` | Material |
| `Content/Models/Units/Shooter/Textures/` | `T_Baggi_D`, `T_Baggi_N` |

**C++**

Standard component set; no custom animation classes.

### Current Parameters

#### Unit Logic

| Parameter | Value |
|-----------|-------|
| Attack delta range | 15 |
| Rotate speed | 3 |
| Try attack delta | 2 |
| Unit cost | 5 |

#### Unit Parameters

| Parameter | Value |
|-----------|-------|
| Max health | 85 |
| Defence type | Light |

#### Universal Weapon Controller

| Parameter | Value |
|-----------|-------|
| Damage | 2.07 |
| Attack range | 1500 |
| Reload time | 1.01 |
| Critical chance | 30 |
| Critical rate | 1.85 |
| Attack angle | 2.6 |
| Rotate speed | 2.1 |
| Explosion radius | 0 (not AOE) |
| Ammunition count (`m_shotCount`) | 4 |
| Ammunition shot delay (`m_shotDelay`) | 0.1 |
| Delay between position shots | 0.1 |

#### Unit Scrap Destroy

| Parameter | Value |
|-----------|-------|
| Explosion radius | 115 |
| Min explosion power | 700 |
| Max explosion power | 1100 |
| Parts destroy time | 2 |

#### Projectile `BP_ShooterProjectile`

| Parameter | Value |
|-----------|-------|
| Add damage delay | 0.05 |

---

## Lancer

### Description

Medium tank with a **rotating turret** and small-radius plasma AOE shots. The cannon rotates independently of the hull; **`ULancerCannonAnimation`** plays between shots. Higher range and damage than Shooter; cost is 10 minerals.

### Assets

**Blueprints**

| File | Purpose |
|------|---------|
| `Content/Blueprint/Units/Lancer/BP_Lancer.uasset` | Main prefab |
| `Content/Blueprint/Units/Lancer/BP_PlasmaShotProjectile.uasset` | Plasma projectile |
| `Content/Blueprint/Units/Lancer/BP_TankShotParticle.uasset` | Muzzle VFX |

**Models and animation**

| Path | Contents |
|------|----------|
| `Content/Models/Units/Lancer/Meshes/` | Hull (`SM_Lancer_*`), cannon skeletal meshes (`SK_LancerCannon`, `SKM_LancerCannon`) |
| `Content/Models/Units/Lancer/Animation/A_LancerCannon.uasset` | Cannon recoil animation |
| `Content/Models/Units/Lancer/Materials/M_Lancer.uasset` | Material |
| `Content/Models/Units/Lancer/Textures/` | `T_Lancer_D`, `T_Lancer_N` |

**C++**

| Class | Purpose |
|-------|---------|
| `ULancerCannonAnimation` | `Source/AtomDestiny/Unit/Lancer/` — blocks Fire until animation is ready |

### Current Parameters

#### Unit Logic

| Parameter | Value |
|-----------|-------|
| Unit type | Lancer |
| Unit size | Medium |
| Unit cost | 10 |
| Attack delta range | 15 |
| Default stop distance | 250 |
| Try attack delta | 3 |

#### Unit Parameters

| Parameter | Value |
|-----------|-------|
| Max health | 120 |

#### Universal Weapon Controller

| Parameter | Value |
|-----------|-------|
| Damage | 16.3 |
| Attack range | 3500 |
| Critical chance | 25 |
| Critical rate | 1.45 |
| Attack angle | 4.5 |
| Explosion radius | 155 (AOE) |
| Rotated weapon | true |
| Rotate speed | 0.75 |
| Ammunition shot delay | 0 |

#### Unit Scrap Destroy

| Parameter | Value |
|-----------|-------|
| Min explosion power | 700 |
| Max explosion power | 1100 |
| Parts destroy time | 2 |

#### Projectile `BP_PlasmaShotProjectile`

Impact prefab is configured (see BP in the editor).

---

## Hunter

### Description

Heavy walker (**Huge**). Three weapon systems:

1. **Mesh gun** — fast hitscan center gun (short range).
2. **Left / Right launcher** — symmetric rocket launchers, long range, AOE, minimum firing distance 4500.

Uses **`UHunterAnimation`** and AnimBP **`ABP_Hunter`** (bool **`IsWalking`**). Launchers do not stop the unit when firing (`Stop when attack = false`); raycast is disabled for them.

### Assets

**Blueprints**

| File | Purpose |
|------|---------|
| `Content/Blueprint/Units/Hunter/BP_Hunter.uasset` | Main prefab |
| `Content/Blueprint/Units/Hunter/ABP_Hunter.uasset` | Animation Blueprint |
| `Content/Blueprint/Units/Hunter/BP_HunterGunProjectile.uasset` | Gun hitscan |
| `Content/Blueprint/Units/Hunter/BP_HunterGunShotParticle.uasset` | Gun muzzle VFX |
| `Content/Blueprint/Units/Hunter/BP_HunterGunProjectileImpact.uasset` | Gun impact VFX |
| `Content/Blueprint/Units/Hunter/BP_RocketProjectile.uasset` | Homing rocket |
| `Content/Blueprint/Units/Hunter/BP_HunterAimRocketTrail.uasset` | Rocket trail |

**Models and animation**

| Path | Contents |
|------|----------|
| `Content/Models/Units/Hunter/Meshes/SK_Hunter.uasset` | Skeletal mesh |
| `Content/Models/Units/Hunter/Animations/A_Hunter_Idle.uasset` | Idle |
| `Content/Models/Units/Hunter/Animations/A_Hunter_Walk.uasset` | Walk |
| `Content/Models/Units/Hunter/Materials/M_Hunter.uasset` | Material |
| `Content/Models/Units/Hunter/Textures/` | `T_Hunter_D`, `T_Hunter_N` |

**C++**

| Class | Purpose |
|-------|---------|
| `UHunterAnimation` | `Source/AtomDestiny/Unit/Hunter/` — drives `IsWalking` in AnimBP |

### Current Parameters

#### Unit Logic

| Parameter | Value |
|-----------|-------|
| Unit type | Hunter |
| Unit size | Huge |

#### Unit Parameters

| Parameter | Value |
|-----------|-------|
| Max health | 255 |

#### Mesh gun (Universal Weapon Controller)

| Parameter | Value |
|-----------|-------|
| Damage | 1.95 |
| Attack range | 2500 |
| Reload time | 3.9 |
| Critical chance | 27 |
| Critical rate | 1.75 |
| Attack angle | 2.7 |
| Rotate speed | 0.7 |
| Ammunition count | 100 |
| Ammunition shot delay | 0.15 |
| Delay between position shots | 0.1 |

#### Left / Right launcher (identical parameters)

| Parameter | Value |
|-----------|-------|
| Damage | 7.9 |
| Attack range | 15700 |
| Reload time | 5 |
| Attack angle | 9.5 |
| Explosion radius | 1500 |
| Min shot distance | 4500 |
| Weapon type | Explosive |
| Rotated weapon | true |
| Rotate speed | 1.1 |
| Stop when attack | false |
| Use raycast | false |
| Delay between position shots | 0.25 |
| Ammunition shot delay | 0 |

#### Projectiles

**`BP_HunterGunProjectile`**

| Parameter | Value |
|-----------|-------|
| Add damage delay | 0.05 |

**`BP_RocketProjectile`** (`AAimRocket`)

| Parameter | Value |
|-----------|-------|
| Speed | 550 |
| Life time | 15 |
| Locked time | 0.39 |
| Angular speed | 1.15 |

---

## Balance Notes

- **Attack range** and distances are in **Unreal units** (uu); effective range on a map depends on level scale.
- Shooter dump field **`m_shotCount`** maps to **`Ammunition count`** in the editor (legacy BP name).
- Lancer **`Rotated weapon`** + **`LancerCannonAnimation`**: can fire only when `IsReady() == true`.
- Hunter **launchers** fire without raycast — they may hit targets behind cover if logic still has them selected.

---

## Related Docs

- [Units.md](./Units.md) — general architecture and how to create new units
- [Units-BlueprintValues.md](./Units-BlueprintValues.md) — full automatic field dump (includes LaserTank)
