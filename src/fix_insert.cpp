/* ----------------------------------------------------------------------
    This is the

    ██╗     ██╗ ██████╗  ██████╗  ██████╗ ██╗  ██╗████████╗███████╗
    ██║     ██║██╔════╝ ██╔════╝ ██╔════╝ ██║  ██║╚══██╔══╝██╔════╝
    ██║     ██║██║  ███╗██║  ███╗██║  ███╗███████║   ██║   ███████╗
    ██║     ██║██║   ██║██║   ██║██║   ██║██╔══██║   ██║   ╚════██║
    ███████╗██║╚██████╔╝╚██████╔╝╚██████╔╝██║  ██║   ██║   ███████║
    ╚══════╝╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝®

    DEM simulation engine, released by
    DCS Computing Gmbh, Linz, Austria
    http://www.dcs-computing.com, office@dcs-computing.com

    LIGGGHTS® is part of CFDEM®project:
    http://www.liggghts.com | http://www.cfdem.com

    Core developer and main author:
    Christoph Kloss, christoph.kloss@dcs-computing.com

    LIGGGHTS® is open-source, distributed under the terms of the GNU Public
    License, version 2 or later. It is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. You should have
    received a copy of the GNU General Public License along with LIGGGHTS®.
    If not, see http://www.gnu.org/licenses . See also top-level README
    and LICENSE files.

    LIGGGHTS® and CFDEM® are registered trade marks of DCS Computing GmbH,
    the producer of the LIGGGHTS® software and the CFDEM®coupling software
    See http://www.cfdem.com/terms-trademark-policy for details.

-------------------------------------------------------------------------
    Contributing author and copyright for this file:
    Christoph Kloss (JKU Linz, DCS Computing GmbH, Linz)
    Richard Berger (JKU Linz)

    Copyright 2012-     DCS Computing GmbH, Linz
    Copyright 2009-2015 JKU Linz
------------------------------------------------------------------------- */

#include <cmath>
#include <algorithm>
#include <limits>
#include <stdlib.h>
#include <string.h>
#include "atom.h"
#include "atom_vec.h"
#include "force.h"
#include "update.h"
#include "comm.h"
#include "modify.h"
#include "domain.h"
#include "random_park.h"
#include "memory.h"
#include "error.h"
#include "fix_multisphere.h"
#include "fix_particledistribution_discrete.h"
#include "fix_template_sphere.h"
#include "fix_property_atom.h"
#include "irregular.h"
#include "fix_insert.h"
#include "fix_mesh_surface.h"
#include "tri_mesh.h"
#include "math_extra_liggghts.h"
#include "mpi_liggghts.h"
#include "vector_liggghts.h"

#include "probability_distribution.h"
#include "region_neighbor_list.h"

using namespace LAMMPS_NS;
using namespace FixConst;

#define EPSILON 0.001

#define LMP_DEBUGMODE_FIXINSERT false //(667 == update->ntimestep)//  true
#define LMP_DEBUG_OUT_FIXINSERT screen

namespace {

bool ray_intersects_triangle(const double *origin, const double *direction,
                             const double *v0, const double *v1, const double *v2,
                             const double eps, double &t)
{
  double edge1[3], edge2[3], pvec[3], tvec[3], qvec[3];
  vectorSubtract3D(v1, v0, edge1);
  vectorSubtract3D(v2, v0, edge2);
  vectorCross3D(direction, edge2, pvec);

  const double det = vectorDot3D(edge1, pvec);
  if(fabs(det) <= eps) return false;

  const double inv_det = 1.0 / det;

  vectorSubtract3D(origin, v0, tvec);
  const double u = vectorDot3D(tvec, pvec) * inv_det;
  if(u < -eps || u > 1.0 + eps) return false;

  vectorCross3D(tvec, edge1, qvec);
  const double v = vectorDot3D(direction, qvec) * inv_det;
  if(v < -eps || u + v > 1.0 + eps) return false;

  t = vectorDot3D(edge2, qvec) * inv_det;
  return t > eps;
}

}

/* ---------------------------------------------------------------------- */

FixInsert::FixInsert(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg),
  neighList(*new RegionNeighborList<interpolate_no>(lmp))
{
  if (narg < 7) error->fix_error(FLERR,this,"not enough arguments");

  restart_global = 1;

  setup_flag = false;

  fix_distribution = NULL;
  fix_multisphere = NULL;
  multisphere = NULL;
  recvcounts = NULL;
  displs = NULL;
  irregular = NULL;
  constructor_finalized_ = false;

  compress_flag = false ;

  // required args
  iarg = 3;

  if(strcmp(arg[iarg++],"seed")) error->fix_error(FLERR,this,"expecting keyword 'seed'");
  // random number generator, seed depends on proc
  random = new RanPark(lmp, arg[iarg++], true);
  seed = random->getSeed();
  if (seed <= 0) error->fix_error(FLERR,this,"illegal seed");

  // set defaults
  init_defaults();
}

/* ---------------------------------------------------------------------- */

bool FixInsert::parse_base_keyword(int narg, char **arg)
{
  if(parse_shared_keyword(narg,arg))
    return true;

#ifdef SUPERQUADRIC_ACTIVE_FLAG
  if (strcmp(arg[iarg],"check_obb") == 0) {
    if (iarg+2 > narg) error->fix_error(FLERR,this,"");
    if(strcmp(arg[iarg+1],"yes")==0) check_obb_flag = 1;
    else if(strcmp(arg[iarg+1],"no")==0) check_obb_flag = 0;
    else error->fix_error(FLERR,this,"");
    if(check_ol_flag==0) check_obb_flag = 0;
    iarg += 2;
    return true;
  }
#endif

  return false;
}

/* ---------------------------------------------------------------------- */

