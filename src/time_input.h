#ifndef LMP_TIME_INPUT_H
#define LMP_TIME_INPUT_H

#include "pointers.h"

namespace LAMMPS_NS {

enum TimeInputUnit {
  TIMEINPUT_UNIT_STEP = 0,
  TIMEINPUT_UNIT_SECOND = 1
};

struct TimeInputValue {
  double value;
  TimeInputUnit unit;
  int consumed;
};

bool parse_time_input(class LAMMPS *lmp, int narg, char **arg, int iarg,
                      TimeInputUnit default_unit, TimeInputValue &result);

bool is_time_input_unit_token(const char *token, TimeInputUnit &unit);

}

#endif
