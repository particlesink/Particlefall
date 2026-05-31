/* ----------------------------------------------------------------------
    This is the

    ██╗     ██╗ ██████╗  ██████╗  ██████╗ ██╗  ██╗████████╗███████╗
    ██║     ██║██╔════╝ ██╔════╝ ██╔════╝ ██║  ██║╚══██╔══╝██╔════╝
    ██║     ██║██║  ███╗██║  ███╗██║  ███╗███████║   ██║   ███████╗
    ██║     ██║██║   ██║██║   ██║██║   ██║██╔══██║   ██║   ╚════██║
    ███████╗██║╚██████╔╝╚██████╔╝╚██████╔╝██║  ██║   ██║   ███████║
    ╚══════╝╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝®

-------------------------------------------------------------------------
    Internal VTK writer implementation used by dump styles without the
    external VTK dependency.
------------------------------------------------------------------------- */

#ifdef LAMMPS_VTK

#include "dump_vtk.h"
#include "error.h"
#include "comm.h"
#include "update.h"
#include "universe.h"
#include <fstream>
#include <iomanip>
#include <sstream>

namespace LAMMPS_NS
{

namespace
{
std::string base_name(const char *filename)
{
    std::string name(filename);
    const std::size_t dot = name.find_last_of('.');
    return dot == std::string::npos ? name : name.substr(0, dot);
}

std::string extension_of(const char *filename)
{
    std::string name(filename);
    const std::size_t dot = name.find_last_of('.');
    return dot == std::string::npos ? std::string() : name.substr(dot);
}

std::string leaf_name(const std::string &path)
{
    const std::size_t pos = path.find_last_of("/\\");
    return pos == std::string::npos ? path : path.substr(pos + 1);
}

std::string piece_file_name(const char *filename, int piece, const char *serial_ext)
{
    std::ostringstream oss;
    oss << base_name(filename) << "_" << piece << serial_ext;
    return oss.str();
}

void open_ascii(std::ofstream &file, const char *filename)
{
    file.open(filename);
    file << std::scientific << std::setprecision(16);
}

void write_legacy_scalar_or_vector(std::ofstream &file, vtkSmartPointer<vtkAbstractArray> array)
{
    if (!array) return;
    const int ncomp = array->GetNumberOfComponents();

    if (array->GetDataType() == VTK_INT)
    {
        vtkIntArray *ints = static_cast<vtkIntArray *>(array.GetPointer());
        const std::vector<int> &data = ints->data();
        if (ncomp == 3)
        {
            file << "VECTORS " << array->GetName() << " int\n";
            for (std::size_t i = 0; i < data.size(); i += 3)
                file << data[i] << ' ' << data[i+1] << ' ' << data[i+2] << '\n';
        }
        else
        {
            file << "SCALARS " << array->GetName() << " int 1\n";
            file << "LOOKUP_TABLE default\n";
            for (std::size_t i = 0; i < data.size(); ++i)
                file << data[i] << '\n';
        }
    }
    else if (array->GetDataType() == VTK_DOUBLE)
    {
        vtkDoubleArray *doubles = static_cast<vtkDoubleArray *>(array.GetPointer());
        const std::vector<double> &data = doubles->data();
        if (ncomp == 3)
        {
            file << "VECTORS " << array->GetName() << " double\n";
            for (std::size_t i = 0; i < data.size(); i += 3)
                file << data[i] << ' ' << data[i+1] << ' ' << data[i+2] << '\n';
        }
        else
        {
            file << "SCALARS " << array->GetName() << " double 1\n";
            file << "LOOKUP_TABLE default\n";
            for (std::size_t i = 0; i < data.size(); ++i)
                file << data[i] << '\n';
        }
    }
}

void write_xml_data_array(std::ofstream &file, vtkSmartPointer<vtkAbstractArray> array, int indent)
{
    if (!array) return;
    std::string pad(indent, ' ');
    file << pad << "<DataArray ";
    if (array->GetDataType() == VTK_INT)
        file << "type=\"Int32\" ";
    else if (array->GetDataType() == VTK_DOUBLE)
        file << "type=\"Float64\" ";
    else
        file << "type=\"String\" ";
    file << "Name=\"" << array->GetName() << "\" ";
    if (array->GetNumberOfComponents() > 1)
        file << "NumberOfComponents=\"" << array->GetNumberOfComponents() << "\" ";
    file << "format=\"ascii\">\n";
    file << pad << "  ";
    if (array->GetDataType() == VTK_INT)
    {
        const std::vector<int> &data = static_cast<vtkIntArray *>(array.GetPointer())->data();
        for (std::size_t i = 0; i < data.size(); ++i) file << data[i] << ' ';
    }
    else if (array->GetDataType() == VTK_DOUBLE)
    {
        const std::vector<double> &data = static_cast<vtkDoubleArray *>(array.GetPointer())->data();
        for (std::size_t i = 0; i < data.size(); ++i) file << data[i] << ' ';
    }
    else
    {
        const std::vector<std::string> &data = static_cast<vtkStringArray *>(array.GetPointer())->data();
        for (std::size_t i = 0; i < data.size(); ++i) file << data[i] << ' ';
    }
    file << "\n" << pad << "</DataArray>\n";
}

void write_xml_point_data(std::ofstream &file, vtkSmartPointer<vtkDataSetAttributes> attrs, int indent, const char *tag)
{
    std::string pad(indent, ' ');
    file << pad << "<" << tag << ">\n";
    if (attrs)
    {
        const std::vector<vtkSmartPointer<vtkAbstractArray> > &arrays = attrs->arrays();
        for (std::size_t i = 0; i < arrays.size(); ++i)
            write_xml_data_array(file, arrays[i], indent + 2);
    }
    file << pad << "</" << tag << ">\n";
}

void write_polydata_xml(vtkPolyData *poly, const char *filename)
{
    std::ofstream file;
    open_ascii(file, filename);
    file << "<?xml version=\"1.0\"?>\n";
    file << "<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    file << "  <PolyData>\n";
    const int npoints = poly->GetNumberOfPoints();
    const int ncells = poly->GetNumberOfCells();
    file << "    <Piece NumberOfPoints=\"" << npoints << "\" NumberOfVerts=\""
         << (poly->GetVerts() ? poly->GetVerts()->GetNumberOfCells() : 0) << "\" NumberOfLines=\"" 
         << (poly->GetLines() ? poly->GetLines()->GetNumberOfCells() : 0)
         << "\" NumberOfStrips=\"0\" NumberOfPolys=\""
         << (poly->GetPolys() ? poly->GetPolys()->GetNumberOfCells() : 0) << "\">\n";
    write_xml_point_data(file, poly->GetPointData(), 6, "PointData");
    write_xml_point_data(file, poly->GetCellData(), 6, "CellData");
    file << "      <Points>\n";
    file << "        <DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">\n          ";
    for (int i = 0; i < npoints; ++i)
    {
        const double *pt = poly->GetPoint(i);
        file << pt[0] << ' ' << pt[1] << ' ' << pt[2] << ' ';
    }
    file << "\n        </DataArray>\n";
    file << "      </Points>\n";

    auto write_cells = [&file](const char *tag, vtkSmartPointer<vtkCellArray> cells)
    {
        file << "      <" << tag << ">\n";
        file << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n          ";
        if (cells)
        {
            const std::vector<vtkCellArray::Cell> &all = cells->cells();
            for (std::size_t i = 0; i < all.size(); ++i)
                for (std::size_t j = 0; j < all[i].ids.size(); ++j)
                    file << all[i].ids[j] << ' ';
        }
        file << "\n        </DataArray>\n";
        file << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n          ";
        if (cells)
        {
            int offset = 0;
            const std::vector<vtkCellArray::Cell> &all = cells->cells();
            for (std::size_t i = 0; i < all.size(); ++i)
            {
                offset += static_cast<int>(all[i].ids.size());
                file << offset << ' ';
            }
        }
        file << "\n        </DataArray>\n";
        file << "      </" << tag << ">\n";
    };

    write_cells("Verts", poly->GetVerts());
    write_cells("Lines", poly->GetLines());
    write_cells("Polys", poly->GetPolys());

    file << "    </Piece>\n";
    file << "  </PolyData>\n";
    file << "</VTKFile>\n";
}

void write_unstructured_xml(vtkUnstructuredGrid *grid, const char *filename)
{
    std::ofstream file;
    open_ascii(file, filename);
    file << "<?xml version=\"1.0\"?>\n";
    file << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    file << "  <UnstructuredGrid>\n";
    file << "    <Piece NumberOfPoints=\"" << grid->GetNumberOfPoints()
         << "\" NumberOfCells=\"" << grid->GetNumberOfCells() << "\">\n";
    write_xml_point_data(file, grid->GetPointData(), 6, "PointData");
    write_xml_point_data(file, grid->GetCellData(), 6, "CellData");
    file << "      <Points>\n";
    file << "        <DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">\n          ";
    for (int i = 0; i < grid->GetNumberOfPoints(); ++i)
    {
        const double *pt = grid->GetPoint(i);
        file << pt[0] << ' ' << pt[1] << ' ' << pt[2] << ' ';
    }
    file << "\n        </DataArray>\n";
    file << "      </Points>\n";
    file << "      <Cells>\n";
    file << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n          ";
    const std::vector<vtkCellArray::Cell> &cells = grid->GetCells();
    for (std::size_t i = 0; i < cells.size(); ++i)
        for (std::size_t j = 0; j < cells[i].ids.size(); ++j)
            file << cells[i].ids[j] << ' ';
    file << "\n        </DataArray>\n";
    file << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n          ";
    int offset = 0;
    for (std::size_t i = 0; i < cells.size(); ++i)
    {
        offset += static_cast<int>(cells[i].ids.size());
        file << offset << ' ';
    }
    file << "\n        </DataArray>\n";
    file << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n          ";
    for (std::size_t i = 0; i < cells.size(); ++i)
        file << cells[i].type << ' ';
    file << "\n        </DataArray>\n";
    file << "      </Cells>\n";
    file << "    </Piece>\n";
    file << "  </UnstructuredGrid>\n";
    file << "</VTKFile>\n";
}

void write_rectilinear_xml(vtkRectilinearGrid *grid, const char *filename)
{
    std::ofstream file;
    open_ascii(file, filename);
    std::array<int, 3> dims = grid->GetDimensions();
    file << "<?xml version=\"1.0\"?>\n";
    file << "<VTKFile type=\"RectilinearGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    file << "  <RectilinearGrid WholeExtent=\"0 " << dims[0]-1 << " 0 " << dims[1]-1 << " 0 " << dims[2]-1 << "\">\n";
    file << "    <Piece Extent=\"0 " << dims[0]-1 << " 0 " << dims[1]-1 << " 0 " << dims[2]-1 << "\">\n";
    write_xml_point_data(file, grid->GetPointData(), 6, "PointData");
    write_xml_point_data(file, grid->GetCellData(), 6, "CellData");
    file << "      <Coordinates>\n";
    write_xml_data_array(file, grid->GetXCoordinates(), 8);
    write_xml_data_array(file, grid->GetYCoordinates(), 8);
    write_xml_data_array(file, grid->GetZCoordinates(), 8);
    file << "      </Coordinates>\n";
    file << "    </Piece>\n";
    file << "  </RectilinearGrid>\n";
    file << "</VTKFile>\n";
}

void write_legacy_poly(vtkPolyData *poly, const char *filename, const char *label)
{
    std::ofstream file;
    open_ascii(file, filename);
    file << "# vtk DataFile Version 3.0\n";
    file << (label ? label : "Generated by Packfall") << "\n";
    file << "ASCII\n";
    file << "DATASET POLYDATA\n";
    file << "POINTS " << poly->GetNumberOfPoints() << " double\n";
    for (int i = 0; i < poly->GetNumberOfPoints(); ++i)
    {
        const double *pt = poly->GetPoint(i);
        file << pt[0] << ' ' << pt[1] << ' ' << pt[2] << '\n';
    }
    vtkSmartPointer<vtkCellArray> cells = poly->GetPolys() ? poly->GetPolys() : (poly->GetLines() ? poly->GetLines() : poly->GetVerts());
    const bool line_mode = !poly->GetPolys() && poly->GetLines();
    const bool vert_mode = !poly->GetPolys() && !poly->GetLines() && poly->GetVerts();
    const char *cell_tag = line_mode ? "LINES" : (vert_mode ? "VERTICES" : "POLYGONS");
    int total = 0;
    if (cells)
    {
        const std::vector<vtkCellArray::Cell> &all = cells->cells();
        for (std::size_t i = 0; i < all.size(); ++i)
            total += 1 + static_cast<int>(all[i].ids.size());
        file << cell_tag << " " << all.size() << " " << total << "\n";
        for (std::size_t i = 0; i < all.size(); ++i)
        {
            file << all[i].ids.size();
            for (std::size_t j = 0; j < all[i].ids.size(); ++j)
                file << ' ' << all[i].ids[j];
            file << '\n';
        }
    }
    if (poly->GetPointData())
    {
        file << "POINT_DATA " << poly->GetNumberOfPoints() << "\n";
        const std::vector<vtkSmartPointer<vtkAbstractArray> > &arrays = poly->GetPointData()->arrays();
        for (std::size_t i = 0; i < arrays.size(); ++i)
            write_legacy_scalar_or_vector(file, arrays[i]);
    }
    if (poly->GetCellData() && cells)
    {
        file << "CELL_DATA " << cells->GetNumberOfCells() << "\n";
        const std::vector<vtkSmartPointer<vtkAbstractArray> > &arrays = poly->GetCellData()->arrays();
        for (std::size_t i = 0; i < arrays.size(); ++i)
            write_legacy_scalar_or_vector(file, arrays[i]);
    }
}

void write_legacy_unstructured(vtkUnstructuredGrid *grid, const char *filename, const char *label)
{
    std::ofstream file;
    open_ascii(file, filename);
    file << "# vtk DataFile Version 3.0\n";
    file << (label ? label : "Generated by Packfall") << "\n";
    file << "ASCII\n";
    file << "DATASET UNSTRUCTURED_GRID\n";
    file << "POINTS " << grid->GetNumberOfPoints() << " double\n";
    for (int i = 0; i < grid->GetNumberOfPoints(); ++i)
    {
        const double *pt = grid->GetPoint(i);
        file << pt[0] << ' ' << pt[1] << ' ' << pt[2] << '\n';
    }
    const std::vector<vtkCellArray::Cell> &cells = grid->GetCells();
    int total = 0;
    for (std::size_t i = 0; i < cells.size(); ++i)
        total += 1 + static_cast<int>(cells[i].ids.size());
    file << "CELLS " << cells.size() << " " << total << "\n";
    for (std::size_t i = 0; i < cells.size(); ++i)
    {
        file << cells[i].ids.size();
        for (std::size_t j = 0; j < cells[i].ids.size(); ++j)
            file << ' ' << cells[i].ids[j];
        file << '\n';
    }
    file << "CELL_TYPES " << cells.size() << "\n";
    for (std::size_t i = 0; i < cells.size(); ++i)
        file << cells[i].type << "\n";
    if (grid->GetPointData())
    {
        file << "POINT_DATA " << grid->GetNumberOfPoints() << "\n";
        const std::vector<vtkSmartPointer<vtkAbstractArray> > &arrays = grid->GetPointData()->arrays();
        for (std::size_t i = 0; i < arrays.size(); ++i)
            write_legacy_scalar_or_vector(file, arrays[i]);
    }
    if (grid->GetCellData())
    {
        file << "CELL_DATA " << grid->GetNumberOfCells() << "\n";
        const std::vector<vtkSmartPointer<vtkAbstractArray> > &arrays = grid->GetCellData()->arrays();
        for (std::size_t i = 0; i < arrays.size(); ++i)
            write_legacy_scalar_or_vector(file, arrays[i]);
    }
}

void write_legacy_rectilinear(vtkRectilinearGrid *grid, const char *filename, const char *label)
{
    std::ofstream file;
    open_ascii(file, filename);
    std::array<int, 3> dims = grid->GetDimensions();
    file << "# vtk DataFile Version 3.0\n";
    file << (label ? label : "Generated by Packfall") << "\n";
    file << "ASCII\n";
    file << "DATASET RECTILINEAR_GRID\n";
    file << "DIMENSIONS " << dims[0] << " " << dims[1] << " " << dims[2] << "\n";
    file << "X_COORDINATES " << dims[0] << " double\n";
    const std::vector<double> &x = grid->GetXCoordinates()->data();
    for (std::size_t i = 0; i < x.size(); ++i) file << x[i] << ' ';
    file << "\nY_COORDINATES " << dims[1] << " double\n";
    const std::vector<double> &y = grid->GetYCoordinates()->data();
    for (std::size_t i = 0; i < y.size(); ++i) file << y[i] << ' ';
    file << "\nZ_COORDINATES " << dims[2] << " double\n";
    const std::vector<double> &z = grid->GetZCoordinates()->data();
    for (std::size_t i = 0; i < z.size(); ++i) file << z[i] << ' ';
    file << "\n";
}

void write_multiblock_vtm(vtkMultiBlockDataSet *mb, const char *filename)
{
    std::string base = base_name(filename);
    std::ofstream file;
    open_ascii(file, filename);
    file << "<?xml version=\"1.0\"?>\n";
    file << "<VTKFile type=\"vtkMultiBlockDataSet\" version=\"1.0\" byte_order=\"LittleEndian\">\n";
    file << "  <vtkMultiBlockDataSet>\n";
    for (unsigned int i = 0; i < mb->GetNumberOfBlocks(); ++i)
    {
        vtkSmartPointer<vtkDataObject> block = mb->GetBlock(i);
        if (!block) continue;
        std::ostringstream block_name;
        block_name << base << "_block" << i;
        std::string data_file;
        if (block->IsA("vtkPolyData"))
        {
            data_file = block_name.str() + ".vtp";
            write_polydata_xml(static_cast<vtkPolyData *>(block.GetPointer()), data_file.c_str());
        }
        else if (block->IsA("vtkUnstructuredGrid"))
        {
            data_file = block_name.str() + ".vtu";
            write_unstructured_xml(static_cast<vtkUnstructuredGrid *>(block.GetPointer()), data_file.c_str());
        }
        else if (block->IsA("vtkRectilinearGrid"))
        {
            data_file = block_name.str() + ".vtr";
            write_rectilinear_xml(static_cast<vtkRectilinearGrid *>(block.GetPointer()), data_file.c_str());
        }
        else
            continue;
        file << "    <DataSet index=\"" << i << "\" file=\"" << leaf_name(data_file) << "\"/>\n";
    }
    file << "  </vtkMultiBlockDataSet>\n";
    file << "</VTKFile>\n";
}
}

DumpVTK::DumpVTK(LAMMPS *lmp) :
    lmp_(lmp),
    vtk_compressor_(VTK_COMP_NONE),
    binary_(false)
{
    filesuffixes[0] = (char*) ".vtk";
    filesuffixes[1] = (char*) ".vtp";
    filesuffixes[2] = (char*) ".vtu";
    filesuffixes[3] = (char*) ".vti";
    filesuffixes[4] = (char*) ".vtr";
    filesuffixes[5] = (char*) ".vtm";
    filesuffixes[6] = (char*) ".pvtp";
    filesuffixes[7] = (char*) ".pvtu";
    filesuffixes[8] = (char*) ".pvti";
    filesuffixes[9] = (char*) ".pvtr";
}

int DumpVTK::modify_param(int narg, char **arg)
{
    if (strcmp(arg[0],"binary") == 0) {
        if (narg < 2)
            lmp_->error->all(FLERR,"Illegal dump_modify command [binary]");
        if (strcmp(arg[1],"yes") == 0)
            binary_ = true;
        else if (strcmp(arg[1],"no") == 0)
            binary_ = false;
        else
            lmp_->error->all(FLERR,"Illegal dump_modify command [binary]");
        return 2;
    }

    if (strcmp(arg[0],"compressor") == 0)
    {
        if (narg < 2)
            lmp_->error->all(FLERR,"Illegal dump_modify command [compressor]");
        if (strcmp(arg[1],"none") == 0)
        {
            vtk_compressor_ = VTK_COMP_NONE;
            return 2;
        }
        lmp_->error->warning(FLERR, "Internal VTK path ignores compression settings");
        return 2;
    }

    return 0;
}

void DumpVTK::setVtkWriterOptions(vtkSmartPointer<vtkXMLWriter>) {}
void DumpVTK::setVtkWriterOptions(vtkSmartPointer<vtkDataWriter>) {}

void DumpVTK::write_vtp(vtkSmartPointer<vtkDataObject> data, const int vtk_file_format, const char * const filename)
{
    vtkPolyData *poly = dynamic_cast<vtkPolyData *>(data.GetPointer());
    if (!poly) lmp_->error->all(FLERR, "Internal error");

    if (vtk_file_format == VTK_FILE_FORMATS::PVTP)
    {
        const std::string piece = piece_file_name(filename, lmp_->comm->me, ".vtp");
        write_polydata_xml(poly, piece.c_str());
        if (lmp_->comm->me == 0)
        {
            std::ofstream file;
            open_ascii(file, filename);
            file << "<?xml version=\"1.0\"?>\n";
            file << "<VTKFile type=\"PPolyData\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
            file << "  <PPolyData>\n";
            file << "    <PPointData/>\n";
            file << "    <PCellData/>\n";
            file << "    <PPoints><PDataArray type=\"Float64\" NumberOfComponents=\"3\"/></PPoints>\n";
            for (int i = 0; i < lmp_->comm->nprocs; ++i)
                file << "    <Piece Source=\"" << leaf_name(piece_file_name(filename, i, ".vtp")) << "\"/>\n";
            file << "  </PPolyData>\n";
            file << "</VTKFile>\n";
        }
    }
    else if (vtk_file_format == VTK_FILE_FORMATS::VTP)
        write_polydata_xml(poly, filename);
    else
        lmp_->error->all(FLERR, "Internal error");
}

void DumpVTK::write_vtu(vtkSmartPointer<vtkDataObject> data, const int vtk_file_format, const char * const filename)
{
    vtkUnstructuredGrid *grid = dynamic_cast<vtkUnstructuredGrid *>(data.GetPointer());
    if (!grid) lmp_->error->all(FLERR, "Internal error");

    if (vtk_file_format == VTK_FILE_FORMATS::PVTU)
    {
        const std::string piece = piece_file_name(filename, lmp_->comm->me, ".vtu");
        write_unstructured_xml(grid, piece.c_str());
        if (lmp_->comm->me == 0)
        {
            std::ofstream file;
            open_ascii(file, filename);
            file << "<?xml version=\"1.0\"?>\n";
            file << "<VTKFile type=\"PUnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
            file << "  <PUnstructuredGrid>\n";
            file << "    <PPointData/>\n";
            file << "    <PCellData/>\n";
            file << "    <PPoints><PDataArray type=\"Float64\" NumberOfComponents=\"3\"/></PPoints>\n";
            for (int i = 0; i < lmp_->comm->nprocs; ++i)
                file << "    <Piece Source=\"" << leaf_name(piece_file_name(filename, i, ".vtu")) << "\"/>\n";
            file << "  </PUnstructuredGrid>\n";
            file << "</VTKFile>\n";
        }
    }
    else if (vtk_file_format == VTK_FILE_FORMATS::VTU)
        write_unstructured_xml(grid, filename);
    else
        lmp_->error->all(FLERR, "Internal error");
}

void DumpVTK::write_vti(vtkSmartPointer<vtkAlgorithmOutput>, const int, const char * const)
{
    lmp_->error->all(FLERR, "Internal VTK image output is not implemented");
}

void DumpVTK::write_vtr(vtkSmartPointer<vtkDataObject> data, const int vtk_file_format, const char * const filename)
{
    vtkRectilinearGrid *grid = dynamic_cast<vtkRectilinearGrid *>(data.GetPointer());
    if (!grid) lmp_->error->all(FLERR, "Internal error");

    if (vtk_file_format == VTK_FILE_FORMATS::PVTR)
    {
        const std::string piece = piece_file_name(filename, lmp_->comm->me, ".vtr");
        write_rectilinear_xml(grid, piece.c_str());
        if (lmp_->comm->me == 0)
        {
            std::ofstream file;
            open_ascii(file, filename);
            file << "<?xml version=\"1.0\"?>\n";
            file << "<VTKFile type=\"PRectilinearGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
            file << "  <PRectilinearGrid>\n";
            for (int i = 0; i < lmp_->comm->nprocs; ++i)
                file << "    <Piece Source=\"" << leaf_name(piece_file_name(filename, i, ".vtr")) << "\"/>\n";
            file << "  </PRectilinearGrid>\n";
            file << "</VTKFile>\n";
        }
    }
    else if (vtk_file_format == VTK_FILE_FORMATS::VTR)
        write_rectilinear_xml(grid, filename);
    else
        lmp_->error->all(FLERR, "Internal error");
}

void DumpVTK::write_vtm(vtkSmartPointer<vtkMultiBlockDataSet> data, const char * const filename, const bool parallel)
{
    if (!parallel)
        write_multiblock_vtm(data.GetPointer(), filename);
    else
    {
        const std::string piece = piece_file_name(filename, lmp_->comm->me, ".vtm");
        write_multiblock_vtm(data.GetPointer(), piece.c_str());
    }
}

void DumpVTK::write_vtk_unstructured_grid(vtkSmartPointer<vtkDataObject> data, const int vtk_file_format, const char * const filename, char * const label)
{
    if (vtk_file_format != VTK_FILE_FORMATS::VTK)
        lmp_->error->all(FLERR, "Internal error");
    vtkUnstructuredGrid *grid = dynamic_cast<vtkUnstructuredGrid *>(data.GetPointer());
    if (!grid) lmp_->error->all(FLERR, "Internal error");
    write_legacy_unstructured(grid, filename, label);
}

void DumpVTK::write_vtk_rectilinear_grid(vtkSmartPointer<vtkDataObject> data, const int vtk_file_format, const char * const filename, char * const label)
{
    if (vtk_file_format != VTK_FILE_FORMATS::VTK)
        lmp_->error->all(FLERR, "Internal error");
    vtkRectilinearGrid *grid = dynamic_cast<vtkRectilinearGrid *>(data.GetPointer());
    if (!grid) lmp_->error->all(FLERR, "Internal error");
    write_legacy_rectilinear(grid, filename, label);
}

void DumpVTK::write_vtk_poly(vtkSmartPointer<vtkDataObject> data, const int vtk_file_format, const char * const filename, char * const label)
{
    if (vtk_file_format != VTK_FILE_FORMATS::VTK)
        lmp_->error->all(FLERR, "Internal error");
    vtkPolyData *poly = dynamic_cast<vtkPolyData *>(data.GetPointer());
    if (!poly) lmp_->error->all(FLERR, "Internal error");
    write_legacy_poly(poly, filename, label);
}

vtkMPIController * DumpVTK::getLocalController()
{
    vtkMPIController *vtkGlobalController = static_cast<vtkMPIController*>(vtkMultiProcessController::GetGlobalController());
    if (!vtkGlobalController)
        lmp_->error->all(FLERR, "Global VTK MPI Controller not found");

    if (lmp_->universe->existflag == 0)
        return vtkGlobalController;
    vtkMPIController *vtkLocalController = vtkGlobalController->PartitionController(lmp_->universe->iworld, 0);
    if (!vtkLocalController)
        lmp_->error->all(FLERR, "Local VTK MPI Controller not found");
    return vtkLocalController;
}

void DumpVTK::setFileCurrent(char * &filecurrent, char * const filename, const int multifile, const int padflag)
{
    delete [] filecurrent;
    filecurrent = NULL;

    if (multifile == 0)
    {
        filecurrent = new char[strlen(filename) + 1];
        strcpy(filecurrent, filename);
    }
    else
    {
        filecurrent = new char[strlen(filename) + 16];
        char *ptr = strchr(filename,'*');
        *ptr = '\0';
        if (padflag == 0)
            sprintf(filecurrent,"%s" BIGINT_FORMAT "%s",
                    filename,lmp_->update->ntimestep,ptr+1);
        else
        {
            char bif[8],pad[16];
            strcpy(bif,BIGINT_FORMAT);
            sprintf(pad,"%%s%%0%d%s%%s",padflag,&bif[1]);
            sprintf(filecurrent,pad,filename,lmp_->update->ntimestep,ptr+1);
        }
        *ptr = '*';
    }
}

int DumpVTK::identify_file_type(char * const filename, std::list<int> &allowed_extensions, char * const style, int &multiproc, int &nclusterprocs, int &filewriter, int &fileproc, MPI_Comm &world, MPI_Comm &clustercomm)
{
    if (multiproc)
        type_error("It is no longer allowed to enable parallel writing by setting the % character, please see the documentation for help.", style, allowed_extensions);

    char *suffix = strrchr(filename, '.');
    if (strlen(suffix) == 5)
    {
        multiproc = 1;
        nclusterprocs = 1;
        filewriter = 1;
        fileproc = lmp_->comm->me;
        MPI_Comm_split(world,lmp_->comm->me,0,&clustercomm);
        std::list<int>::iterator it = allowed_extensions.begin();
        for (; it != allowed_extensions.end(); it++)
            if (*it >= VTK_FILE_FORMATS::vtk_serial_file_types && strcmp(suffix, filesuffixes[*it]) == 0)
                return *it;
        type_error("Could not find allowed filetype for parallel writing.", style, allowed_extensions);
    }
    else if (strlen(suffix) == 4)
    {
        std::list<int>::iterator it = allowed_extensions.begin();
        for (; it != allowed_extensions.end(); it++)
            if (*it < VTK_FILE_FORMATS::vtk_serial_file_types && strcmp(suffix, filesuffixes[*it]) == 0)
                return *it;
        type_error("Could not find allowed filetype for serial writing.", style, allowed_extensions);
    }
    else
        type_error("Could not find allowed filetype for writing of VTK file.", style, allowed_extensions);
    return -1;
}

void DumpVTK::type_error(std::string msg, char * const style, std::list<int> &allowed_extensions)
{
    std::stringstream ss;
    ss << "dump " << std::string(style) << ": " << msg << " Allowed file extensions for this dump style are:";
    std::list<int>::iterator it = allowed_extensions.begin();
    for (; it != allowed_extensions.end(); it++)
        ss << " " << std::string(filesuffixes[*it]);
    lmp_->error->all(FLERR, ss.str().c_str());
}

}

#endif
