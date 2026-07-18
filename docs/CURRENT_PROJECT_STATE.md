# Hephaestus — Current Project State

Last reconciled: 2026-07-18

This document reconciles the historical Claude handover, the project owner's updated description, and the code currently in the repository. The untouched Claude documents are retained under `docs/claude-handover/` for provenance, but they are not the current source of truth for implementation status.

## Product vision

Hephaestus is intended to become a story-driven space-program game set during the 1960s space race.

- The player explores a small open-world town and a launch complex loosely inspired by Cape Canaveral.
- The space program grows by gathering launch resources, completing missions, and increasing its budget through successful results.
- The long-term story objective is to beat the Soviet program to the Moon.
- The planned visual direction is PS1-style low-poly art paired with strong, realistic lighting. Ray tracing is under consideration.
- Unreal Engine 5.8 is the selected engine for rendering and broader gameplay systems.
- The current repository is a standalone C++ physics prototype/core. No Unreal project or Unreal integration is present yet.

Story mode, the town, launch-complex gameplay, resource collection, budgets, missions, player controls, and rendering are not in development yet. The active focus is the physics foundation.

## Current technical shape

- C++17 and CMake 3.20+.
- Executable target: `Hephaestus`; CMake project name remains `PhysicsEngine`.
- `nlohmann-json` is the only third-party dependency.
- World data is loaded from `src/Data/Universe.json`.
- The JSON currently describes 18 bodies: the Sun, planets, and selected moons.
- Planetary motion uses fixed Keplerian rails. Spacecraft are still intended to use simulated forces rather than rails.
- Atmospheric source data currently consists of Venus CSV tables derived from probe data, plus per-body atmosphere metadata in `Universe.json`.

## Implemented

### Universe construction

`Universe_init` parses valid JSON and constructs a vector of `PlanetaryBody` objects. It loads each body's name, initial position, mass, radius, CSV paths, and orbital elements. Rotation-matrix components are computed once during construction. The Sun receives a zero-period orbit and is skipped by the orbital update.

### Keplerian planetary rails

The orbital design described as “not implemented” in the Claude handover is now present in code:

- `OrbitParams` and the cached `RotationFrame` are defined.
- Mean motion and mean anomaly are calculated from elapsed time.
- Kepler's equation is solved with six Newton–Raphson iterations.
- Perifocal coordinates are rotated into three-dimensional world coordinates.
- The main loop updates every non-zero-period body.
- A separate input thread allows the user to enter `E` to stop the loop.

The latest committed revision built and produced orbital debug output before the current atmosphere work began.

### Solar-system and atmosphere data

`Universe.json` is valid and substantially expanded compared with the Claude snapshot. It includes orbital and atmospheric metadata for 18 bodies. Six Venus atmosphere CSV files are currently present in the working tree.

## In progress or incomplete

### Atmosphere system — active work and current build blocker

The current working tree does not compile. A Visual Studio developer-environment build reaches `atmos_physics.cpp` and fails on these concrete issues:

- `planetID` and `timeOfDay` are referenced but are not parameters or local variables.
- `lookup_atmos_pressure` is called before a visible declaration.
- `std::to_string()` is called with no argument.
- `drag` and `calc_C_D` still have no return value.

There is also an interface split that should be resolved before implementation continues:

- `atmos_physics.hpp` declares static class methods.
- `atmos_physics.cpp` defines unrelated file-local free functions and does not include the header.
- The density function's header and source signatures do not match.
- The name `atmos_demsity_now` still contains a typo.

The universe data already contains molar mass in grams per mole and surface pressure in bar. Any ideal-gas density calculation must explicitly convert these to kilograms per mole and pascals before using `UNIVERSAL_GAS_CONSTANT`, or the result will have incorrect units.

### Atmosphere data consistency

- `Universe.json` still references `0VENUS1LOW2NORTH.csv` and `0VENUS1MID2NORTH.csv`, but both files are currently deleted in the working tree.
- The six present CSVs do not have one uniform row schema. In particular, `0VENUS1LOW2DAY.csv` declares three columns but its data rows contain a fourth value.
- Column order varies between files.
- Altitude and pressure units, time-of-day categories, layer boundaries, interpolation behavior, and the meaning of `planetaryAngle` need an explicit contract before the lookup code can be reliable.
- The atmospheric metadata in `Universe.json` is not loaded into `PlanetaryBody`; only `CSVPATHS` is retained.

### Parent-relative moon motion

`Universe.json` records a `parent` for moons, but `Universe_init` ignores it and `PlanetaryBody` does not store it. Every orbit is therefore evaluated around the world origin. Planetary heliocentric motion is present, but moons currently orbit the Sun/world origin instead of their parent planet. This is already identified in the latest commit message.

### Entity and spacecraft physics

`Entity` is declared again in the current working tree, but all definitions in `Entities.cpp` remain commented out. Gravity, aerodynamic surfaces, drag application, spacecraft state integration, and spacecraft tests are not implemented. The entity declarations will produce linker errors once those functions are actually used.

### Other incomplete code

- `Universe.cpp` is an unused stub; `GetPlanet` has no implementation or return.
- `UniversalFunctions.hpp` is entirely commented out.
- There is no automated test suite.
- The simulation loop has no fixed timestep, pacing, or separation between simulation time and wall-clock time.
- Orbital debug output is emitted for every body on every unrestricted loop iteration, which dominates runtime and floods the console.
- Positions, orbital parameters, and elapsed time use single-precision `float`. At solar-system distances this loses substantial positional precision and will be unsuitable for spacecraft-scale calculations without a deliberate coordinate/precision strategy.
- The orbital solver does not wrap mean anomaly into a bounded range, so long or accelerated simulations will accumulate avoidable precision loss.

## Reconciled status versus the Claude handover

The following Claude claims are obsolete:

- Orbital mechanics is no longer merely conceptual; the rail implementation exists and has run.
- `OrbitParams` and the cached rotation frame now exist.
- `PlanetaryBodies` has been replaced by the singular `PlanetaryBody` type.
- The JSON loader exists and `Universe.json` is valid and populated with 18 bodies.
- The main loop has been fixed and runs, with threaded exit input.
- A third-party dependency is now used (`nlohmann-json`).
- Unreal Engine 5.8 has been selected for graphics and gameplay, and the intended player experience is now defined.

The following handover points remain relevant:

- Planets use deterministic Keplerian rails while spacecraft are intended to use simulated physics.
- Atmospheric physics, entity gravity, drag, and aerodynamic representation remain incomplete.
- `Vec3` is minimal and mutable, and lacks the broader math operations a full spacecraft simulation will need.
- Data/logic boundaries and naming are inconsistent and should be settled before the atmosphere API expands.

## Recommended near-term sequence

1. Define the atmosphere API and units around planetary-body data rather than filename construction and disconnected integer IDs.
2. Reconcile the two deleted Venus tables and normalize or explicitly describe every CSV schema.
3. Make the header and implementation agree, then implement pressure lookup/interpolation and density with explicit unit conversions.
4. Add small data-validation and atmosphere unit tests before connecting drag.
5. Store parent relationships and compose moon positions with their parent's current position.
6. Choose the precision, coordinate-frame, timestep, and Unreal unit-conversion strategy before spacecraft propagation is implemented.

No source-code fixes were applied during this reconciliation; the current physics work was left intact.
