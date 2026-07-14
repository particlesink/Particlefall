# SPH-DEM embedded sampler bridge design

This note describes the next practical evolution of `fix couple/sphdem`.
The goal is to move from the current one-point unresolved bridge to a
distributed coupling model closer to what commercial tools often describe as
"embedded SPH samples inside a rigid DEM particle".

## Why the current bridge is not enough

The current `fix couple/sphdem` applies drag, pressure-gradient, and buoyancy
at the DEM particle center using a single kernel-weighted reconstruction of the
surrounding SPH field.

That is a useful prototype, but it has predictable limitations:

- buoyancy turns on too early near a free surface
- the full particle volume is used even when the particle is only partly
  immersed
- no hydrodynamic torque is generated from asymmetric immersion
- superquadrics and multisphere bodies are reduced to a single support point
- interface behavior depends too strongly on smoothing length and support radius

These are exactly the cases where an embedded-sampler method helps.

## Target model

The target coupling should be:

- `SPH-SPH` for the carrier-phase evolution
- `embedded-sampler to SPH` interpolation for local fluid state
- `embedded-sampler to rigid DEM body` force and torque accumulation

Conceptually:

1. Each DEM body owns a set of sampling points fixed in the body frame.
2. At every timestep, those points are mapped to world coordinates.
3. SPH field quantities are reconstructed at each sample point from nearby SPH
   particles.
4. Pointwise drag, pressure, and buoyancy contributions are computed.
5. Contributions are summed into a net force and torque on the DEM body.
6. Equal-and-opposite reaction is distributed back to neighboring SPH particles.

This makes coupling indirect in a physically useful way:

- the fluid does not act on the rigid body only at its center
- the body does not need an ad hoc global immersed fraction
- partial immersion appears naturally from which sample points are inside SPH
  support

## Incremental implementation strategy

We should not jump directly to a full rigid-body, arbitrary-shape model.
The clean path is:

1. Keep the current center-sample bridge as `v0`.
2. Add an embedded-sampler path for spheres first.
3. Extend the same machinery to superquadrics.
4. Extend again to multisphere bodies.

This keeps each stage testable.

## Proposed code structure

The current bridge logic lives entirely in:

- `src/fix_couple_sphdem.h`
- `src/fix_couple_sphdem.cpp`

That file should remain the top-level fix, but the internal logic should be
split into explicit stages:

- sampler generation
- SPH field reconstruction at sampler locations
- pointwise force evaluation
- rigid-body accumulation
- SPH reaction distribution

Suggested internal helpers:

- `generate_body_samples(...)`
- `reconstruct_fluid_state_at_sample(...)`
- `compute_sample_drag(...)`
- `compute_sample_pressure_force(...)`
- `compute_sample_buoyancy(...)`
- `accumulate_body_force_and_torque(...)`
- `distribute_sample_reaction_to_fluid(...)`

If the implementation grows, the bridge can later be split into:

- `src/sph_dem_sampler.h/.cpp`
- `src/sph_dem_interpolation.h/.cpp`
- `src/sph_dem_force_model.h/.cpp`

but that is not necessary for the first embedded version.

## Sampler representation

Each DEM particle needs body-frame sample coordinates.

For a sphere:

- start with a small, fixed radial shell or interior point set
- examples:
  - center only
  - 6 axis points
  - 18 to 32 quasi-uniform interior points

For a superquadric:

- generate samples in the body frame using normalized parametric coordinates
- classify them as interior or near-surface points
- transform to world coordinates using the current orientation

For a multisphere body:

- reuse existing multisphere body information if available
- either:
  - use the component sphere centers as sample anchors, or
  - generate per-component sub-samples and accumulate to the parent body

Required per-sample data:

- body-local position
- optional weight
- optional representative volume
- optional surface normal or signed-distance metadata

## First sphere sampler scheme

The sphere-first version should be intentionally simple.

Recommended initial scheme:

- one sample at the center
- six samples at `0.5 R` on `+/-x`, `+/-y`, `+/-z`
- optional twelve more off-axis points if needed later

Each sample gets an equal or volume-weighted fraction of the total sphere
volume. The important point is that the total represented volume sums to the
actual sphere volume.

This already fixes the worst free-surface behavior:

- top samples leave SPH support earlier than bottom samples
- buoyancy ramps in and out with immersion
- nonzero torque appears if the fluid support is asymmetric

## Fluid-state reconstruction at each sample

At each sample point, reconstruct:

- local fluid velocity
- local density
- local pressure
- local pressure gradient
- optional local voidage proxy

Use the same kernel family already used by SPH so the reconstruction remains
consistent with existing SPH machinery.

For sample point `s`, accumulate over nearby SPH particles:

- `u_f(s)`
- `rho_f(s)`
- `p_f(s)`
- `grad p_f(s)`

using the same weighted interpolation pattern already used in the current
bridge, but now evaluated at each sample instead of only at the DEM center.

Important numerical rules:

- skip non-finite SPH contributors
- skip zero or negative density contributors
- require a minimum support weight before accepting a sample
- store a support flag per sample

## Pointwise force model

For sample point `s`, define:

- `v_body(s) = v_cm + omega x r_s`
- `u_rel(s) = u_f(s) - v_body(s)`

Then compute:

### Drag

Start with the same linear unresolved drag already used by the current bridge:

- `F_drag(s) = C_d * V_s * u_rel(s)`

where `V_s` is the sample-represented volume.

Later upgrades can use:

- Reynolds-number dependent drag
- porosity-corrected drag
- anisotropic drag for non-spherical shapes

### Pressure-gradient force

Use:

- `F_pg(s) = -V_s * grad p_f(s)`

This is the same term as today, but distributed across the body.

### Buoyancy

Use:

- `F_b(s) = -rho_f(s) * V_s * g`

This is still Archimedes-style, but now it acts only where samples actually
see fluid support.

That means the total effective immersed volume becomes:

- `V_eff = sum(V_s for supported samples)`

which is much better than applying the full rigid-body volume from a single
center sample.

## Force and torque accumulation

For each DEM body:

- `F_total = sum_s F_s`
- `T_total = sum_s (r_s_world - x_cm) x F_s`

For ordinary sphere DEM particles:

- apply `F_total` to `atom->f[i]`
- apply `T_total` to the torque storage already used by sphere/superquadric
  integration

For superquadrics:

- use the same force path, but ensure torque accumulation lands in the correct
  rotational state variables used by `fix nve/superquadric`

For multisphere:

- accumulate to the parent body, not merely to component spheres individually

The multisphere infrastructure already exists in the tree, so this should be
designed around body-level accumulation instead of atom-level hacks.

## Reaction back to the SPH phase

Each sample force must push back onto the SPH phase conservatively.

For each sample point:

1. reconstruct the same SPH support set used for interpolation
2. normalize the support weights
3. distribute `-F_sample` over the contributing SPH particles

This keeps the bridge conservative and avoids applying the entire rigid-body
reaction through a single DEM-center stencil.

## Output design

The current fix already exposes:

- a global vector
- a per-atom 14-column array

The embedded version should preserve backward compatibility where possible, but
extend output to expose distributed behavior.

Recommended additions:

### Global outputs

- number of coupled rigid bodies
- number of active samples
- total drag xyz
- total pressure-gradient xyz
- total buoyancy xyz
- total torque xyz
- summed effective immersed volume

### Per-body outputs

Retain:

- drag xyz
- pressure-gradient xyz
- buoyancy xyz
- total xyz

Add:

- torque xyz
- effective immersed fraction
- active sample count

### Optional debug output

For development, allow dumping sampler points as auxiliary particles or point
clouds with:

- sample id
- parent body id
- sample support flag
- local fluid density
- local pressure
- local force xyz

This will make ParaView debugging much easier than inferring everything from the
body center.

## User-facing syntax direction

The current syntax is:

```text
fix ID group-ID couple/sphdem fluid-group solid-group kernel-style h drag-coefficient
```

The embedded extension should avoid breaking this.

A likely extension path is optional keywords, for example:

```text
fix bridge all couple/sphdem fluid solid cubicspline 0.02 0.01 \
    sampler sphere_7 \
    torque yes \
    buoyancy embedded \
    reaction distributed
```

Possible sampler keywords:

- `sampler center`
- `sampler sphere_7`
- `sampler sphere_19`
- `sampler superquadric_surface`
- `sampler superquadric_volume`

This lets the current center-sample bridge remain available as the cheap mode.

## Shape roadmap

### Phase 1: spheres

Implement:

- embedded samples for `atom_style sphere`
- force accumulation
- torque accumulation
- distributed reaction

Validation targets:

- partially immersed floating sphere
- sinking sphere
- terminal settling in quiescent fluid
- off-center entry that should create torque

### Phase 2: superquadrics

Implement:

- sample generation in body coordinates
- orientation transform using quaternion state
- anisotropic torque response

Validation targets:

- rod-like superquadric entering a bath
- flat superquadric near a free surface
- orientation-dependent settling

### Phase 3: multisphere

Implement:

- body-level sampling for clumps
- force/torque accumulation onto the multisphere body

Validation targets:

- dumbbell clump entering a bath
- asymmetric clump with restoring torque

## Parallel and performance notes

The current bridge is explicitly single-rank only.

The embedded version will increase neighbor work, so parallel design matters.
Even before multi-rank support is implemented, we should structure the code so
that later communication is obvious:

- samples belong to local rigid bodies
- SPH contributors may be ghost particles
- per-sample reaction may need ghost-to-owner reduction
- body-level torque and force must remain deterministic

For early development, keep the fix single-rank, but do not hard-code data
structures in a way that blocks later ghost exchange.

## Recommended next coding steps

1. Refactor the current `fix couple/sphdem` into explicit helper stages.
2. Add a sphere sampler with 7 points and represented sample volumes.
3. Replace full-volume buoyancy with sample-volume buoyancy.
4. Accumulate torque and expose it in the fix outputs.
5. Add a debug dump mode for sampler points.
6. Add two focused tests:
   - floating sphere near free surface
   - off-center immersion generating nonzero torque
7. Only after that, extend to superquadrics.

## Bottom line

The embedded-sampler bridge is the right next architecture for Packfall.

It preserves the existing SPH solver and DEM integrators, but moves the
fluid-solid coupling from:

- "one force at the center of the particle"

to:

- "distributed SPH-informed forces over the rigid body volume"

That should fix the early-buoyancy artifact, make partial immersion behave much
more naturally, and give Packfall a credible path to SPH coupling for both
spheres and superquadrics.