bool FixInsert::parse_shared_keyword(int narg, char **arg)
{
  if(strcmp(arg[iarg],"distributiontemplate") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      int ifix = modify->find_fix(arg[iarg+1]);
      if(ifix < 0 || strncmp(modify->fix[ifix]->style,"particledistribution/discrete",29))
        error->fix_error(FLERR,this,"Fix insert requires you to define a valid ID for a fix of type particledistribution/discrete");
      fix_distribution = static_cast<FixParticledistributionDiscrete*>(modify->fix[ifix]);
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"maxattempt") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      maxattempt = atoi(arg[iarg+1]);
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"nparticles") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg+1],"INF") == 0)
        ninsert_exists = 0;
      else ninsert = atof(arg[iarg+1]);
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"mass") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg+1],"INF") == 0)
        ninsert_exists = 0;
      else massinsert = atof(arg[iarg+1]);
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"massrate") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      massflowrate = atof(arg[iarg+1]);
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"particlerate") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      nflowrate = atof(arg[iarg+1]);
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"insert_every_time") == 0 || strcmp(arg[iarg],"insert_every") == 0 || strcmp(arg[iarg],"every") == 0 || strcmp(arg[iarg],"interval") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg+1],"once") == 0) insert_every = 0;
      else if(strcmp(arg[iarg],"insert_every_time") == 0 || strcmp(arg[iarg],"interval") == 0)
      {
          insert_every = time_to_step(atof(arg[iarg+1]),true);
      }
      else
          insert_every = atoi(arg[iarg+1]);
      if(insert_every < 0) error->fix_error(FLERR,this,"insert_every must be >= 0");
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"start") == 0 || strcmp(arg[iarg],"start_time") == 0 || strcmp(arg[iarg],"t_zero") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg],"start") == 0)
        first_ins_step = atoi(arg[iarg+1]);
      else
      {
        first_ins_step = time_to_step(atof(arg[iarg+1]),false);
        const int min_start = modify->fix_restart_in_progress() ? update->ntimestep : update->ntimestep + 1;
        if(first_ins_step < min_start)
          first_ins_step = min_start;
      }
      if(first_ins_step < update->ntimestep + 1 && !modify->fix_restart_in_progress())
        error->fix_error(FLERR,this,"'start' step can not be before current step");
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"stop_time") == 0 || strcmp(arg[iarg],"t_last") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      last_ins_step_ = time_to_step(atof(arg[iarg+1]),false);
      last_ins_step_set_ = true;
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"overlapcheck") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg+1],"yes")==0) check_ol_flag = 1;
      else if(strcmp(arg[iarg+1],"no")==0) check_ol_flag = 0;
      else error->fix_error(FLERR,this,"");
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"all_in") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg+1],"yes")==0) all_in_flag = 1;
      else if(strcmp(arg[iarg+1],"no")==0) all_in_flag = 0;
      else error->fix_error(FLERR,this,"");
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"set_property") == 0) {
      if (iarg+3 > narg) error->fix_error(FLERR,this,"");
      int n = strlen(arg[iarg+1]) + 1;
      property_name = new char[n];
      strcpy(property_name,arg[iarg+1]);
      fix_property_value = force->numeric(FLERR,arg[iarg+2]);
      iarg += 3;
      return true;
    } else if (strcmp(arg[iarg],"random_distribute") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg+1],"uncorrelated")==0) exact_number = 0;
      else if(strcmp(arg[iarg+1],"exact")==0) exact_number = 1;
      else error->fix_error(FLERR,this,"");
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"verbose") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"");
      if(strcmp(arg[iarg+1],"no")==0) print_stats_during_flag = 0;
      else if(strcmp(arg[iarg+1],"yes")==0) print_stats_during_flag = 1;
      else error->fix_error(FLERR,this,"");
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"compress_tags") == 0) {
      if (iarg+2 > narg) error->fix_error(FLERR,this,"not enough arguments for compress_tags");
      if(0 == strcmp(arg[iarg+1],"yes"))
        compress_flag = true;
      else if(0 == strcmp(arg[iarg+1],"no"))
        compress_flag = false;
      else
        error->fix_error(FLERR,this,"expecting 'yes' or 'no' after 'compress_tags'");
      iarg += 2;
      return true;
    } else if (strcmp(arg[iarg],"vel") == 0) {
      if (iarg+5 > narg) error->fix_error(FLERR,this,"not enough keyword for 'vel'");
      if (strcmp(arg[iarg+1],"constant") == 0)  {
          v_insert[0] = atof(arg[iarg+2]);
          v_insert[1] = atof(arg[iarg+3]);
          v_insert[2] = atof(arg[iarg+4]);
          iarg += 5;
      } else if (strcmp(arg[iarg+1],"uniform") == 0) {
          if (iarg+8 > narg) error->fix_error(FLERR,this,"not enough keyword for 'uniform'");
          v_randomSetting = RANDOM_UNIFORM;
          v_insert[0] = atof(arg[iarg+2]);
          v_insert[1] = atof(arg[iarg+3]);
          v_insert[2] = atof(arg[iarg+4]);
          v_insertFluct[0] = atof(arg[iarg+5]);
          v_insertFluct[1] = atof(arg[iarg+6]);
          v_insertFluct[2] = atof(arg[iarg+7]);
          iarg += 8;
      } else if (strcmp(arg[iarg+1],"gaussian") == 0) {
          if (iarg+8 > narg) error->fix_error(FLERR,this,"not enough keyword for 'gaussian'");
          v_randomSetting = RANDOM_GAUSSIAN;
          v_insert[0] = atof(arg[iarg+2]);
          v_insert[1] = atof(arg[iarg+3]);
          v_insert[2] = atof(arg[iarg+4]);
          v_insertFluct[0] = atof(arg[iarg+5]);
          v_insertFluct[1] = atof(arg[iarg+6]);
          v_insertFluct[2] = atof(arg[iarg+7]);
          iarg += 8;
      } else
          error->fix_error(FLERR,this,"expecting keyword 'constant' or 'uniform' or 'gaussian' after keyword 'vel'");
      return true;
    } else if (strcmp(arg[iarg],"omega") == 0) {
      if (iarg+5 > narg) error->fix_error(FLERR,this,"");
      if (strcmp(arg[iarg+1],"constant") == 0)
      {
          omega_insert[0] = atof(arg[iarg+2]);
          omega_insert[1] = atof(arg[iarg+3]);
          omega_insert[2] = atof(arg[iarg+4]);
      } else error->fix_error(FLERR,this,"expecting keyword 'constant' after keyword 'omega'");
      iarg += 5;
      return true;
    } else if (strcmp(arg[iarg],"orientation") == 0) {
      if (iarg+2 > narg)
        error->fix_error(FLERR,this,"not enough arguments for 'orientation'");
      iarg++;
      if(strcmp(arg[iarg],"random") == 0)
      {
          quat_random_ = true;
          iarg++;
      }
      else if(strcmp(arg[iarg],"template") == 0)
      {
          quat_random_ = false;
          iarg++;
      }
      else if (strcmp(arg[iarg],"constant") == 0)
      {
          iarg++;
          if (iarg+4 > narg) error->fix_error(FLERR,this,"");
          quat_insert[0] = atof(arg[iarg++]);
          quat_insert[1] = atof(arg[iarg++]);
          quat_insert[2] = atof(arg[iarg++]);
          quat_insert[3] = atof(arg[iarg++]);
      } else error->fix_error(FLERR,this,"expecting 'random', template' or 'constant' after keyword 'quat'");
      return true;
    } else if (parse_inclusion_keyword(narg,arg)) {
      return true;
    }

  return false;
}

