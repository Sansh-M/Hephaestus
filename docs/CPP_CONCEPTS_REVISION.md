# Hephaestus C++ Concepts Revision Guide

Last reviewed against the repository: 2026-08-19

This is a compact refresher for returning to Hephaestus after time away. It
deliberately skips basic variables, `if` statements, ordinary loops, and simple
class syntax. It concentrates on the C++ ideas that are easier to forget or use
incorrectly and ties each one to the project.

The saved source is authoritative. Some current files are experiments or are
incomplete, so **“present in the repository” does not automatically mean “a
finished pattern to copy.”** The final section separately records advanced ideas
that were discussed for future spacecraft and Unreal work but are not yet used
by the working physics core.

## Ten-minute recall checklist

Before changing the simulation, be able to explain these points:

1. `Universe` owns the runtime planet and entity collections.
2. `Universe(std::vector<PlanetaryBody>)` is a by-value sink; `std::move` enables
   the vector's move operation to transfer its buffer into `planets_`.
3. `const T&`, `T&`, and `T` express three different mutation/ownership contracts.
4. Returned references, pointers, and `std::string_view`s do not own what they
   refer to and can dangle.
5. `std::optional`, `nullptr`, `bool`, and exceptions communicate different kinds
   of failure.
6. `Universe::advance()` owns update order: sample old gravity, advance all
   planets once, then finish every entity's velocity-Verlet step.
7. A `std::thread` copies its arguments unless `std::ref` is used; a joinable
   thread must be joined or otherwise safely managed.
8. Header-defined static data needs the correct linkage (`inline static` in the
   C++17 rotation table); file-private helpers use an unnamed namespace.
9. A `float` has too little precision for both solar-system positions and
   spacecraft-scale changes in one global frame.
10. The current loop uses variable wall-clock `delta`; a fixed-timestep
    accumulator was discussed but is not implemented.

## 1. The current ownership and update model

The most important mental model is not a syntax feature; it is who owns state
and who is allowed to change it.

```text
main
  builds vector<PlanetaryBody>
          |
          v  moved into
      Universe
      |- planets_          authoritative mutable planet state
      |- entities_         authoritative mutable entity state
      |- PlanetaryMotion   updates prescribed planetary rails
      `- GravityEffect     calculates and integrates entity gravity
```

`main()` creates one local `Universe`, gives it the loaded planets, adds an
entity, and then calls `advance()`. Physics helpers do not own the planets or
recursively advance the world. This is **composition**: a `Universe` *has a*
`PlanetaryMotion` and a `GravityEffect`; it does not inherit from them.

This is also a small form of **dependency injection**. The already-built planet
collection is supplied to the `Universe` constructor instead of being hidden in
a global or created inside every physics system. Dependencies stay visible and
can later be replaced in tests or adapters.

Seen in:

- [`Universe.hpp`](../src/Universe.hpp)
- [`Universe.cpp`](../src/Universe.cpp)
- [`main.cpp`](../src/main.cpp)

### Why update order is an invariant

Velocity Verlet needs acceleration at both ends of a step. Because the planets
are moving prescribed gravity sources, `Universe::advance()` must:

1. evaluate every entity's acceleration using planet positions at
   `time.previous`;
2. advance the planet collection exactly once to `time.current`;
3. predict each entity's new position;
4. evaluate acceleration again using the new entity and planet positions;
5. update velocity using the average of the old and new acceleration.

Changing this order is not a harmless refactor. Advancing planets inside an
entity loop would make the result depend on how many entities exist and on their
container order.

## 2. Value semantics, ownership, and move semantics

Most project types (`Vec3`, `OrbitParams`, `Entity`, `PlanetaryBody`) currently
have **value semantics**: assigning or passing by value creates an independent
copy of their data.

The `Universe` constructor uses a useful **by-value sink** pattern:

```cpp
explicit Universe(std::vector<PlanetaryBody> planets);

Universe::Universe(std::vector<PlanetaryBody> planets)
    : planets_(std::move(planets)) {}
```

The parameter is a local object owned by the constructor. `std::move` does not
move anything by itself; it marks `planets` as eligible to have its resources
transferred. `std::vector`'s move operation can transfer its heap buffer instead
of copying every planet. The moved-from parameter remains valid but its contents
are unspecified, and it is about to be destroyed anyway.

Call-site consequence:

- Passing the temporary returned by `buildPlanetaryBodies()` is efficient: the
  temporary can be moved into the parameter and then into `planets_`.
- Passing an lvalue vector copies it unless the caller deliberately writes
  `std::move(myPlanets)` and accepts that it is relinquishing the contents.

`explicit` prevents a one-argument constructor from being used as an unintended
implicit conversion. `final` on `Universe` says that it is not designed as an
inheritance base class.

`Entity` also moves its by-value name parameter:

```cpp
Entity(std::string entityName, ...)
    : name(std::move(entityName)), ... {}
