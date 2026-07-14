# SPH revival status

This note records the current state of the surviving SPH implementation in
Packfall and the shortest practical path toward SPH-DEM coupling.

## What is clearly alive already

The codebase still contains a usable SPH stack, not just orphaned files:

- `atom_style sph`
- `atom_style hybrid ... sph ...`
- `pair_style sph/artVisc/tensCorr`
- `pair_style sph/morris/tensCorr`
- `fix sph/density/continuity`
- `fix sph/density/summation`
- `fix sph/density/corr`
- `fix sph/pressure`
- `fix nve/sph`
- `fix nve/sph/stationary`
- `fix wall/sph`
- `fix wall/region/sph`
- `fix wall/sph/general`
- `fix wall/sph/general/gap`
- `fix sph/integrity`

The current test harness now exercises four SPH paths:

- continuity density update
- density summation
- stationary SPH wall particles
- a shortened tutorial-style wall-and-gravity regression

## What this means for coupling feasibility

The atom-style side is more encouraging than it first appears:

- `AtomVecSPH` implements the hybrid pack/unpack hooks
- `atom_style hybrid` explicitly documents union-of-attributes behavior
- the granular and SPH stacks can therefore likely coexist in one run at the
  storage/atom-style level

That is important, because it means SPH revival is probably not blocked by atom
storage alone.

## What is still missing for SPH-DEM coupling

No existing source or example was found that demonstrates an actual SPH-DEM
interaction law between mobile SPH particles and mobile DEM particles.

What exists today is:

- SPH-SPH interaction
- SPH-wall interaction
- DEM-DEM interaction
- DEM-CFD related coupling infrastructure in other parts of the codebase

What was not found:

- an SPH-DEM pair style
- an SPH-DEM contact/drag/buoyancy bridge for mobile solid particles
- an example using `pair_style hybrid` to combine SPH and granular physics in a
  single physically coupled run
- tests validating mixed SPH plus DEM execution

So the likely gap is not "SPH is gone", but rather "the fluid-solid coupling
layer is absent or incomplete".

## Recommended next implementation steps

1. Add a parser/execution test for `atom_style hybrid sphere sph`.
2. Add a parser/execution test for `pair_style hybrid` with one SPH and one
   granular sub-style in the same deck.
3. Determine the intended SPH-DEM interaction model:
   - unresolved drag/buoyancy only
   - contact-like SPH boundary force against DEM particles
   - fully resolved particle-fluid coupling
4. If unresolved coupling is the goal, start with a minimal per-particle force
   bridge:
   - fluid samples or reconstructed fields
   - drag term on DEM particles
   - reaction force back to SPH
5. If resolved coupling is the goal, introduce an explicit SPH-DEM interaction
   style and a validation suite around canonical cases.

## Suggested validation ladder

Build validation in this order:

1. SPH-only conservation and density sanity checks
2. SPH wall/channel benchmarks
3. mixed SPH-plus-DEM parser and one-step execution tests
4. single settling DEM sphere in SPH
5. buoyancy and terminal-velocity checks
6. packed DEM bed with SPH flow-through

This keeps the early work focused on proving infrastructure before investing in
expensive coupled benchmarks.

## Next bridge architecture

The current `fix couple/sphdem` is still a center-sampled unresolved bridge.
The recommended next step is an embedded-sampler coupling model, where each DEM
body owns multiple body-frame sample points that reconstruct SPH state locally
and accumulate distributed force and torque back to the rigid body.

That design is described in:

- `doc/sph_dem_embedded_bridge_design.md`