/* ---------------------------------------------------------------------- */

void FixInsert::finalize_constructor_setup()
{
  if(constructor_finalized_)
    return;

  // memory not allocated initially
  ninsert_this_max_local = 0;

  // check for missing or contradictory settings
  sanity_check();

  //min/max type to be inserted, need that to check if material properties defined for all materials
  type_max = fix_distribution->max_type();
  type_min = fix_distribution->min_type();

  // allgather arrays
  MPI_Comm_rank(world,&me);
  MPI_Comm_size(world,&nprocs);
  recvcounts = new int[nprocs];
  displs = new int[nprocs];

  // set next reneighbor
  force_reneighbor = 1;
  next_reneighbor = first_ins_step;
  most_recent_ins_step = -1;

  vector_flag = 1;
  size_vector = 2;
  global_freq = 1;

  print_stats_start_flag = 1;

  irregular = new Irregular(lmp);

  // calc max insertion radius
  int ntypes = atom->ntypes;
  maxrad = 0.;
  minrad = 1000.;
  for(int i = 1; i <= ntypes; i++)
  {
     maxrad = std::max(maxrad,max_rad(i));
     minrad = std::min(minrad,min_rad(i));
  }

  constructor_finalized_ = true;
}

/* ---------------------------------------------------------------------- */

FixInsert::~FixInsert()
{
  delete random;
  delete [] recvcounts;
  delete [] displs;
  delete &neighList;
  if(property_name) delete []property_name;
  if(mesh_filter_id_) delete []mesh_filter_id_;

  if(irregular) delete irregular;
  irregular = 0;
}

/* ---------------------------------------------------------------------- */

void FixInsert::setup(int vflag)
{
  
  // do this only once
  if(setup_flag) return;
  else setup_flag = true;

  // calculate ninsert, insert_every, ninsert_per
  calc_insertion_properties();

  // calc last step of insertion
  if(ninsert_exists)
  {
      if(ninsert <= ninsert_per)
        final_ins_step = first_ins_step;
      else
        final_ins_step = first_ins_step +
                static_cast<int>(static_cast<double>(ninsert)/ninsert_per) *  static_cast<double>(insert_every);

      if(final_ins_step < 0)
        error->fix_error(FLERR,this,"Particle insertion: Overflow - need too long for particle insertion. "
                                    "Please decrease # particles to insert or increase insertion rate");
      if(ninsert < 0)
        error->fix_error(FLERR,this,"Particle insertion: Overflow - too many particles for particle insertion. "
                                    "Please decrease # particles to insert.");
  }
  else
    final_ins_step = -1;

  // print statistics
  print_stats_start();

}

/* ---------------------------------------------------------------------- */

void FixInsert::init_defaults()
{
  // default is that total # of particles to insert by this command is known
  ninsert_exists = 1;

  ninsert = ninserted = 0;
  massinsert = massinserted = 0.;
  nflowrate = massflowrate = 0.;

  insert_every = -1;
  ninsert_per = 0.;

  // 1st insertion on next timestep is default
  first_ins_step = update->ntimestep + 1;

  maxattempt = 50;

  check_ol_flag = 1;
  all_in_flag = 0;
#ifdef SUPERQUADRIC_ACTIVE_FLAG
  check_obb_flag = 1;
#endif

  exact_number = 1;

  v_randomSetting = RANDOM_CONSTANT;
  vectorZeroize3D(v_insert);
  vectorZeroize3D(v_insertFluct);
  vectorZeroize3D(omega_insert);

  quatIdentity4D(quat_insert);
  quat_random_ = false;

  print_stats_during_flag = 1;
  warn_boxentent = true;

  property_name = 0;
  fix_property = 0;
  fix_property_value = 0.;

  mesh_filter_id_ = 0;
  mesh_filter_fix_ = 0;
  mesh_filter_mesh_ = 0;
  mesh_filter_enabled_ = false;
  mesh_filter_inside_ = true;
  mesh_filter_axis_[0] = 1;
  mesh_filter_axis_[1] = 1;
  mesh_filter_axis_[2] = 1;
  last_ins_step_set_ = false;
  last_ins_step_ = std::numeric_limits<int>::max();
}

/* ---------------------------------------------------------------------- */

