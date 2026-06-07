#include <cstdlib>
#include <cstring>

#include "time_input.h"
#include "error.h"
#include "force.h"
#include "lammps.h"

namespace LAMMPS_NS {

namespace {

bool is_inline_unit_suffix(const char *suffix, TimeInputUnit &unit)
{
  if(strcmp(suffix,"s") == 0 || strcmp(suffix,"sec") == 0)
  {
    unit = TIMEINPUT_UNIT_SECOND;
    return true;
  }

  if(strcmp(suffix,"step") == 0 || strcmp(suffix,"steps") == 0)
  {
    unit = TIMEINPUT_UNIT_STEP;
    return true;
  }

  return false;
}

}

bool is_time_input_unit_token(const char *token, TimeInputUnit &unit)
{
  return is_inline_unit_suffix(token,unit);
}

bool parse_time_input(LAMMPS *lmp, int narg, char **arg, int iarg,
                      TimeInputUnit default_unit, TimeInputValue &result)
{
  if(iarg >= narg)
    return false;

  char *endptr = NULL;
  const double value = lmp->force->numeric(FLERR,arg[iarg]);
  std::strtod(arg[iarg],&endptr);

  result.value = value;
  result.unit = default_unit;
  result.consumed = 1;

  if(endptr && *endptr)
  {
    if(!is_inline_unit_suffix(endptr,result.unit))
      lmp->error->all(FLERR,"Illegal time unit specifier");
    return true;
  }

  if(iarg + 1 < narg)
  {
    TimeInputUnit trailing_unit = default_unit;
    if(is_time_input_unit_token(arg[iarg+1],trailing_unit))
    {
      result.unit = trailing_unit;
      result.consumed = 2;
    }
  }

  return true;
}

}