```

The move in the member initializer is essential to the sink pattern. The current
`PlanetaryBody` constructor accepts several strings/a vector by value but then
initializes the members with copies such as `name(name)` and
`CSVPATHS(CSVPATHS)`. If those parameters remain by-value sinks, moving them into
their members would avoid the extra copies. Conversely, `addEntity(const
Entity&)` deliberately copies the supplied entity into `entities_`.

### Constructor initializer lists

Members are initialized in the initializer list *before* the constructor body
runs. This is required for reference members and `const` members and is normally
more direct than default-constructing and then assigning.

Important trap: members initialize in the order they are **declared in the
class**, not the visual order in the initializer list. Keep both orders aligned.

Seen in:

- [`PlanetaryBody.hpp`](../src/PlanetaryBody.hpp)
- [`Entities.hpp`](../src/Entities.hpp)

## 3. References, pointers, views, and `const`

These spellings are API contracts, not decoration:

| Form | Meaning in this project | Typical use |
|---|---|---|
| `T value` | This function owns a copy/movable value | Constructor sink, small `Vec3` result |
| `const T& value` | Borrow an existing object; do not mutate it | Planet/vector inputs |
| `T& value` | Borrow and allow mutation | Entity integration, planet updates |
| `const T* value` | Borrow a possibly absent object | `findPlanet()` result |
| `std::string_view value` | Borrow a non-owning sequence of characters | Search key passed to `findPlanet()` |

Read pointer declarations from the name outward: `const T*` is a pointer through
which the `T` cannot be changed; `T* const` would instead be a pointer variable
that cannot be redirected. `findPlanet()` uses the first form.

A member function's trailing `const` means it does not modify the observable
state of its object:

```cpp
Vec3 totalAccelerationAt(...) const;
const Vec3& getPos() const;
```

The two `const`s in the second declaration mean different things: the returned
`Vec3` cannot be mutated through that reference, and `getPos()` does not mutate
the `PlanetaryBody`.

`Universe::planets()` returns `const std::vector<PlanetaryBody>&`. This avoids an
expensive collection copy while stopping the caller from adding, removing, or
mutating planets through that view. The `Universe` remains the owner.

### Lifetime rules to remember

- A reference, pointer, or `string_view` never extends the referred object's
  lifetime.
- A value returned by `findPlanet()` is valid only while its `Universe` exists
  and while the `planets_` vector has not reallocated or erased that element.
- A reference returned by `getPos()` is tied to its `PlanetaryBody`.
- Never return a reference or view to a local variable.
- Do not store the `string_view` search argument unless the caller guarantees
  that its original characters remain alive.

The current `findPlanet()` consumes the view immediately, so its use is safe.

### `noexcept`

The `Universe` accessors and `findPlanet()` are marked `noexcept`, promising that
they will not let exceptions escape. This documents intent and can enable some
library optimizations. Only use it when every operation on the path is genuinely
non-throwing; violating the promise calls `std::terminate`.

## 4. Structs, aggregates, and data-oriented records

The project uses small structs to group values that form one concept:

- `SimulationTime`: current time, previous time, and their derived delta;
- `OrbitParams`: the seven orbital elements used by the rail solver;
- `RotationFrame`: cached matrix components;
- `PlanetaryCoordinate`: latitude, longitude, and altitude;
- `AeroCoefficients`, `AeroCoefficientPoint`, and
  `CoefficientAeroDefinition`: increasingly larger aerodynamic records;
- nested `RotationParameters` and `SolarFrame`: details private to one system.

These are **value objects** rather than inheritance hierarchies. Brace
initialization constructs them compactly and prevents narrowing conversions in
many contexts:

```cpp
Vec3 velocity{0.0f, 0.0f, 0.0f};
AtmosphereRow previous{}; // value-initialize all numeric fields to zero
```

Aggregate initialization is positional. Reordering fields silently changes the
meaning of existing initializers, so field order is part of the data contract.
For larger records, a validating constructor or builder becomes safer.

Default member initializers, such as `velocity{0.0f, 0.0f, 0.0f}`, apply when a
constructor does not explicitly initialize that member.

## 5. STL containers and allocation behavior

### `std::vector`

`std::vector<T>` owns a contiguous, dynamically sized sequence of `T` values.
Hephaestus uses it for planets, entities, coefficient-table points, coordinates,
and cached acceleration samples.

```cpp
std::vector<PlanetaryBody> planets;
planets.reserve(jsonPlanetCount);
planets.push_back(buildPlanet(...));
```

`reserve(n)` changes capacity, not size. It prevents repeated allocations while
loading a known number of bodies. It does not create `n` elements, so indexed
access is still invalid until elements are inserted.

When a vector grows beyond its capacity, reallocation can move every element and
invalidate pointers, references, and iterators into it. Indices survive a pure
reallocation but can change meaning after insertion or erasure before them.

Choose the loop based on what must change:

```cpp
for (const auto& planet : planets) { /* read without copying */ }
for (auto& planet : planets)       { /* mutate each planet */ }
for (std::size_t i = 0; i < entities.size(); ++i) { /* align two vectors */ }
```

`auto` deduces the underlying type, but a plain `auto` variable is a value copy
and drops top-level references/`const`. Write `auto&` or `const auto&` explicitly
when borrowing is the point. In contrast, copying a small iterator into
`const auto found` is intentional.

### `std::map`

`PlanetaryRotation` stores rotation records in
`std::map<std::string, RotationParameters>`. `find()` returns an iterator; compare
it with `end()` before using `iterator->second`. A map owns both its keys and
values and performs ordered logarithmic lookup.

For a fixed, read-heavy table, the key design and missing-key behavior matter
more than the container syntax. `parametersFor()` converts a missing map entry
into a clear exception.

## 6. Iterators, standard algorithms, and lambdas

Standard algorithms separate *how to traverse* from *what qualifies*.

`Universe::findPlanet()` combines `std::find_if` with a lambda:

```cpp
const auto found = std::find_if(
    planets_.cbegin(), planets_.cend(),
    [id](const PlanetaryBody& planet) {
        return std::string_view(planet.getName()) == id;
    });