void FixInsert::sanity_check()
{
    if(fix_distribution == NULL)
        error->fix_error(FLERR,this,"have to define a 'distributiontemplate'");

    if(MathExtraLiggghts::abs(vectorMag4DSquared(quat_insert)-1.) > 1e-10)
        error->fix_error(FLERR,this,"quaternion not valid");

    if(ninsert > 0 && massinsert > 0.)
        error->fix_error(FLERR,this,"must not define both 'nparticles' and 'mass'");
    if(nflowrate > 0. && massflowrate > 0.)
        error->fix_error(FLERR,this,"must not define both 'particlerate' and 'massrate'");

    if(insert_every == 0 && (massflowrate > 0. || nflowrate > 0.))
        error->fix_error(FLERR,this,"must not define 'particlerate' or 'massrate' for 'insert_every' = 0");
    if(last_ins_step_set_ && last_ins_step_ < first_ins_step)
        error->fix_error(FLERR,this,"'stop_time' / 't_last' must not be before the insertion start time");

    if(0 == comm->me)
    {
        
        std::vector<int> seeds;
        seeds.push_back(random->state());
        seeds.push_back(fix_distribution->random_state());
        for(int itemplate = 0; itemplate < fix_distribution->n_particletemplates(); itemplate++)
        {
            
            seeds.push_back(fix_distribution->particletemplates()[itemplate]->random_insertion_state());
        }

        std::sort(seeds.begin(),seeds.end());

        if(std::unique(seeds.begin(),seeds.end()) !=seeds.end() )
        {
            char errstr[1024];
            sprintf(errstr,"Fix %s, ID %s: Random number generation: It is required that all the random seeds of this fix insert/*, \n"
                           "  the random seed of particle distribution fix (id %s) template and all random seeds of the \n"
                           "  fix particletemplate/* commands used by particle distribution fix (id %s) are different\n"
                           "  Hint: possible valid (different) seeds would be the following numbers:\n"
                           "        15485863, 15485867, 32452843, 32452867, 49979687, 49979693, 67867967, 67867979, 86028121, 86028157",
                           style,id,fix_distribution->id,fix_distribution->id);

            if(input->seed_check_throw_error())
                error->one(FLERR,errstr);
            else
                error->warning(FLERR,errstr);
        }
    }
}

/* ---------------------------------------------------------------------- */

void FixInsert::print_stats_start()
{
  if (me == 0 && print_stats_start_flag) {

    if(ninsert_exists)
    {
        if (screen)
            fprintf(screen ,"INFO: Particle insertion %s: %f particles every %d steps - particle rate %f  (mass rate %e)\n"
                            "      %d particles (mass %e) within %d steps\n",
                id,ninsert_per,insert_every,nflowrate,massflowrate,ninsert,massinsert,final_ins_step-first_ins_step);

        if (logfile)
            fprintf(logfile,"INFO: Particle insertion %s: %f particles every %d steps - particle rate %f, (mass rate %e)\n"
                            "      %d particles (mass %e) within %d steps\n",
                id,ninsert_per,insert_every,nflowrate,massflowrate,ninsert,massinsert,final_ins_step-first_ins_step);
    }
    else if(massflowrate > 0.)
    {
        if (screen)
            fprintf(screen ,"INFO: Particle insertion %s: %f particles every %d steps - particle rate %f  (mass rate %e)\n",
                id,ninsert_per,insert_every,nflowrate,massflowrate);

        if (logfile)
            fprintf(logfile,"INFO: Particle insertion %s: %f particles every %d steps - particle rate %f, (mass rate %e)\n",
                id,ninsert_per,insert_every,nflowrate,massflowrate);
    }
    else
    {
        if (screen)
            fprintf(screen ,"INFO: Particle insertion %s: inserting every %d steps\n",id,insert_every);

        if (logfile)
            fprintf(logfile ,"INFO: Particle insertion %s: inserting every %d steps\n",id,insert_every);
    }
  }
}

/* ---------------------------------------------------------------------- */

void FixInsert::print_stats_during(int ninsert_this, double mass_inserted_this)
{
  bigint step = update->ntimestep;

  if (me == 0 && print_stats_during_flag)
  {
    if (screen)
      fprintf(screen ,"INFO: Particle insertion %s: inserted %d particle templates (mass %e) at step " BIGINT_FORMAT "\n"
                      " - a total of %d particle templates (mass %e) inserted so far.\n",
              id,ninsert_this,mass_inserted_this,step,ninserted,massinserted);

    if (logfile)
      fprintf(logfile,"INFO: Particle insertion %s: inserted %d particle templates (mass %e) at step " BIGINT_FORMAT "\n"
                      " - a total of %d particle templates (mass %e) inserted so far.\n",
              id,ninsert_this,mass_inserted_this,step,ninserted,massinserted);
  }
}

/* ---------------------------------------------------------------------- */

