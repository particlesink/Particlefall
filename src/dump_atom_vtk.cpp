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
    Anton Gladky(TU Bergakademie Freiberg), gladky.anton@gmail.com
-------------------------------------------------------------------------

    Internal VTK prototype path:
    This dump style intentionally writes VTK-compatible serial ASCII files
    without depending on the external VTK library.
------------------------------------------------------------------------- */

#include <string.h>
#include "dump_atom_vtk.h"
#include "atom.h"
#include "group.h"
#include "error.h"
#include "memory.h"
#include "comm.h"
#include "update.h"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace LAMMPS_NS;

/* ---------------------------------------------------------------------- */

DumpATOMVTK::DumpATOMVTK(LAMMPS *lmp, int narg, char **arg) :
    Dump(lmp, narg, arg),
    tmpEXP(lmp)
{
  if (narg != 5) error->all(FLERR,"Illegal dump command");
  if (multiproc) error->all(FLERR,"Invalid dump filename");

  filecurrent = NULL;

  sortBuffer = new SortBuffer(lmp, true);

  size_one = 17;

  char *str = (char *) "%d %g %g %g";
  int n = strlen(str) + 1;
  format_default = new char[n];
  strcpy(format_default,str);
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::init_style()
{
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::write_header(bigint /* n */)
{
}

/* ---------------------------------------------------------------------- */

int DumpATOMVTK::count()
{
  n_calls_ = 0;

  return Dump::count();
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::pack(int *ids)
{
  int n = 0;
  int m = 0;

  int *tag = atom->tag;
  int *type = atom->type;
  int *mask = atom->mask;
  double *rmass = atom->rmass;
  double *mass = atom->mass;
  double **x = atom->x;
  double **v = atom->v;
  double **f = atom->f;
  double **o = atom->omega;
  int nlocal = atom->nlocal;

  for (int i = 0; i < nlocal; i++) {
    if (mask[i] & groupbit) {

      if (ids) ids[n++] = tag[i];

      double massTemp;

      if (rmass) {
        massTemp=rmass[i];
      } else {
        massTemp=mass[type[i]];
      }

      int me = comm->me;

      buf[m++] = x[i][0];
      buf[m++] = x[i][1];
      buf[m++] = x[i][2];

      buf[m++] = atom->radius[i];
      buf[m++] = massTemp;
      buf[m++] = static_cast<double>(tag[i]);
      buf[m++] = static_cast<double>(type[i]);

      buf[m++] = v[i][0];
      buf[m++] = v[i][1];
      buf[m++] = v[i][2];

      buf[m++] = o[i][0];
      buf[m++] = o[i][1];
      buf[m++] = o[i][2];

      buf[m++] = f[i][0];
      buf[m++] = f[i][1];
      buf[m++] = f[i][2];

      buf[m++] = static_cast<double>(me);
    }
  }

  setFileCurrent();
  tmpEXP.setFileName(filecurrent);
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::write_data(int n, double *mybuf)
{
  if (comm->me != 0) return;
  n_calls_++;
  int m = 0;
  for (int i = 0; i < n; i++) {
    DumpATOMVTK::DataVTK tmpVTKDat(
      V3(mybuf[m+0], mybuf[m+1], mybuf[m+2]),
      mybuf[m+3], mybuf[m+4], static_cast<int> (mybuf[m+5]), static_cast<int> (mybuf[m+6]),
      V3(mybuf[m+7], mybuf[m+8], mybuf[m+9]),
      V3(mybuf[m+10], mybuf[m+11], mybuf[m+12]),
      V3(mybuf[m+13], mybuf[m+14], mybuf[m+15]),
      static_cast<int> (mybuf[m+16]));
    tmpEXP.add(tmpVTKDat);
    m += size_one;
  }

  if(n_calls_ == comm->nprocs) {
    tmpEXP.writeSER();
    tmpEXP.clear();
    delete [] filecurrent;
    filecurrent = NULL;
  }
}

/* ---------------------------------------------------------------------- */

DumpATOMVTK::DataVTK::DataVTK(V3 Pos, double Rad, double Mass, int Id, int Type,
  V3 VelL, V3 VelA, V3 Force, int proc)
{
  _Pos = Pos;
  _Rad = Rad;
  _Mass = Mass;
  _Id = Id;
  _Type = Type;
  _VelL = VelL;
  _VelA = VelA;
  _Force = Force;
  _proc = proc;
}

/* ---------------------------------------------------------------------- */

std::string DumpATOMVTK::DataVTK::serialize()
{
  std::string tmp;
  std::ostringstream stringStream;

  stringStream << _Pos[0] << ' ' << _Pos[1] << ' ' << _Pos[2] << ' ' << _Rad << ' ' << _Mass << ' ' << _Id
   << ' ' << _Type << ' '
   << _VelL[0] << ' ' << _VelL[1] << ' ' << _VelL[2] << ' '
   << _VelA[0] << ' ' << _VelA[1] << ' ' << _VelA[2] << ' '
   << _Force[0] << ' ' << _Force[1] << ' ' << _Force[2] << ' ' << _proc << '\n';

  tmp = stringStream.str();
  return tmp;
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::vtkExportData::add(DumpATOMVTK::DataVTK &d)
{
  vtkData.push_back(d);
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::vtkExportData::setFileName(const char *fileName)
{
  file_name_ = fileName;

  const std::size_t dot_pos = file_name_.rfind('.');
  if (dot_pos == std::string::npos)
    lmp_->error->all(FLERR, "dump atom/vtk requires a .vtk or .vtu filename");

  const std::string extension = file_name_.substr(dot_pos);
  if (extension == ".vtk")
    format_ = FORMAT_VTK_LEGACY;
  else if (extension == ".vtu")
    format_ = FORMAT_VTU_XML;
  else
    lmp_->error->all(FLERR, "dump atom/vtk internal prototype supports only .vtk and .vtu output");
}

/* ---------------------------------------------------------------------- */

DumpATOMVTK::vtkExportData::vtkExportData(LAMMPS *lmp) :
    lmp_(lmp),
    format_(FORMAT_VTU_XML)
{
}

/* ---------------------------------------------------------------------- */

int DumpATOMVTK::vtkExportData::size()
{
  return static_cast<int>(vtkData.size());
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::vtkExportData::writeSER()
{
  if (format_ == FORMAT_VTK_LEGACY)
    write_legacy_vtk();
  else
    write_vtu();
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::vtkExportData::write_legacy_vtk() const
{
  std::ofstream file(file_name_.c_str());
  if (!file.is_open())
    lmp_->error->one(FLERR, "Cannot open dump atom/vtk file");

  file << std::scientific << std::setprecision(16);
  file << "# vtk DataFile Version 3.0\n";
  file << "Generated by Packfall internal atom/vtk prototype\n";
  file << "ASCII\n";
  file << "DATASET UNSTRUCTURED_GRID\n";
  file << "POINTS " << vtkData.size() << " double\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Pos[0] << ' ' << vtkData[i]._Pos[1] << ' ' << vtkData[i]._Pos[2] << '\n';

  file << "CELLS " << vtkData.size() << ' ' << vtkData.size() * 2 << '\n';
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << "1 " << i << '\n';

  file << "CELL_TYPES " << vtkData.size() << '\n';
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << "1\n";

  file << "POINT_DATA " << vtkData.size() << '\n';

  file << "SCALARS radii double 1\n";
  file << "LOOKUP_TABLE default\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Rad << '\n';

  file << "SCALARS mass double 1\n";
  file << "LOOKUP_TABLE default\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Mass << '\n';

  file << "SCALARS id int 1\n";
  file << "LOOKUP_TABLE default\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Id << '\n';

  file << "SCALARS type int 1\n";
  file << "LOOKUP_TABLE default\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Type << '\n';

  file << "SCALARS proc int 1\n";
  file << "LOOKUP_TABLE default\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._proc << '\n';

  file << "VECTORS velocity_lin double\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._VelL[0] << ' ' << vtkData[i]._VelL[1] << ' ' << vtkData[i]._VelL[2] << '\n';

  file << "VECTORS velocity_ang double\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._VelA[0] << ' ' << vtkData[i]._VelA[1] << ' ' << vtkData[i]._VelA[2] << '\n';

  file << "VECTORS force double\n";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Force[0] << ' ' << vtkData[i]._Force[1] << ' ' << vtkData[i]._Force[2] << '\n';
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::vtkExportData::write_vtu() const
{
  std::ofstream file(file_name_.c_str());
  if (!file.is_open())
    lmp_->error->one(FLERR, "Cannot open dump atom/vtk file");

  file << std::scientific << std::setprecision(16);
  file << "<?xml version=\"1.0\"?>\n";
  file << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
  file << "  <UnstructuredGrid>\n";
  file << "    <Piece NumberOfPoints=\"" << vtkData.size() << "\" NumberOfCells=\"" << vtkData.size() << "\">\n";
  file << "      <PointData>\n";

  file << "        <DataArray type=\"Float64\" Name=\"radii\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Rad << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Float64\" Name=\"mass\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Mass << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Int32\" Name=\"id\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Id << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Int32\" Name=\"type\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Type << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Int32\" Name=\"proc\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._proc << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Float64\" Name=\"velocity_lin\" NumberOfComponents=\"3\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._VelL[0] << ' ' << vtkData[i]._VelL[1] << ' ' << vtkData[i]._VelL[2] << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Float64\" Name=\"velocity_ang\" NumberOfComponents=\"3\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._VelA[0] << ' ' << vtkData[i]._VelA[1] << ' ' << vtkData[i]._VelA[2] << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Float64\" Name=\"force\" NumberOfComponents=\"3\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Force[0] << ' ' << vtkData[i]._Force[1] << ' ' << vtkData[i]._Force[2] << ' ';
  file << "\n        </DataArray>\n";

  file << "      </PointData>\n";
  file << "      <CellData/>\n";
  file << "      <Points>\n";
  file << "        <DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << vtkData[i]._Pos[0] << ' ' << vtkData[i]._Pos[1] << ' ' << vtkData[i]._Pos[2] << ' ';
  file << "\n        </DataArray>\n";
  file << "      </Points>\n";
  file << "      <Cells>\n";

  file << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << i << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << (i + 1) << ' ';
  file << "\n        </DataArray>\n";

  file << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n          ";
  for (std::size_t i = 0; i < vtkData.size(); ++i)
    file << "1 ";
  file << "\n        </DataArray>\n";

  file << "      </Cells>\n";
  file << "    </Piece>\n";
  file << "  </UnstructuredGrid>\n";
  file << "</VTKFile>\n";
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::vtkExportData::show()
{
  for (std::size_t i = 0; i < vtkData.size(); i++) {
    std::cerr << vtkData[i].serialize();
  }
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::vtkExportData::clear()
{
  vtkData.clear();
}

/* ---------------------------------------------------------------------- */

void DumpATOMVTK::setFileCurrent()
{
  delete [] filecurrent;
  filecurrent = NULL;

  if (multifile == 0) {
    filecurrent = new char[strlen(filename) + 1];
    strcpy(filecurrent, filename);
  } else {
    filecurrent = new char[strlen(filename) + 16];
    char *ptr = strchr(filename,'*');
    *ptr = '\0';
    if (padflag == 0)
      sprintf(filecurrent,"%s" BIGINT_FORMAT "%s",
              filename,update->ntimestep,ptr+1);
    else {
      char bif[8],pad[16];
      strcpy(bif,BIGINT_FORMAT);
      sprintf(pad,"%%s%%0%d_%%d%s%%s",padflag,&bif[1]);
      sprintf(filecurrent,pad,filename,comm->me,update->ntimestep,ptr+1);
    }
    *ptr = '*';
  }
}

/* ---------------------------------------------------------------------- */

int DumpATOMVTK::vtkExportData::modify_param(int narg, char **arg)
{
  if (strcmp(arg[0],"binary") == 0) {
    if (narg < 2)
      lmp_->error->all(FLERR,"Illegal dump_modify command [binary]");
    if (strcmp(arg[1],"no") == 0)
      return 2;
    if (strcmp(arg[1],"yes") == 0)
      lmp_->error->all(FLERR, "dump atom/vtk internal prototype only supports ASCII output");
    lmp_->error->all(FLERR,"Illegal dump_modify command [binary]");
  }

  if (strcmp(arg[0],"compressor") == 0) {
    if (narg < 2)
      lmp_->error->all(FLERR,"Illegal dump_modify command [compressor]");
    if (strcmp(arg[1],"none") == 0)
      return 2;
    if (strcmp(arg[1],"zlib") == 0 || strcmp(arg[1],"lz4") == 0)
      lmp_->error->all(FLERR, "dump atom/vtk internal prototype does not support compression");
    lmp_->error->all(FLERR,"Illegal dump_modify command [compressor]");
  }

  return 0;
}

/* ---------------------------------------------------------------------- */

int DumpATOMVTK::modify_param(int narg, char **arg)
{
  return tmpEXP.modify_param(narg, arg);
}