```

- `cbegin()`/`cend()` produce const iterators.
- `[id]` captures the search view by value.
- The lambda parameter is a const reference, avoiding a planet copy.
- The result is either the matching iterator or the end sentinel.
- `&*found` first dereferences the iterator and then takes the element's address.

`PlanetaryMotion` uses `[&]` to capture surrounding locals by reference. This is
convenient but broad: future edits can accidentally make the lambda depend on a
new local. Prefer an explicit capture such as `[&planet]` or `[parentName]` when
the dependency is small.

The atmosphere path builder uses `std::transform` with a lambda. Passing an
`unsigned char` to `std::toupper` before converting back to `char` avoids
undefined behavior for negative signed-`char` values.

Seen in:

- [`Universe.cpp`](../src/Universe.cpp)
- [`PlanetaryMotion.hpp`](../src/Physics/PlanetaryMotion.hpp)
- [`atmos_physics.cpp`](../src/Physics/atmos_physics.cpp)

## 7. Operator overloading for mathematical value types

`Vec3` overloads operators so numerical code resembles the equations:

```cpp
const Vec3 newPosition =
    oldPosition + oldVelocity * dt + oldAcceleration * (0.5f * dt * dt);
```

A non-mutating binary operator returns a new value and is itself `const`:

```cpp
Vec3 operator+(const Vec3& other) const;
```

A compound assignment mutates the left operand and returns `*this` by reference:

```cpp
Vec3& operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
```

Returning `Vec3&` permits normal chaining and matches built-in operator behavior.
Keep overloads unsurprising: `+` should not mutate either operand, while `+=`
should mutate only the left operand.

Project-specific traps:

- C++ `^` is bitwise XOR, **not exponentiation**. Use `x * x`, `x * x * x`, or
  `std::pow` when a genuine general exponent is needed.
- The existing member `Vec3::operator*(float)` supports `vector * scalar` but not
  `scalar * vector`; symmetric syntax would require a suitable non-member
  overload.
- The current `normalize(Vec3)` signature is an awkward hybrid: it is a
  non-static member but normalizes its parameter instead of `*this`. Treat it as
  unfinished API design rather than a pattern to repeat.

Seen in [`constants.hpp`](../src/constants.hpp).

## 8. Compile-time constants, class-wide state, and linkage

### `constexpr`

`constexpr` says a value or function can participate in compile-time evaluation
when its inputs permit it. Physics constants and angle constants use it:

```cpp
namespace Physics {
    constexpr float GRAV_CONST = 6.67430e-11f;
}
```

The namespace prevents generic names such as `PI` from polluting the global
namespace. Prefer qualified use such as `Physics::GRAV_CONST` over
`using namespace Physics` in headers.

### Static members and `inline static` in C++17

A static data member belongs to the class rather than to each object. The
rotation parameter table should exist once, not once per `PlanetaryRotation`.

Because it is defined in a header included by multiple translation units, C++17
`inline static` allows identical definitions to be merged without violating the
**one-definition rule (ODR)**:

```cpp
inline static const std::map<std::string, RotationParameters>
    rotationParameters = { /* ... */ };