int FixInsert::setmask()
{
  int mask = 0;
  mask |= PRE_EXCHANGE;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixInsert::init()
{
    int ntimestep = update->ntimestep;

    if (!atom->radius_flag || !atom->rmass_flag)
        error->fix_error(FLERR,this,"Fix insert requires atom attributes radius, rmass");
    if (domain->triclinic)
        error->fix_error(FLERR,this,"Cannot use with triclinic box");
    if (domain->dimension != 3)
        error->fix_error(FLERR,this,"Can use fix insert for 3d simulations only");
    
    fix_multisphere = static_cast<FixMultisphere*>(modify->find_fix_style("multisphere", 0));
    if(!fix_multisphere) multisphere = NULL;
    else multisphere = &fix_multisphere->data();

    // in case of new fix insert in a restarted simulation, have to add current time-step
    if(next_reneighbor > 0 && next_reneighbor < ntimestep)
    {
        
        error->fix_error(FLERR,this,"'start' step can not be before current step");
    }

    if(property_name)
    {
         fix_property = static_cast<FixPropertyAtom*>(modify->find_fix_property(property_name,"property/atom","scalar",1,1,this->style,true));
    }

    resolve_mesh_filter();
}

/* ---------------------------------------------------------------------- */

void FixInsert::reset_timestep(bigint newstep,bigint oldstep)
{
    
    next_reneighbor += (newstep-oldstep);
    
}

/* ---------------------------------------------------------------------- */

int FixInsert::min_type()
{
    return type_min;
}

/* ---------------------------------------------------------------------- */

int FixInsert::max_type()
{
    return type_max;
}

/* ---------------------------------------------------------------------- */

double FixInsert::max_rad(int type)
{
    return fix_distribution->max_rad(type);
}

/* ---------------------------------------------------------------------- */

double FixInsert::min_rad(int type)
{
    return fix_distribution->min_rad(type);
}

/* ---------------------------------------------------------------------- */

double FixInsert::max_r_bound()
{
    return fix_distribution->max_r_bound();
}

/* ---------------------------------------------------------------------- */

double FixInsert::extend_cut_ghost()
{
    
    if(!fix_multisphere)
        return 0.;

    return 2.*fix_distribution->max_r_bound();
}

/* ---------------------------------------------------------------------- */

int FixInsert::calc_ninsert_this()
{
  if(ninsert_per == 0.) error->fix_error(FLERR,this,"ninsert_per == 0.");

  // number of bodies to insert this timestep
  int ninsert_this = static_cast<int>(ninsert_per + random->uniform());
  if (ninsert_exists && ninserted + ninsert_this > ninsert) ninsert_this = ninsert - ninserted;

  return ninsert_this;
}

/* ----------------------------------------------------------------------
   perform particle insertion
------------------------------------------------------------------------- */

void FixInsert::pre_exchange()
{
  
  int ninsert_this, ninsert_this_local; // global and local # bodies to insert this time-step

  // just return if should not be called on this timestep
  
  if (next_reneighbor != update->ntimestep || most_recent_ins_step == update->ntimestep) return;
  if (last_ins_step_set_ && update->ntimestep > last_ins_step_)
  {
    next_reneighbor = 0;
    return;
  }
  most_recent_ins_step = update->ntimestep;

  // things to be done before inserting new particles
  
  if(!pre_insert())
    return;

  // number of particles to insert this timestep
  ninsert_this = calc_ninsert_this();
  
  // limit to max number of particles that shall be inserted
  // to avoid that max # may be slightly exceeded by random processes
  // in fix_distribution->randomize_list, set exact_number to 1
  if (ninsert_exists && ninserted + ninsert_this >= ninsert)
  {
      ninsert_this = ninsert - ninserted;
      if(ninsert_this < 0)
        ninsert_this = 0;
      exact_number = 1;
  }

  // distribute ninsert_this across processors
  ninsert_this_local = distribute_ninsert_this(ninsert_this);
  
  // re-allocate list if necessary
  
  if(ninsert_this_local > ninsert_this_max_local)
  {
      init_list(ninsert_this_local);
      ninsert_this_max_local = ninsert_this_local;
  }

  // generate list of insertions
  // number of inserted particles can change if exact_number = 0
  
  ninsert_this_local = generate_list(ninsert_this_local,groupbit,exact_number);
  
  MPI_Sum_Scalar(ninsert_this_local,ninsert_this,world);

  if(ninsert_this == 0)
  {
      // warn if flowrate should be fulfilled
      if((nflowrate > 0. || massflowrate > 0.) && warn_on_zero_insertion() && comm->me == 0)
        error->warning(FLERR,"Particle insertion: Inserting no particle - check particle insertion settings");

      // schedule next insertion
      if (insert_every && (!ninsert_exists || ninserted < ninsert))
        next_reneighbor += insert_every;
      
      else if(0 == insert_every)
        next_reneighbor = -1;

      if(last_ins_step_set_ && next_reneighbor > last_ins_step_)
        next_reneighbor = 0;

      return;
  }
  else if(ninsert_this < 0)
  {
      
      error->fix_error(FLERR,this,"Particle insertion: Internal error");
  }

  double min_subbox_extent;
  int min_dim;
  domain->min_subbox_extent(min_subbox_extent,min_dim);

  if(warn_boxentent && min_subbox_extent < 2.2 *max_r_bound())
  {
      char msg[256];
      sprintf(msg,"Particle insertion on proc %d: sub-domain is smaller than the bounding radius of insert particles to insert: \nMax. bounding "
                  "sphere diameter is %f, sub-domain extent in %s direction is only %f ",
                  comm->me,2.*max_r_bound(),0==min_dim?"x":(1==min_dim?"y":"z"),min_subbox_extent);
      error->warning(FLERR,msg);
  }

  // warn if max # insertions exceeded by random processes
  if (ninsert_exists && ninserted + ninsert_this > ninsert)
  {
      error->warning(FLERR,"INFO: Particle insertion: Number of particles to insert was slightly exceeded by random process");
  }

  // fill xnear array with particles to check overlap against
  
  // add particles in insertion volume to xnear list
  neighList.reset();

  if(check_ol_flag)
    load_xnear(ninsert_this_local);

  // insertion counters in this step
  int ninserted_this = 0, ninserted_spheres_this = 0;
  int ninserted_this_local = 0, ninserted_spheres_this_local = 0;
  double mass_inserted_this = 0.;
  double mass_inserted_this_local = 0.;

  // randomize insertion positions and set v, omega
  // also performs overlap check via xnear if requested
  // returns # bodies and # spheres that could actually be inserted
  x_v_omega(ninsert_this_local,ninserted_this_local,ninserted_spheres_this_local,mass_inserted_this_local);

  // actual particle insertion

  fix_distribution->pre_insert(ninserted_this_local,fix_property,fix_property_value);
  
  ninserted_spheres_this_local = fix_distribution->insert(ninserted_this_local);

  // warn if max # insertions exceeded by random processes
  if (ninsert_exists && ninserted + ninsert_this > ninsert)
  {
      error->warning(FLERR,"INFO: Particle insertion: Number of particles to insert was slightly exceeded by random process");
  }

  // set tag # of new particles beyond all previous atoms, reset global natoms
  // if global map exists, reset it now instead of waiting for comm
  // since deleting atoms messes up ghosts
  int step = update->ntimestep;

  if (atom->tag_enable)
  {

    //force all tags to be reset by setting them to zero
    if(compress_flag)
    {
        if(comm->me == 0)
            printf("FixInsertStream: resetting tags @ step %d. \n", step);
        int *tag = atom->tag;
        for (int i = 0; i < atom->nlocal; i++)
            tag[i] = 0;
    }

    atom->tag_extend();
    atom->natoms += static_cast<double>(ninserted_spheres_this);
    if (atom->map_style)
    {
      atom->nghost = 0;
      atom->map_init();
      atom->map_set();
    }
  }

  // give particle distributions the chance to do some wrap-up
  
  fix_distribution->finalize_insertion();

  // give derived classes the chance to do some wrap-up
  
  finalize_insertion(ninserted_spheres_this_local);

  // tally stats
  MPI_Sum_Scalar(ninserted_this_local,ninserted_this,world);
  ninserted += ninserted_this;
  MPI_Sum_Scalar(mass_inserted_this_local,mass_inserted_this,world);
  massinserted += mass_inserted_this;
  print_stats_during(ninserted_this,mass_inserted_this);

  if(ninserted_this < ninsert_this && comm->me == 0)
      error->warning(FLERR,"Particle insertion: Less insertions than requested");

  if (irregular->migrate_check())
      irregular->migrate_atoms();

  // next timestep to insert
  if (insert_every && (!ninsert_exists || ninserted < ninsert)) next_reneighbor += insert_every;
  else next_reneighbor = 0;

  if(last_ins_step_set_ && next_reneighbor > last_ins_step_)
    next_reneighbor = 0;

}

/* ----------------------------------------------------------------------
   distribute insertions across processors
------------------------------------------------------------------------- */

int FixInsert::distribute_ninsert_this(int ninsert_this)
{
    int me, nprocs, ngap, ninsert_this_local, *ninsert_this_local_all;
    double fraction_local, fraction_local_all_sum, *fraction_local_all, *remainder, r, rsum;

    me = comm->me;
    nprocs = comm->nprocs;

    fraction_local = insertion_fraction();
    
    if(!exact_number)
        return static_cast<int>(fraction_local*static_cast<double>(ninsert_this) + random->uniform());

    // for exact_number==1, have to allgather to exactly match ninsert_this

    fraction_local_all = new double[nprocs];
    remainder = new double[nprocs];
    ninsert_this_local_all = new int[nprocs];

    // allgather local fractions
    MPI_Allgather(&fraction_local,1,MPI_DOUBLE,fraction_local_all,1,MPI_DOUBLE,world);

    // proc0 calculates ninsert_this_local for all processes
    
    if(me == 0)
    {
        // remove fractions < 2% / nprocs
        // have to normalize so not all portions get cancelled away for higher proc counts
        // normalize fraction_local_all so sum across processors is 1

        double lower_thresh = 0.02 / static_cast<double>(nprocs);

        fraction_local_all_sum = 0.;
        for(int iproc = 0; iproc < nprocs; iproc++)
        {
            if(fraction_local_all[iproc] < lower_thresh)
                fraction_local_all[iproc] = 0.;
            fraction_local_all_sum += fraction_local_all[iproc];
        }

        if(fraction_local_all_sum == 0.)
            error->one(FLERR,"Internal error distributing particles to processes");

        for(int iproc = 0; iproc < nprocs; iproc++)
            fraction_local_all[iproc] /= fraction_local_all_sum;

        rsum = 0.;
        for(int iproc = 0; iproc < nprocs; iproc++)
        {
            ninsert_this_local_all[iproc] = static_cast<int>(fraction_local_all[iproc]*static_cast<double>(ninsert_this));
            remainder[iproc] = fraction_local_all[iproc]*static_cast<double>(ninsert_this) - ninsert_this_local_all[iproc];
            rsum += remainder[iproc];
            
        }

        ngap = round(rsum);
        
        for(int i = 0; i < ngap; i++)
        {
            r = random->uniform() * static_cast<double>(ngap);
            int iproc = 0;
            rsum = remainder[iproc];

            while(iproc < (nprocs-1) && rsum < r)
            {
                iproc++;
                rsum += remainder[iproc];
            }
            ninsert_this_local_all[iproc]++;
        }
    }

    // Bcast the result
    MPI_Bcast(ninsert_this_local_all,nprocs, MPI_INT,0,world);
    ninsert_this_local = ninsert_this_local_all[me];

    delete []fraction_local_all;
    delete []remainder;
    delete []ninsert_this_local_all;

    return ninsert_this_local;
}

/* ----------------------------------------------------------------------
   fill neighbor list with nearby particles
------------------------------------------------------------------------- */

int FixInsert::load_xnear(int ninsert_this_local)
{
  // load up neighbor list with local and ghosts

  double **x = atom->x;
  double *radius = atom->radius;
  const int nall = atom->nlocal + atom->nghost;
  const double tol = 1e-10;

  BoundingBox bb = getBoundingBox();
  neighList.reset();
#ifdef SUPERQUADRIC_ACTIVE_FLAG
  neighList.set_obb_flag(check_obb_flag);
#endif

  if(neighList.setBoundingBox(bb, maxrad,true,true))
  {
    for (int i = 0; i < nall; ++i)
    {
      double xremap[3] = {x[i][0], x[i][1], x[i][2]};
      domain->remap(xremap);

      if(xremap[0] < domain->boxlo[0] - tol || xremap[0] > domain->boxhi[0] + tol ||
         xremap[1] < domain->boxlo[1] - tol || xremap[1] > domain->boxhi[1] + tol ||
         xremap[2] < domain->boxlo[2] - tol || xremap[2] > domain->boxhi[2] + tol)
      {
        char msg[1024];
        snprintf(msg, sizeof(msg),
                 "Fix insert load_xnear detected inconsistent atom state before neighbor-list insertion. "
                 "atom=%d step=" BIGINT_FORMAT " radius=%g x=(%g,%g,%g) remapped=(%g,%g,%g) "
                 "load_xnear maxrad=%g box=[(%g,%g,%g),(%g,%g,%g)].",
                 i, update->ntimestep, radius[i], x[i][0], x[i][1], x[i][2],
                 xremap[0], xremap[1], xremap[2],
                 maxrad,
                 domain->boxlo[0], domain->boxlo[1], domain->boxlo[2],
                 domain->boxhi[0], domain->boxhi[1], domain->boxhi[2]);
        error->one(FLERR,msg);
      }
      
      if (is_nearby(i) && neighList.isInBoundingBox(xremap) )
      {
#ifdef SUPERQUADRIC_ACTIVE_FLAG
        if(atom->superquadric_flag and check_obb_flag)
          neighList.insert_superquadric(xremap, radius[i], atom->quaternion[i], atom->shape[i], atom->blockiness[i]);
        else
          neighList.insert(xremap, radius[i]);
#else
        neighList.insert(xremap, radius[i]);
#endif
      }
    }
  }

  return neighList.count();
}

/* ----------------------------------------------------------------------
   generate random velocity based on random setting
------------------------------------------------------------------------- */

void FixInsert::generate_random_velocity(double * velocity) {
  switch(v_randomSetting) {
    case RANDOM_UNIFORM:
      velocity[0] = v_insert[0] + v_insertFluct[0] * 2.0 * (random->uniform()-0.50);
      velocity[1] = v_insert[1] + v_insertFluct[1] * 2.0 * (random->uniform()-0.50);
      velocity[2] = v_insert[2] + v_insertFluct[2] * 2.0 * (random->uniform()-0.50);
      break;

    case RANDOM_GAUSSIAN:
      velocity[0] = v_insert[0] + v_insertFluct[0] * random->gaussian();
      velocity[1] = v_insert[1] + v_insertFluct[1] * random->gaussian();
      velocity[2] = v_insert[2] + v_insertFluct[2] * random->gaussian();
  }
}

/* ---------------------------------------------------------------------- */

int FixInsert::time_to_step(double time_value, bool round_up) const
{
  if(!update->timestep_set)
    error->fix_error(FLERR,const_cast<FixInsert*>(this),"need so set 'timestep' before");
  if(time_value < 0.)
    error->fix_error(FLERR,const_cast<FixInsert*>(this),"time value must be >= 0");

  const double scaled = time_value/update->dt;
  const double eps = 1e-12;
  return round_up ? static_cast<int>(ceil(scaled - eps)) : static_cast<int>(floor(scaled + eps));
}

/* ---------------------------------------------------------------------- */

bool FixInsert::parse_inclusion_keyword(int narg, char **arg)
{
  if (strcmp(arg[iarg],"inclusion_mesh") == 0) {
    if (iarg+2 > narg) error->fix_error(FLERR,this,"");
    if(mesh_filter_id_) delete []mesh_filter_id_;
    const int n = strlen(arg[iarg+1]) + 1;
    mesh_filter_id_ = new char[n];
    strcpy(mesh_filter_id_,arg[iarg+1]);
    mesh_filter_enabled_ = true;
    iarg += 2;
    return true;
  } else if (strcmp(arg[iarg],"inclusion_side") == 0) {
    if (iarg+2 > narg) error->fix_error(FLERR,this,"");
    if(strcmp(arg[iarg+1],"inside") == 0) mesh_filter_inside_ = true;
    else if(strcmp(arg[iarg+1],"outside") == 0) mesh_filter_inside_ = false;
    else error->fix_error(FLERR,this,"expecting 'inside' or 'outside' after 'inclusion_side'");
    iarg += 2;
    return true;
  } else if (strcmp(arg[iarg],"inclusion_direction") == 0) {
    if (iarg+2 > narg) error->fix_error(FLERR,this,"");
    mesh_filter_axis_[0] = 0;
    mesh_filter_axis_[1] = 0;
    mesh_filter_axis_[2] = 0;
    if(strchr(arg[iarg+1],'x')) mesh_filter_axis_[0] = 1;
    if(strchr(arg[iarg+1],'y')) mesh_filter_axis_[1] = 1;
    if(strchr(arg[iarg+1],'z')) mesh_filter_axis_[2] = 1;
    if(!mesh_filter_axis_[0] && !mesh_filter_axis_[1] && !mesh_filter_axis_[2])
      error->fix_error(FLERR,this,"expecting a combination of x, y and z after 'inclusion_direction'");
    iarg += 2;
    return true;
  } else if (strcmp(arg[iarg],"inside") == 0 || strcmp(arg[iarg],"outside") == 0) {
    if (iarg+5 > narg) error->fix_error(FLERR,this,"");
    if(mesh_filter_id_) delete []mesh_filter_id_;
    const int n = strlen(arg[iarg+1]) + 1;
    mesh_filter_id_ = new char[n];
    strcpy(mesh_filter_id_,arg[iarg+1]);
    mesh_filter_enabled_ = true;
    mesh_filter_inside_ = (strcmp(arg[iarg],"inside") == 0);
    mesh_filter_axis_[0] = atoi(arg[iarg+2]) != 0 ? 1 : 0;
    mesh_filter_axis_[1] = atoi(arg[iarg+3]) != 0 ? 1 : 0;
    mesh_filter_axis_[2] = atoi(arg[iarg+4]) != 0 ? 1 : 0;
    if(!mesh_filter_axis_[0] && !mesh_filter_axis_[1] && !mesh_filter_axis_[2])
      error->fix_error(FLERR,this,"inside/outside requires at least one active axis flag");
    iarg += 5;
    return true;
  }

  return false;
}

/* ---------------------------------------------------------------------- */

void FixInsert::resolve_mesh_filter()
{
  if(!mesh_filter_enabled_)
    return;

  int ifix = modify->find_fix(mesh_filter_id_);
  if(ifix < 0)
    error->fix_error(FLERR,this,"could not find inclusion mesh fix id");

  if(strncmp(modify->fix[ifix]->style,"mesh/surface",12) != 0)
    error->fix_error(FLERR,this,"inclusion_mesh must reference a fix mesh/surface or fix mesh/surface/planar");

  mesh_filter_fix_ = static_cast<FixMeshSurface*>(modify->fix[ifix]);
  mesh_filter_mesh_ = mesh_filter_fix_->triMesh();
  if(!mesh_filter_mesh_)
    error->fix_error(FLERR,this,"could not access mesh referenced by inclusion_mesh");
}

/* ---------------------------------------------------------------------- */

int FixInsert::count_mesh_crossings(const double *pos, int axis) const
{
  if(!mesh_filter_mesh_)
    return 0;

  double origin[3], direction[3], node0[3], node1[3], node2[3];
  vectorCopy3D(pos, origin);
  vectorZeroize3D(direction);
  direction[axis] = 1.0;

  // Use a small geometric tolerance for parity testing without relying on
  // protected mesh internals.
  const double eps = 1e-10;
  origin[(axis + 1) % 3] += 0.341 * eps;
  origin[(axis + 2) % 3] += 0.587 * eps;

  int crossings_local = 0;
  const int nlocal = mesh_filter_mesh_->sizeLocal();
  for(int iTri = 0; iTri < nlocal; ++iTri)
  {
    mesh_filter_mesh_->node(iTri,0,node0);
    mesh_filter_mesh_->node(iTri,1,node1);
    mesh_filter_mesh_->node(iTri,2,node2);

    double t = 0.0;
    if(ray_intersects_triangle(origin,direction,node0,node1,node2,eps,t))
      ++crossings_local;
  }

  int crossings = crossings_local;
  if(comm->nprocs > 1)
    MPI_Allreduce(&crossings_local,&crossings,1,MPI_INT,MPI_SUM,world);

  return crossings;
}

/* ---------------------------------------------------------------------- */

bool FixInsert::mesh_filter_matches(const double *pos) const
{
  return mesh_filter_matches(pos,0.);
}

/* ---------------------------------------------------------------------- */

bool FixInsert::mesh_point_is_inside(const double *pos) const
{
  if(!mesh_filter_enabled_)
    return true;

  for(int axis = 0; axis < 3; ++axis)
  {
    if(!mesh_filter_axis_[axis])
      continue;

    if((count_mesh_crossings(pos,axis) % 2) != 1)
      return false;
  }

  return true;
}

/* ---------------------------------------------------------------------- */

bool FixInsert::mesh_filter_matches(const double *pos, double extent) const
{
  if(!mesh_filter_enabled_)
    return true;

  for(int axis = 0; axis < 3; ++axis)
  {
    if(!mesh_filter_axis_[axis])
      continue;

    const bool inside = (count_mesh_crossings(pos,axis) % 2) == 1;
    if(inside != mesh_filter_inside_)
      return false;
  }

  if(extent <= 0.)
    return true;

  for(int ix = (mesh_filter_axis_[0] ? -1 : 0); ix <= (mesh_filter_axis_[0] ? 1 : 0); ++ix)
    for(int iy = (mesh_filter_axis_[1] ? -1 : 0); iy <= (mesh_filter_axis_[1] ? 1 : 0); ++iy)
      for(int iz = (mesh_filter_axis_[2] ? -1 : 0); iz <= (mesh_filter_axis_[2] ? 1 : 0); ++iz)
      {
        if(ix == 0 && iy == 0 && iz == 0)
          continue;

        double probe[3];
        probe[0] = pos[0] + static_cast<double>(ix) * extent;
        probe[1] = pos[1] + static_cast<double>(iy) * extent;
        probe[2] = pos[2] + static_cast<double>(iz) * extent;

        for(int axis = 0; axis < 3; ++axis)
        {
          if(!mesh_filter_axis_[axis])
            continue;

          const bool inside = (count_mesh_crossings(probe,axis) % 2) == 1;
          if(inside != mesh_filter_inside_)
            return false;
        }
      }

  return true;
}

/* ----------------------------------------------------------------------
   pack entire state of Fix into one write
------------------------------------------------------------------------- */

void FixInsert::write_restart(FILE *fp)
{
  int n = 0;
  double list[5];
  list[n++] = static_cast<double>(random->state());
  list[n++] = static_cast<double>(ninserted);
  list[n++] = static_cast<double>(first_ins_step);
  list[n++] = static_cast<double>(next_reneighbor);
  list[n++] = massinserted;

  if (comm->me == 0) {
    int size = n * sizeof(double);
    fwrite(&size,sizeof(int),1,fp);
    fwrite(list,sizeof(double),n,fp);
  }
}

/* ----------------------------------------------------------------------
   use state info from restart file to restart the Fix
------------------------------------------------------------------------- */

void FixInsert::restart(char *buf)
{
  int n = 0;
  double *list = (double *) buf;
  bigint next_reneighbor_re;

  seed = static_cast<int> (list[n++]) + comm->me;
  ninserted = static_cast<int> (list[n++]);
  first_ins_step = static_cast<int> (list[n++]);
  next_reneighbor_re = static_cast<bigint> (list[n++]);
  massinserted = list[n++];

  random->reset(seed);

  // in order to be able to continue pouring with increased number of particles
  // if insert was already finished in run to be restarted
  if(next_reneighbor_re != 0) next_reneighbor = next_reneighbor_re;

}

/* ----------------------------------------------------------------------
   output
------------------------------------------------------------------------- */

double FixInsert::compute_vector(int index)
{
    if(index == 0) return static_cast<double>(ninserted);
    if(index == 1) return massinserted;
    return 0.0;
}
