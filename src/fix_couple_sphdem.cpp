/* ----------------------------------------------------------------------
    This is the

    LIGGGHTS/Packfall source tree.
------------------------------------------------------------------------- */

#include <cmath>
#include <cstring>
#include "fix_couple_sphdem.h"
#include "atom.h"
#include "group.h"
#include "update.h"
#include "respa.h"
#include "force.h"
#include "domain.h"
#include "modify.h"
#include "memory.h"
#include "error.h"
#include "mpi_liggghts.h"
#include "sph_kernels.h"
#include "fix_gravity.h"

using namespace LAMMPS_NS;
using namespace FixConst;

namespace {
const double SPHDEM_EPS = 1.0e-20;
const int SPHDEM_SAMPLE_COUNT = 7;

inline double sphdem_clamp(double value, double lo, double hi)
{
  if (value < lo) return lo;
  if (value > hi) return hi;
  return value;
}

inline bool sphdem_finite3(const double *v)
{
  return std::isfinite(v[0]) && std::isfinite(v[1]) && std::isfinite(v[2]);
}

inline void sphdem_cross3(const double *a, const double *b, double *out)
{
  out[0] = a[1]*b[2] - a[2]*b[1];
  out[1] = a[2]*b[0] - a[0]*b[2];
  out[2] = a[0]*b[1] - a[1]*b[0];
}
}

/* ---------------------------------------------------------------------- */

FixCoupleSphDem::FixCoupleSphDem(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg),
  fluid_igroup_(-1),
  fluid_groupbit_(0),
  solid_igroup_(-1),
  solid_groupbit_(0),
  kernel_id_(-1),
  nlevels_respa_(0),
  nmax_(0),
  gravity_fix_(0),
  smoothing_length_(0.0),
  hinv_(0.0),
  cutoff_(0.0),
  drag_coefficient_(0.0),
  bridge_atom_(0)
{
  if (narg != 8) error->all(FLERR,"Illegal fix couple/sphdem command");

  fluid_igroup_ = group->find(arg[3]);
  if (fluid_igroup_ < 0) error->all(FLERR,"Could not find fluid group ID for fix couple/sphdem");
  fluid_groupbit_ = group->bitmask[fluid_igroup_];

  solid_igroup_ = group->find(arg[4]);
  if (solid_igroup_ < 0) error->all(FLERR,"Could not find solid group ID for fix couple/sphdem");
  solid_groupbit_ = group->bitmask[solid_igroup_];

  if (fluid_igroup_ == solid_igroup_)
    error->all(FLERR,"Fix couple/sphdem requires distinct fluid and solid groups");

  if (SPH_KERNEL_NS::sph_kernels_unique_id() < 0)
    error->all(FLERR,"Cannot proceed, sph kernels need unique IDs");

  kernel_id_ = SPH_KERNEL_NS::sph_kernel_id(arg[5]);
  if (kernel_id_ < 0)
    error->all(FLERR,"Illegal fix couple/sphdem command, unknown sph kernel");

  smoothing_length_ = force->numeric(FLERR,arg[6]);
  if (smoothing_length_ <= 0.0)
    error->all(FLERR,"Fix couple/sphdem requires smoothing length > 0");

  drag_coefficient_ = force->numeric(FLERR,arg[7]);
  if (drag_coefficient_ < 0.0)
    error->all(FLERR,"Fix couple/sphdem requires drag coefficient >= 0");

  hinv_ = 1.0 / smoothing_length_;
  cutoff_ = smoothing_length_ * SPH_KERNEL_NS::sph_kernel_cut(kernel_id_);

  vector_flag = 1;
  size_vector = 15;
  global_freq = 1;
  extvector = 1;
  peratom_flag = 1;
  size_peratom_cols = 18;
  peratom_freq = 1;

  nmax_ = atom->nmax;
  memory->create(bridge_atom_,nmax_,size_peratom_cols,"couple/sphdem:bridge_atom");
  array_atom = bridge_atom_;
  atom->add_callback(0);
  atom->add_callback(1);

  reset_stats();
  const int nlocal = atom->nlocal;
  for (int i = 0; i < nlocal; ++i)
    for (int m = 0; m < size_peratom_cols; ++m)
      bridge_atom_[i][m] = 0.0;
}