```

A static member function has no `this` pointer and can directly access only
arguments and static members. This suits helpers such as `dot`, `cross`, and
`parametersFor` that do not depend on a particular `PlanetaryRotation` object.

### Translation-unit-private helpers

The unnamed namespace in `atmos_physics.cpp` gives its names **internal
linkage**: other `.cpp` files cannot refer to that `BAR_TO_PASCALS` or molar-mass
table. File-level `static` functions in the same file serve a similar purpose,
though an unnamed namespace is the clearer modern grouping.

## 9. Headers, source files, and the one-definition rule

A header is textually inserted wherever it is included. A `.cpp` file becomes a
separate **translation unit**. The linker later joins compiled translation
units.

Important project patterns:

- `#pragma once` prevents one header from being processed twice in a translation
  unit. Its absence previously caused a `PlanetaryMotion` redefinition.
- `Universe.hpp` declares the public type; `Universe.cpp` defines functions with
  scope resolution such as `Universe::advance`.
- Functions defined inside a class body are implicitly `inline`, allowing their
  definitions to appear through the header in multiple translation units.
- Non-inline free-function or static-data definitions placed in a header can
  produce duplicate-symbol linker errors.
- Include what the file actually uses. Do not rely on another header happening
  to include a dependency transitively.
- Avoid `using namespace ...` in headers because it changes name lookup in every
  file that includes them.

Circular design dependencies are usually a responsibility problem, not merely
an include-order problem. The current direction avoids `GravityEffect` owning or
calling `Universe`; `Universe` orchestrates, while `GravityEffect` operates on
the entity and planet data it is given.

## 10. Representing “no result” and handling failure

The code uses four distinct failure contracts:

| Mechanism | Meaning | Project example |
|---|---|---|
| `bool` + output reference | Parsing succeeded or failed | `parseAtmosphereRow(line, row)` |
| `std::optional<T>` | Absence is an expected result | Pressure file/range lookup |
| nullable pointer | An object lookup may miss | `findPlanet()` returns `nullptr` |
| exception | Operation cannot meet its contract | Missing JSON file, invalid coordinate |

### `std::optional`

`std::optional<double>` stores either a `double` or no value:

```cpp
auto pressure = lookup_atmos_pressure(path, altitude);
if (!pressure) {
    return 0.0f;
}
const double pressurePa = *pressure * BAR_TO_PASCALS;
```

Dereference only after checking. `std::nullopt` explicitly constructs the empty
state. Optional is better than a magic pressure such as `-1.0` because every
ordinary `double` remains available for actual data.

### Exceptions and invariants

The JSON loader throws `std::runtime_error` when a required file cannot open.
`PlanetaryRotation` uses `std::invalid_argument` for invalid values and
`std::out_of_range` for values outside permitted numerical bounds. The CSV parser
catches conversion exceptions because a malformed row is expected input noise,
not a reason to abort the whole simulation.

An **invariant** is a condition a type or function relies on always being true.
Examples include a non-zero vector before normalization, finite coordinates,
latitude within `[-pi/2, pi/2]`, positive temperature for the ideal-gas law, and
a positive distance before division. Validate invariants at the boundary where
bad data first becomes meaningful.

Do not use exceptions for routine per-frame branching. Do not silently convert a
real programming/configuration error into zero unless zero genuinely has the
documented physical meaning.

## 11. Streams, parsing, aliases, and template syntax

Standard streams use RAII: `std::ifstream` opens a file and closes it when the
stream object leaves scope, including during exception unwinding.

The atmosphere reader uses a `std::stringstream` and repeated
`std::getline(stream, field, ',')` calls to split one CSV row. `std::stod`
converts text—including scientific notation—to `double` and may throw, which is
why parsing is wrapped in `try`/`catch`.

