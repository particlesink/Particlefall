/* ----------------------------------------------------------------------
    This is the

    LIGGGHTS/Packfall source tree.
------------------------------------------------------------------------- */

#ifdef FIX_CLASS

FixStyle(couple/sphdem,FixCoupleSphDem)

#else

#ifndef LMP_FIX_COUPLE_SPHDEM_H
#define LMP_FIX_COUPLE_SPHDEM_H

#include "fix.h"

namespace LAMMPS_NS {

class FixCoupleSphDem : public Fix {
 public:
  FixCoupleSphDem(class LAMMPS *, int, char **);
  ~FixCoupleSphDem();

  int setmask();
  void init();
  void setup(int);
  void post_force(int);
  void post_force_respa(int, int, int);
  double compute_vector(int);
  void grow_arrays(int);
  void copy_arrays(int, int, int);
  int pack_exchange(int, double *);
  int unpack_exchange(int, double *);
  double memory_usage();

 private:
  int fluid_igroup_;
  int fluid_groupbit_;
  int solid_igroup_;
  int solid_groupbit_;
  int kernel_id_;
  int nlevels_respa_;
  int nmax_;
  class FixGravity *gravity_fix_;

  double smoothing_length_;
  double hinv_;
  double cutoff_;
  double drag_coefficient_;
  double stats_[15];
  double **bridge_atom_;

  void reset_stats();
  void apply_bridge();
  double solid_measure(int) const;
  void update_gravity(double *) const;
};

}

#endif
#endif