/* ---------------------------------------------------------------------- */

FixCoupleSphDem::~FixCoupleSphDem()
{
  atom->delete_callback(id,0);
  atom->delete_callback(id,1);
  memory->destroy(bridge_atom_);
}

/* ---------------------------------------------------------------------- */

int FixCoupleSphDem::setmask()
{
  int mask = 0;
  mask |= POST_FORCE;
  mask |= POST_FORCE_RESPA;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::init()
{
  if (comm->nprocs != 1)
    error->all(FLERR,"Fix couple/sphdem currently supports single-processor runs only");

  if (!atom->rho_flag)
    error->all(FLERR,"Fix couple/sphdem requires an atom style with SPH density");
  if (!atom->rmass_flag || !atom->radius_flag)
    error->all(FLERR,"Fix couple/sphdem requires an atom style with per-particle mass and radius");

  if (strstr(update->integrate_style,"respa"))
    nlevels_respa_ = ((Respa *) update->integrate)->nlevels;

  gravity_fix_ = 0;
  for (int ifix = 0; ifix < modify->nfix; ++ifix) {
    Fix *fix = modify->fix[ifix];
    if (!fix) continue;
    if (strcmp(fix->style,"gravity") != 0) continue;
    if (!(fix->groupbit & solid_groupbit_)) continue;

    gravity_fix_ = static_cast<FixGravity *>(fix);
    break;
  }
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::setup(int vflag)
{
  if (strstr(update->integrate_style,"verlet"))
    post_force(vflag);
  else if (strstr(update->integrate_style,"respa")) {
    ((Respa *) update->integrate)->copy_flevel_f(nlevels_respa_-1);
    post_force_respa(vflag,nlevels_respa_-1,0);
    ((Respa *) update->integrate)->copy_f_flevel(nlevels_respa_-1);
  }
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::reset_stats()
{
  for (int i = 0; i < 15; ++i) stats_[i] = 0.0;
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::grow_arrays(int nmax)
{
  nmax_ = nmax;
  memory->grow(bridge_atom_,nmax_,size_peratom_cols,"couple/sphdem:bridge_atom");
  array_atom = bridge_atom_;
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::copy_arrays(int i, int j, int)
{
  for (int m = 0; m < size_peratom_cols; ++m)
    bridge_atom_[j][m] = bridge_atom_[i][m];
}

/* ---------------------------------------------------------------------- */

int FixCoupleSphDem::pack_exchange(int i, double *buf)
{
  for (int m = 0; m < size_peratom_cols; ++m)
    buf[m] = bridge_atom_[i][m];
  return size_peratom_cols;
}

/* ---------------------------------------------------------------------- */

int FixCoupleSphDem::unpack_exchange(int nlocal, double *buf)
{
  for (int m = 0; m < size_peratom_cols; ++m)
    bridge_atom_[nlocal][m] = buf[m];
  return size_peratom_cols;
}

/* ---------------------------------------------------------------------- */

double FixCoupleSphDem::memory_usage()
{
  return atom->nmax * size_peratom_cols * sizeof(double);
}

/* ---------------------------------------------------------------------- */

double FixCoupleSphDem::solid_measure(int i) const
{
  const double radius = atom->radius[i];
  if (domain->dimension == 2)
    return M_PI * radius * radius;
  return (4.0 / 3.0) * M_PI * radius * radius * radius;
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::apply_bridge()
{
  double **x = atom->x;
  double **v = atom->v;
  double **f = atom->f;
  double *rmass = atom->rmass;
  double *rho = atom->rho;
  double *p = atom->p;
  int *mask = atom->mask;
  int nlocal = atom->nlocal;
  double gravity[3] = {0.0, 0.0, 0.0};

  update_gravity(gravity);

  reset_stats();

  if (atom->nlocal > nmax_) {
    grow_arrays(atom->nmax);
  }

  for (int i = 0; i < nlocal; ++i)
    for (int m = 0; m < size_peratom_cols; ++m)
      bridge_atom_[i][m] = 0.0;

  for (int i = 0; i < nlocal; ++i) {
    if (!(mask[i] & groupbit) || !(mask[i] & solid_groupbit_))
      continue;

    const double solid_volume = solid_measure(i);
    const double sample_volume = solid_volume / static_cast<double>(SPHDEM_SAMPLE_COUNT);
    const double radius = atom->radius[i];
    const double sample_radius = 0.5 * radius;
    double omega_i[3] = {0.0, 0.0, 0.0};
    if (atom->omega_flag && atom->omega)
      for (int k = 0; k < 3; ++k) omega_i[k] = atom->omega[i][k];

    const double sample_offsets[SPHDEM_SAMPLE_COUNT][3] = {
      {0.0, 0.0, 0.0},
      {sample_radius, 0.0, 0.0},
      {-sample_radius, 0.0, 0.0},
      {0.0, sample_radius, 0.0},
      {0.0, -sample_radius, 0.0},
      {0.0, 0.0, sample_radius},
      {0.0, 0.0, -sample_radius}
    };

    double total_drag[3] = {0.0, 0.0, 0.0};
    double total_pressure_force[3] = {0.0, 0.0, 0.0};
    double total_buoyancy_force[3] = {0.0, 0.0, 0.0};
    double total_bridge_force[3] = {0.0, 0.0, 0.0};
    double total_bridge_torque[3] = {0.0, 0.0, 0.0};
    double porosity_accum = 0.0;
    double immersed_fraction = 0.0;
    int active_samples = 0;

    for (int isample = 0; isample < SPHDEM_SAMPLE_COUNT; ++isample) {
      const double *sample_offset = sample_offsets[isample];
      double sample_pos[3] = {
        x[i][0] + sample_offset[0],
        x[i][1] + sample_offset[1],
        x[i][2] + sample_offset[2]
      };
      double sample_rot_vel[3] = {0.0, 0.0, 0.0};
      double sample_vel[3] = {v[i][0], v[i][1], v[i][2]};
      sphdem_cross3(omega_i, sample_offset, sample_rot_vel);
      sample_vel[0] += sample_rot_vel[0];
      sample_vel[1] += sample_rot_vel[1];
      sample_vel[2] += sample_rot_vel[2];

      double weighted_velocity[3] = {0.0, 0.0, 0.0};
      double grad_weight_sum[3] = {0.0, 0.0, 0.0};
      double weighted_pressure = 0.0;
      double weighted_pressure_grad[3] = {0.0, 0.0, 0.0};
      double weighted_density = 0.0;
      double weight_sum = 0.0;

      for (int j = 0; j < nlocal; ++j) {
        if (!(mask[j] & groupbit) || !(mask[j] & fluid_groupbit_))
          continue;

        if (!sphdem_finite3(x[j]) || !sphdem_finite3(v[j]))
          continue;

        const double dx = sample_pos[0] - x[j][0];
        const double dy = sample_pos[1] - x[j][1];
        const double dz = sample_pos[2] - x[j][2];
        const double rsq = dx*dx + dy*dy + dz*dz;
        if (!std::isfinite(rsq))
          continue;
        const double r = sqrt(rsq);

        if (r >= cutoff_)
          continue;

        if (!std::isfinite(rho[j]) || rho[j] <= SPHDEM_EPS)
          continue;

        const double volume_j = rmass[j] / rho[j];
        if (!std::isfinite(volume_j))
          continue;
        const double kernel = SPH_KERNEL_NS::sph_kernel(kernel_id_, r, smoothing_length_, hinv_);
        const double weight = volume_j * kernel;
        if (!std::isfinite(weight))
          continue;
        const double grad_kernel_mag = SPH_KERNEL_NS::sph_kernel_der(kernel_id_, r, smoothing_length_, hinv_);
        if (!std::isfinite(grad_kernel_mag))
          continue;

        double grad_kernel[3] = {0.0, 0.0, 0.0};
        if (r > SPHDEM_EPS) {
          const double rinv = 1.0 / r;
          grad_kernel[0] = grad_kernel_mag * dx * rinv;
          grad_kernel[1] = grad_kernel_mag * dy * rinv;
          grad_kernel[2] = grad_kernel_mag * dz * rinv;
        }

        weight_sum += weight;
        weighted_velocity[0] += weight * v[j][0];
        weighted_velocity[1] += weight * v[j][1];
        weighted_velocity[2] += weight * v[j][2];
        weighted_density += weight * rho[j];
        grad_weight_sum[0] += volume_j * grad_kernel[0];
        grad_weight_sum[1] += volume_j * grad_kernel[1];
        grad_weight_sum[2] += volume_j * grad_kernel[2];

        if (p && std::isfinite(p[j])) {
          weighted_pressure += weight * p[j];
          weighted_pressure_grad[0] += volume_j * p[j] * grad_kernel[0];
          weighted_pressure_grad[1] += volume_j * p[j] * grad_kernel[1];
          weighted_pressure_grad[2] += volume_j * p[j] * grad_kernel[2];
        }
      }

      if (!std::isfinite(weight_sum) || weight_sum <= SPHDEM_EPS)
        continue;

      const double inv_weight_sum = 1.0 / weight_sum;
      const double fill_fraction = sphdem_clamp(weight_sum, 0.0, 1.0);
      const double porosity = sphdem_clamp(weight_sum, 0.05, 1.0);
      const double effective_sample_volume = sample_volume * fill_fraction;

      weighted_velocity[0] *= inv_weight_sum;
      weighted_velocity[1] *= inv_weight_sum;
      weighted_velocity[2] *= inv_weight_sum;
      weighted_density *= inv_weight_sum;

      if (!sphdem_finite3(weighted_velocity) || !std::isfinite(weighted_density))
        continue;

      double pressure_gradient[3] = {0.0, 0.0, 0.0};
      if (p) {
        weighted_pressure *= inv_weight_sum;
        pressure_gradient[0] = (weighted_pressure_grad[0] - weighted_pressure * grad_weight_sum[0]) * inv_weight_sum;
        pressure_gradient[1] = (weighted_pressure_grad[1] - weighted_pressure * grad_weight_sum[1]) * inv_weight_sum;
        pressure_gradient[2] = (weighted_pressure_grad[2] - weighted_pressure * grad_weight_sum[2]) * inv_weight_sum;
        if (!sphdem_finite3(pressure_gradient))
          continue;
      }

      const double scale = drag_coefficient_ * sample_volume / porosity;
      const double drag[3] = {
        scale * (weighted_velocity[0] - sample_vel[0]),
        scale * (weighted_velocity[1] - sample_vel[1]),
        scale * (weighted_velocity[2] - sample_vel[2])
      };
      const double pressure_force[3] = {
        -effective_sample_volume * pressure_gradient[0],
        -effective_sample_volume * pressure_gradient[1],
        -effective_sample_volume * pressure_gradient[2]
      };
      const double buoyancy_force[3] = {
        -weighted_density * effective_sample_volume * gravity[0],
        -weighted_density * effective_sample_volume * gravity[1],
        -weighted_density * effective_sample_volume * gravity[2]
      };
      const double bridge_force[3] = {
        drag[0] + pressure_force[0] + buoyancy_force[0],
        drag[1] + pressure_force[1] + buoyancy_force[1],
        drag[2] + pressure_force[2] + buoyancy_force[2]
      };
      double bridge_torque[3] = {0.0, 0.0, 0.0};
      sphdem_cross3(sample_offset, bridge_force, bridge_torque);

      total_drag[0] += drag[0];
      total_drag[1] += drag[1];
      total_drag[2] += drag[2];
      total_pressure_force[0] += pressure_force[0];
      total_pressure_force[1] += pressure_force[1];
      total_pressure_force[2] += pressure_force[2];
      total_buoyancy_force[0] += buoyancy_force[0];
      total_buoyancy_force[1] += buoyancy_force[1];
      total_buoyancy_force[2] += buoyancy_force[2];
      total_bridge_force[0] += bridge_force[0];
      total_bridge_force[1] += bridge_force[1];
      total_bridge_force[2] += bridge_force[2];
      total_bridge_torque[0] += bridge_torque[0];
      total_bridge_torque[1] += bridge_torque[1];
      total_bridge_torque[2] += bridge_torque[2];
      porosity_accum += porosity;
      immersed_fraction += fill_fraction / static_cast<double>(SPHDEM_SAMPLE_COUNT);
      ++active_samples;

      for (int j = 0; j < nlocal; ++j) {
        if (!(mask[j] & groupbit) || !(mask[j] & fluid_groupbit_))
          continue;

        if (!sphdem_finite3(x[j]) || !std::isfinite(rho[j]) || rho[j] <= SPHDEM_EPS)
          continue;

        const double dx = sample_pos[0] - x[j][0];
        const double dy = sample_pos[1] - x[j][1];
        const double dz = sample_pos[2] - x[j][2];
        const double rsq = dx*dx + dy*dy + dz*dz;
        if (!std::isfinite(rsq))
          continue;
        const double r = sqrt(rsq);

        if (r >= cutoff_)
          continue;

        const double volume_j = rmass[j] / rho[j];
        if (!std::isfinite(volume_j))
          continue;
        const double kernel = SPH_KERNEL_NS::sph_kernel(kernel_id_, r, smoothing_length_, hinv_);
        const double weight = volume_j * kernel * inv_weight_sum;
        if (!std::isfinite(weight))
          continue;

        f[j][0] -= bridge_force[0] * weight;
        f[j][1] -= bridge_force[1] * weight;
        f[j][2] -= bridge_force[2] * weight;
      }
    }

    if (active_samples == 0)
      continue;

    const double mean_porosity = porosity_accum / static_cast<double>(active_samples);

    f[i][0] += total_bridge_force[0];
    f[i][1] += total_bridge_force[1];
    f[i][2] += total_bridge_force[2];
    if (atom->torque_flag && atom->torque) {
      atom->torque[i][0] += total_bridge_torque[0];
      atom->torque[i][1] += total_bridge_torque[1];
      atom->torque[i][2] += total_bridge_torque[2];
    }

    bridge_atom_[i][0] = total_drag[0];
    bridge_atom_[i][1] = total_drag[1];
    bridge_atom_[i][2] = total_drag[2];
    bridge_atom_[i][3] = total_pressure_force[0];
    bridge_atom_[i][4] = total_pressure_force[1];
    bridge_atom_[i][5] = total_pressure_force[2];
    bridge_atom_[i][6] = total_buoyancy_force[0];
    bridge_atom_[i][7] = total_buoyancy_force[1];
    bridge_atom_[i][8] = total_buoyancy_force[2];
    bridge_atom_[i][9] = total_bridge_force[0];
    bridge_atom_[i][10] = total_bridge_force[1];
    bridge_atom_[i][11] = total_bridge_force[2];
    bridge_atom_[i][12] = mean_porosity;
    bridge_atom_[i][13] = 1.0;
    bridge_atom_[i][14] = total_bridge_torque[0];
    bridge_atom_[i][15] = total_bridge_torque[1];
    bridge_atom_[i][16] = total_bridge_torque[2];
    bridge_atom_[i][17] = immersed_fraction;

    stats_[0] += 1.0;
    stats_[1] += total_drag[0];
    stats_[2] += total_drag[1];
    stats_[3] += total_drag[2];
    stats_[4] += total_pressure_force[0];
    stats_[5] += total_pressure_force[1];
    stats_[6] += total_pressure_force[2];
    stats_[7] += mean_porosity;
    stats_[8] += total_buoyancy_force[0];
    stats_[9] += total_buoyancy_force[1];
    stats_[10] += total_buoyancy_force[2];
    stats_[11] += total_bridge_torque[0];
    stats_[12] += total_bridge_torque[1];
    stats_[13] += total_bridge_torque[2];
    stats_[14] += immersed_fraction;
  }
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::update_gravity(double *gravity) const
{
  gravity[0] = 0.0;
  gravity[1] = 0.0;
  gravity[2] = 0.0;

  if (gravity_fix_) {
    gravity_fix_->get_gravity(gravity);
    return;
  }

  for (int ifix = 0; ifix < modify->nfix; ++ifix) {
    Fix *fix = modify->fix[ifix];
    if (!fix) continue;
    if (strcmp(fix->style,"gravity") != 0) continue;
    if (!(fix->groupbit & solid_groupbit_)) continue;

    static_cast<FixGravity *>(fix)->get_gravity(gravity);
    return;
  }
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::post_force(int)
{
  apply_bridge();
}

/* ---------------------------------------------------------------------- */

void FixCoupleSphDem::post_force_respa(int vflag, int ilevel, int)
{
  if (ilevel == nlevels_respa_ - 1)
    post_force(vflag);
}

/* ---------------------------------------------------------------------- */

double FixCoupleSphDem::compute_vector(int n)
{
  double value = stats_[n];
  MPI_Sum_Scalar(value,world);
  return value;
}