The JSON loader defines a type alias:

```cpp
using json = nlohmann::json;
```

This creates another name for the same type; it does not create a new type.

Calls such as:

```cpp
p.at("name").get<std::string>()
```

use a **function template**. The `<std::string>` template argument selects the
requested conversion type. `at()` performs checked lookup and throws when a
required key is missing; `contains()` is appropriate for explicitly optional
fields such as `CSVPATHS`.

Seen in:

- [`Universe_init.cpp`](../src/Universe_init.cpp)
- [`atmos_physics.cpp`](../src/Physics/atmos_physics.cpp)

## 12. Threads, atomics, references, and time types

The console exit path introduces several non-trivial concurrency rules:

```cpp
std::atomic<bool> exit{false};
std::thread inputThread(listenForExit, std::ref(exit));
// ... simulation loop ...
inputThread.join();
```

- A data race occurs when threads access the same memory concurrently, at least
  one access writes, and there is no synchronization. A data race is undefined
  behavior.
- `std::atomic<bool>` makes the exit flag's concurrent reads and writes safe.
  Its default operations use sequentially consistent ordering, which is more
  than sufficient for this simple stop flag.
- `std::thread` normally copies/decays function arguments into its own storage.
  `std::ref(exit)` wraps the atomic so `listenForExit(std::atomic<bool>&)` receives
  the original object by reference.
- A joinable thread must be `join()`ed or `detach()`ed before its destructor;
  otherwise the program calls `std::terminate`. Joining is the safe choice here
  because shutdown waits for the input worker.
- Do not let a referenced object die while the worker can still access it.

`std::chrono::steady_clock` is monotonic, so it is appropriate for elapsed
durations even if the system clock changes. `duration<float>(now - start).count()`
converts the elapsed interval to floating-point seconds; `duration_cast` performs
an explicit duration-unit conversion for the final microsecond report.

### Wall-clock time is not yet a fixed simulation timestep

The current loop sets `SimulationTime.current` from real elapsed time and uses
`current - previous` directly. The numerical step therefore changes with CPU
load and console output. A fixed-step design instead accumulates real elapsed
time and advances the simulation in repeated constant-size steps. This was
discussed, but it is not present in the current source.

Seen in [`main.cpp`](../src/main.cpp).

## 13. Numerical C++ habits used by the physics code

These are not unique to C++, but C++ makes the representation choices explicit.

### `float` versus `double`

`float` usually carries about seven decimal digits of precision; `double` about
fifteen to sixteen. At a position near `1e11` metres, adjacent representable
`float` values can be kilometres apart. A small spacecraft movement can vanish
when added to that global position.

The code already uses some `double` values for planetary coordinates,
interpolation, gas calculations, and rotations, then uses `static_cast<float>`
where it deliberately returns to the current `Vec3` representation. The cast
makes narrowing visible; it does not prevent precision loss.

Long term, choose a deliberate strategy: double-precision authoritative physics,
well-defined local/reference frames, explicit units, and conversion to Unreal's
renderer coordinates only at the boundary.

### Floating-point validation

- `std::isfinite` rejects NaN and infinities before coordinate math.
- Near-equality uses a tolerance in the atmosphere lookup rather than exact
  equality.
- Exact `== 0.0f` guards in normalization/gravity work for literal zero inputs
  but are not a general near-zero test.
- `std::remainder(angle, 2*pi)` bounds rotation angles and reduces long-run
  trigonometric precision loss.
- Check denominators before division and domains before `sqrt`, `asin`, or `log`.

### Units are part of the type contract

Names such as `altitudeKm`, `pressureBar`, `temperatureK`, `referenceAreaM2`, and
`timeSinceEpochSeconds` carry unit information that a plain `double` cannot.
The atmosphere calculation explicitly converts bar to pascals and stores molar
mass in kg/mol before applying `rho = pM/(RT)`.

Keep units in names and interfaces until the project introduces stronger unit
types. Never let a conversion be implied by a comment alone.

## 14. Query functions, cached data, and separation of responsibilities

Several design patterns make numerical systems easier to reason about:

- **Factory/loading boundary:** the private
  `Universe_init::BuildPlanetaryBody()` translates JSON into a complete
  `PlanetaryBody` and precomputes its `RotationFrame`, so callers do not repeat
  construction rules or remember to initialize the cache.
- **Query rather than hidden mutation:** `PlanetaryRotation::queryWorldCoordinate`
  computes a world coordinate when requested without rewriting the stored
  body-fixed coordinate.
- **Cache stable derived data:** orbital rotation-matrix components are computed
  while loading a body rather than rebuilding them every timestep.
- **Batch shared work:** `queryWorldCoordinates()` computes the current rotating
  basis once, reserves output storage, then transforms many coordinates.
- **Keep calculation helpers narrow:** `GravityEffect` receives the state it
  needs; it does not parse JSON, own entities, or run the main loop.
- **Keep orchestration at the owner:** `Universe::advance()` decides when each
  subsystem runs and maintains cross-system ordering.

“Pure” numerical helpers—same output for the same input and no hidden mutation—
are especially valuable for unit testing. Some current helpers are already close
to this form even when they are member functions.

## 15. Concepts discussed but not yet implemented

These belong in the refresher because they were part of the project's C++ design
work, but do not mistake them for current working source.

### `enum class` for closed states

Use a scoped, strongly typed enum for a finite category or state:

```cpp
enum class FlightState {
    Assembled,
    PoweredFlight,
    Coasting,
    Landed
};
```

Unlike old unscoped enums, names remain under `FlightState::`, and values do not
silently behave like arbitrary integers. This was recommended for flight,
attachment, engine, and simulation states; no `enum class` is currently in the
physics core.

### Narrow runtime polymorphism

Inheritance was recommended only for genuinely interchangeable behavior, such
as player versus AI guidance or alternative atmosphere/trajectory models:

```cpp
class GuidanceSystem {
public:
    virtual ~GuidanceSystem() = default;
    virtual Vec3 command(/* state */) = 0;
};

class PlayerGuidance final : public GuidanceSystem {
public:
    Vec3 command(/* state */) override;
};
```

- `= 0` makes a function pure virtual and the base abstract.
- `virtual` enables dispatch through a base pointer/reference.
- The virtual destructor makes deletion through a base pointer safe.
- `override` asks the compiler to verify that the signature really overrides.
- `final` prevents further inheritance/overriding where extension is not part of
  the design.
- Storing a derived object by value in a base object causes **object slicing**;
  polymorphic objects are normally accessed through references or owning smart
  pointers.

`std::unique_ptr<GuidanceSystem>` was the recommended ownership type for one
exclusive, dynamically selected behavior. It destroys the object automatically,
cannot be copied, and can be moved. The project does not currently need smart
pointers for its value-owned planet/entity vectors.

### Spacecraft composition

The planned direction is:

- `Spacecraft` owns rigid-body state, stages, and `PartInstance` values;
- immutable/reusable `PartDefinition` data is separate from mutable per-craft
  instance state;
- one shared spacecraft-physics system computes gravity, thrust, drag, and
  integration;
- rendered meshes, collision shapes, and aerodynamic proxies remain separate;
- stage separation splits the part graph, recomputes mass properties, and
  preserves momentum.

Recall rule: **composition for what a spacecraft has, `enum class` for which
state/category it is in, and inheritance for interchangeable ways it behaves.**

### Snapshot boundary to Unreal

The discussed boundary is a temporary, copied snapshot of simulation state—not
live `PlanetaryBody*` objects, vector indices, or a save file. An adapter would
translate authoritative physics data into Unreal-friendly records. Unreal could
display those records without owning or mutating the standalone physics core.

The current Unreal C++ content is only a generated module shell; the orbit-map
subsystem/type files are empty drafts. `IMPLEMENT_PRIMARY_GAME_MODULE(...)` in
[`HephaestusGame.cpp`](../Unreal/HephaestusGame/Source/HephaestusGame/HephaestusGame.cpp)
is an Unreal preprocessor macro, not ordinary function-call syntax.

## Return-to-project workflow

When resuming work after a break:

1. Read this guide and [`CURRENT_PROJECT_STATE.md`](CURRENT_PROJECT_STATE.md), but
   verify both against the saved source because project-state prose can age.
2. Re-read the owner and timestep path in `main.cpp`, `Universe.cpp`,
   `PlanetaryMotion.hpp`, and `GravityEffect.hpp` before changing physics order.
3. Identify each new API's ownership, mutation, lifetime, units, and failure
   contract before writing its body.
4. Prefer one small pure numerical helper plus a focused test over connecting an
   unfinished subsystem directly to the main loop.
5. Build and run the focused path, then explain the result in terms of the
   relevant invariant—not merely whether it compiled.
