#ifndef PACKFALL_INTERNAL_VTK_H
#define PACKFALL_INTERNAL_VTK_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

typedef long long vtkIdType;

#define VTK_MAJOR_VERSION 9
#define VTK_MINOR_VERSION 0

#define VTK_VERTEX 1
#define VTK_LINE 3
#define VTK_TRIANGLE 5
#define VTK_HEXAHEDRON 12

#define VTK_INT 6
#define VTK_DOUBLE 11
#define VTK_STRING 13

class vtkDataObject;
class vtkDataSet;
class vtkPolyData;
class vtkUnstructuredGrid;
class vtkRectilinearGrid;
class vtkMultiBlockDataSet;
class vtkAbstractArray;
class vtkDataSetAttributes;
class vtkPoints;
class vtkCellArray;

template <class T>
class vtkSmartPointer
{
public:
  vtkSmartPointer() {}
  explicit vtkSmartPointer(T *ptr) : ptr_(ptr) {}

  template <class U>
  vtkSmartPointer(const vtkSmartPointer<U> &other) :
    ptr_(std::static_pointer_cast<T>(other.shared_ptr())) {}

  template <class U>
  vtkSmartPointer &operator=(const vtkSmartPointer<U> &other)
  {
    ptr_ = std::static_pointer_cast<T>(other.shared_ptr());
    return *this;
  }

  vtkSmartPointer &operator=(T *ptr)
  {
    ptr_.reset(ptr);
    return *this;
  }

  static vtkSmartPointer<T> New()
  {
    return vtkSmartPointer<T>(new T());
  }

  T *operator->() const { return ptr_.get(); }
  T &operator*() const { return *ptr_; }
  T *GetPointer() const { return ptr_.get(); }
  operator T*() const { return ptr_.get(); }
  operator bool() const { return static_cast<bool>(ptr_); }

  std::shared_ptr<T> shared_ptr() const { return ptr_; }

private:
  std::shared_ptr<T> ptr_;
};

class vtkObjectBase
{
public:
  virtual ~vtkObjectBase() {}
  virtual const char *GetClassName() const = 0;
  virtual bool IsA(const char *name) const
  {
    return std::string(GetClassName()) == std::string(name);
  }
};

class vtkInformation : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkInformation"; }
  int NAME() const { return 0; }
  void Set(int, const char *value) { name_ = value ? value : ""; }
  const std::string &GetName() const { return name_; }

private:
  std::string name_;
};

class vtkAbstractArray : public vtkObjectBase
{
public:
  vtkAbstractArray() : ncomp_(1) {}

  void SetNumberOfComponents(int ncomp) { ncomp_ = ncomp; }
  int GetNumberOfComponents() const { return ncomp_; }
  void SetName(const char *name) { name_ = name ? name : ""; }
  const char *GetName() const { return name_.c_str(); }
  virtual int GetDataType() const = 0;
  virtual int GetNumberOfTuples() const = 0;
  virtual void InsertNextTuple(int tuple, vtkAbstractArray *other) = 0;

protected:
  int ncomp_;
  std::string name_;
};

template <class T, int TypeId>
class vtkNumericArray : public vtkAbstractArray
{
public:
  int GetDataType() const { return TypeId; }
  int GetNumberOfTuples() const
  {
    return ncomp_ > 0 ? static_cast<int>(data_.size() / ncomp_) : 0;
  }

  void InsertNextValue(T value) { data_.push_back(value); }
  void InsertNextTupleValue(const T *values)
  {
    for (int i = 0; i < ncomp_; ++i) data_.push_back(values[i]);
  }
  void InsertNextTypedTuple(const T *values) { InsertNextTupleValue(values); }
  void InsertNextTuple(const T *values) { InsertNextTupleValue(values); }
  void InsertNextTuple(int tuple, vtkAbstractArray *other)
  {
    vtkNumericArray<T, TypeId> *casted = static_cast<vtkNumericArray<T, TypeId> *>(other);
    for (int i = 0; i < ncomp_; ++i)
      data_.push_back(casted->data_[tuple * ncomp_ + i]);
  }
  const std::vector<T> &data() const { return data_; }
  std::vector<T> &data() { return data_; }

private:
  std::vector<T> data_;
};

class vtkIntArray : public vtkNumericArray<int, VTK_INT>
{
public:
  const char *GetClassName() const { return "vtkIntArray"; }
};

class vtkDoubleArray : public vtkNumericArray<double, VTK_DOUBLE>
{
public:
  const char *GetClassName() const { return "vtkDoubleArray"; }
};

class vtkStringArray : public vtkAbstractArray
{
public:
  const char *GetClassName() const { return "vtkStringArray"; }
  int GetDataType() const { return VTK_STRING; }
  int GetNumberOfTuples() const { return static_cast<int>(data_.size()); }
  void InsertNextValue(const char *value) { data_.push_back(value ? value : ""); }
  void InsertNextValue(const std::string &value) { data_.push_back(value); }
  void InsertNextTuple(int tuple, vtkAbstractArray *other)
  {
    vtkStringArray *casted = static_cast<vtkStringArray *>(other);
    data_.push_back(casted->data_[tuple]);
  }
  const std::vector<std::string> &data() const { return data_; }

private:
  std::vector<std::string> data_;
};

class vtkDataSetAttributes : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkDataSetAttributes"; }
  void AddArray(vtkSmartPointer<vtkAbstractArray> array) { arrays_.push_back(array); }
  int GetNumberOfArrays() const { return static_cast<int>(arrays_.size()); }
  vtkSmartPointer<vtkAbstractArray> GetAbstractArray(int idx) const { return arrays_[idx]; }
  vtkSmartPointer<vtkAbstractArray> GetArray(int idx) const { return arrays_[idx]; }
  const char *GetArrayName(int idx) const { return arrays_[idx]->GetName(); }
  std::vector<vtkSmartPointer<vtkAbstractArray> > &arrays() { return arrays_; }
  const std::vector<vtkSmartPointer<vtkAbstractArray> > &arrays() const { return arrays_; }

private:
  std::vector<vtkSmartPointer<vtkAbstractArray> > arrays_;
};

class vtkPoints : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkPoints"; }
  vtkIdType InsertNextPoint(double x, double y, double z)
  {
    points_.push_back({x, y, z});
    return static_cast<vtkIdType>(points_.size() - 1);
  }
  void SetNumberOfPoints(int n) { points_.resize(n); }
  void InsertPoint(int idx, double x, double y, double z)
  {
    if (idx >= static_cast<int>(points_.size()))
      points_.resize(idx + 1);
    points_[idx] = {x, y, z};
  }
  void SetPoint(int idx, const double *pt) { points_[idx] = {pt[0], pt[1], pt[2]}; }
  void SetPoint(int idx, double x, double y, double z) { points_[idx] = {x, y, z}; }
  const double *GetPoint(int idx) const { return points_[idx].data(); }
  int GetNumberOfPoints() const { return static_cast<int>(points_.size()); }
  const std::vector<std::array<double, 3> > &points() const { return points_; }

private:
  std::vector<std::array<double, 3> > points_;
};

class vtkIdList : public vtkObjectBase
{
public:
  explicit vtkIdList(int n = 0) : ids_(n, 0) {}
  const char *GetClassName() const { return "vtkIdList"; }
  void SetNumberOfIds(int n) { ids_.resize(n, 0); }
  void SetId(int idx, vtkIdType value)
  {
    if (idx >= static_cast<int>(ids_.size()))
      ids_.resize(idx + 1, 0);
    ids_[idx] = value;
  }
  vtkIdType GetId(int idx) const { return ids_[idx]; }
  int GetNumberOfIds() const { return static_cast<int>(ids_.size()); }
  const std::vector<vtkIdType> &ids() const { return ids_; }

private:
  std::vector<vtkIdType> ids_;
};

class vtkCellArray : public vtkObjectBase
{
public:
  struct Cell
  {
    int type;
    std::vector<vtkIdType> ids;
  };

  const char *GetClassName() const { return "vtkCellArray"; }
  void InsertNextCell(int npts, const vtkIdType *ids)
  {
    Cell cell;
    cell.type = infer_type(npts);
    cell.ids.assign(ids, ids + npts);
    cells_.push_back(cell);
  }
  void InsertNextCell(int npts)
  {
    pending_type_ = infer_type(npts);
    pending_ids_.clear();
    pending_expected_ = npts;
  }
  void InsertCellPoint(vtkIdType id)
  {
    pending_ids_.push_back(id);
    if (static_cast<int>(pending_ids_.size()) == pending_expected_) {
      Cell cell;
      cell.type = pending_type_;
      cell.ids = pending_ids_;
      cells_.push_back(cell);
      pending_ids_.clear();
      pending_expected_ = 0;
    }
  }
  template <class LineLike>
  void InsertNextCell(vtkSmartPointer<LineLike> line)
  {
    Cell cell;
    cell.type = line->GetCellType();
    cell.ids = line->GetPointIds()->ids();
    cells_.push_back(cell);
  }
  int GetNumberOfCells() const { return static_cast<int>(cells_.size()); }
  const std::vector<Cell> &cells() const { return cells_; }

private:
  static int infer_type(int npts)
  {
    if (npts == 1) return VTK_VERTEX;
    if (npts == 2) return VTK_LINE;
    if (npts == 3) return VTK_TRIANGLE;
    return 0;
  }

  std::vector<Cell> cells_;
  int pending_type_ = 0;
  int pending_expected_ = 0;
  std::vector<vtkIdType> pending_ids_;
};

class vtkDataObject : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkDataObject"; }
  virtual vtkSmartPointer<vtkDataSetAttributes> GetPointData() { return vtkSmartPointer<vtkDataSetAttributes>(); }
  virtual vtkSmartPointer<vtkDataSetAttributes> GetCellData() { return vtkSmartPointer<vtkDataSetAttributes>(); }
  virtual vtkSmartPointer<vtkDataSetAttributes> GetAttributes(int) { return vtkSmartPointer<vtkDataSetAttributes>(); }
};

class vtkDataSet : public vtkDataObject
{
public:
  enum AttributeType { POINT = 0, CELL = 1 };

  vtkDataSet() : point_data_(vtkSmartPointer<vtkDataSetAttributes>::New()),
                 cell_data_(vtkSmartPointer<vtkDataSetAttributes>::New()) {}

  const char *GetClassName() const { return "vtkDataSet"; }
  bool IsA(const char *name) const
  {
    return vtkDataObject::IsA(name) || std::string(name) == "vtkDataSet";
  }

  vtkSmartPointer<vtkDataSetAttributes> GetPointData() { return point_data_; }
  vtkSmartPointer<vtkDataSetAttributes> GetCellData() { return cell_data_; }
  vtkSmartPointer<vtkDataSetAttributes> GetAttributes(int which)
  {
    return which == POINT ? point_data_ : cell_data_;
  }

  virtual int GetNumberOfPoints() const = 0;
  virtual int GetNumberOfCells() const = 0;
  virtual const double *GetPoint(int idx) const = 0;

protected:
  vtkSmartPointer<vtkDataSetAttributes> point_data_;
  vtkSmartPointer<vtkDataSetAttributes> cell_data_;
};

class vtkPolyData : public vtkDataSet
{
public:
  const char *GetClassName() const { return "vtkPolyData"; }
  static vtkPolyData *SafeDownCast(vtkDataObject *obj) { return dynamic_cast<vtkPolyData *>(obj); }
  void SetPoints(vtkSmartPointer<vtkPoints> points) { points_ = points; }
  void SetPolys(vtkSmartPointer<vtkCellArray> polys) { polys_ = polys; }
  void SetLines(vtkSmartPointer<vtkCellArray> lines) { lines_ = lines; }
  void SetVerts(vtkSmartPointer<vtkCellArray> verts) { verts_ = verts; }
  vtkSmartPointer<vtkPoints> GetPoints() const { return points_; }
  vtkSmartPointer<vtkCellArray> GetPolys() const { return polys_; }
  vtkSmartPointer<vtkCellArray> GetLines() const { return lines_; }
  vtkSmartPointer<vtkCellArray> GetVerts() const { return verts_; }
  void DeepCopy(vtkPolyData *other)
  {
    points_ = other->points_;
    polys_ = other->polys_;
    lines_ = other->lines_;
    verts_ = other->verts_;
    point_data_ = other->point_data_;
    cell_data_ = other->cell_data_;
  }
  int GetNumberOfPoints() const { return points_ ? points_->GetNumberOfPoints() : 0; }
  int GetNumberOfCells() const
  {
    int n = 0;
    if (polys_) n += polys_->GetNumberOfCells();
    if (lines_) n += lines_->GetNumberOfCells();
    if (verts_) n += verts_->GetNumberOfCells();
    return n;
  }
  const double *GetPoint(int idx) const { return points_->GetPoint(idx); }

private:
  vtkSmartPointer<vtkPoints> points_;
  vtkSmartPointer<vtkCellArray> polys_;
  vtkSmartPointer<vtkCellArray> lines_;
  vtkSmartPointer<vtkCellArray> verts_;
};

class vtkUnstructuredGrid : public vtkDataSet
{
public:
  const char *GetClassName() const { return "vtkUnstructuredGrid"; }
  void Allocate(int, int) {}
  void SetPoints(vtkSmartPointer<vtkPoints> points) { points_ = points; }
  void SetCells(int cell_type, vtkSmartPointer<vtkCellArray> cells)
  {
    cells_ = cells;
    if (cells_) {
      std::vector<vtkCellArray::Cell> patched = cells_->cells();
      for (std::size_t i = 0; i < patched.size(); ++i)
        patched[i].type = cell_type;
      owned_cells_ = patched;
    }
  }
  vtkSmartPointer<vtkPoints> GetPoints() const { return points_; }
  void InsertNextCell(int cell_type, vtkSmartPointer<vtkIdList> ids)
  {
    vtkCellArray::Cell cell;
    cell.type = cell_type;
    cell.ids = ids->ids();
    owned_cells_.push_back(cell);
  }
  const std::vector<vtkCellArray::Cell> &GetCells() const
  {
    return owned_cells_.empty() && cells_ ? cells_->cells() : owned_cells_;
  }
  int GetNumberOfPoints() const { return points_ ? points_->GetNumberOfPoints() : 0; }
  int GetNumberOfCells() const { return static_cast<int>(GetCells().size()); }
  const double *GetPoint(int idx) const { return points_->GetPoint(idx); }

private:
  vtkSmartPointer<vtkPoints> points_;
  vtkSmartPointer<vtkCellArray> cells_;
  std::vector<vtkCellArray::Cell> owned_cells_;
};

class vtkRectilinearGrid : public vtkDataSet
{
public:
  const char *GetClassName() const { return "vtkRectilinearGrid"; }
  void SetDimensions(int i, int j, int k) { dims_ = {i, j, k}; }
  void SetXCoordinates(vtkSmartPointer<vtkDoubleArray> coords) { xcoords_ = coords; }
  void SetYCoordinates(vtkSmartPointer<vtkDoubleArray> coords) { ycoords_ = coords; }
  void SetZCoordinates(vtkSmartPointer<vtkDoubleArray> coords) { zcoords_ = coords; }
  std::array<int, 3> GetDimensions() const { return dims_; }
  vtkSmartPointer<vtkDoubleArray> GetXCoordinates() const { return xcoords_; }
  vtkSmartPointer<vtkDoubleArray> GetYCoordinates() const { return ycoords_; }
  vtkSmartPointer<vtkDoubleArray> GetZCoordinates() const { return zcoords_; }
  int GetNumberOfPoints() const { return dims_[0] * dims_[1] * dims_[2]; }
  int GetNumberOfCells() const
  {
    return std::max(dims_[0] - 1, 0) * std::max(dims_[1] - 1, 0) * std::max(dims_[2] - 1, 0);
  }
  const double *GetPoint(int) const
  {
    static double dummy[3] = {0.0, 0.0, 0.0};
    return dummy;
  }

private:
  std::array<int, 3> dims_ = {{0, 0, 0}};
  vtkSmartPointer<vtkDoubleArray> xcoords_;
  vtkSmartPointer<vtkDoubleArray> ycoords_;
  vtkSmartPointer<vtkDoubleArray> zcoords_;
};

class vtkMultiBlockDataSet : public vtkDataObject
{
public:
  const char *GetClassName() const { return "vtkMultiBlockDataSet"; }
  unsigned int GetNumberOfBlocks() const { return static_cast<unsigned int>(blocks_.size()); }
  void SetBlock(unsigned int idx, vtkSmartPointer<vtkDataObject> block)
  {
    if (idx >= blocks_.size()) {
      blocks_.resize(idx + 1);
      meta_.resize(idx + 1);
      for (std::size_t i = 0; i < meta_.size(); ++i)
        if (!meta_[i]) meta_[i] = vtkSmartPointer<vtkInformation>::New();
    }
    blocks_[idx] = block;
  }
  vtkSmartPointer<vtkDataObject> GetBlock(unsigned int idx) const { return blocks_[idx]; }
  vtkSmartPointer<vtkInformation> GetMetaData(unsigned int idx)
  {
    if (idx >= meta_.size()) {
      meta_.resize(idx + 1);
    }
    if (!meta_[idx]) meta_[idx] = vtkSmartPointer<vtkInformation>::New();
    return meta_[idx];
  }
  int NAME() const { return 0; }

private:
  std::vector<vtkSmartPointer<vtkDataObject> > blocks_;
  std::vector<vtkSmartPointer<vtkInformation> > meta_;
};

class vtkLine : public vtkObjectBase
{
public:
  vtkLine() : ids_(vtkSmartPointer<vtkIdList>(new vtkIdList(2))) {}
  const char *GetClassName() const { return "vtkLine"; }
  vtkSmartPointer<vtkIdList> GetPointIds() { return ids_; }
  int GetCellType() const { return VTK_LINE; }

private:
  vtkSmartPointer<vtkIdList> ids_;
};

class vtkTriangle : public vtkObjectBase
{
public:
  vtkTriangle() : ids_(vtkSmartPointer<vtkIdList>(new vtkIdList(3))) {}
  const char *GetClassName() const { return "vtkTriangle"; }
  vtkSmartPointer<vtkIdList> GetPointIds() { return ids_; }
  int GetCellType() const { return VTK_TRIANGLE; }

private:
  vtkSmartPointer<vtkIdList> ids_;
};

class vtkHexahedron : public vtkObjectBase
{
public:
  vtkHexahedron() : ids_(vtkSmartPointer<vtkIdList>(new vtkIdList(8))) {}
  const char *GetClassName() const { return "vtkHexahedron"; }
  vtkSmartPointer<vtkIdList> GetPointIds() { return ids_; }
  int GetCellType() const { return VTK_HEXAHEDRON; }

private:
  vtkSmartPointer<vtkIdList> ids_;
};

class vtkPolyVertex : public vtkObjectBase
{
public:
  vtkPolyVertex() : ids_(vtkSmartPointer<vtkIdList>::New()) {}
  const char *GetClassName() const { return "vtkPolyVertex"; }
  vtkSmartPointer<vtkIdList> GetPointIds() { return ids_; }
  int GetCellType() const { return VTK_VERTEX; }

private:
  vtkSmartPointer<vtkIdList> ids_;
};

class vtkMultiProcessController : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkMultiProcessController"; }
  static vtkMultiProcessController *GetGlobalController() { return global_.GetPointer(); }
  static void SetGlobalController(vtkSmartPointer<vtkMultiProcessController> controller) { global_ = controller; }
  static void SetGlobalController(vtkMultiProcessController *controller) { global_ = vtkSmartPointer<vtkMultiProcessController>(controller); }

private:
  inline static vtkSmartPointer<vtkMultiProcessController> global_;
};

class vtkMPICommunicatorOpaqueComm
{
public:
  template <class T>
  explicit vtkMPICommunicatorOpaqueComm(T *) {}
};

class vtkMPICommunicator : public vtkObjectBase
{
public:
  static vtkMPICommunicator *New() { return new vtkMPICommunicator(); }
  const char *GetClassName() const { return "vtkMPICommunicator"; }
  void InitializeExternal(vtkMPICommunicatorOpaqueComm *) {}
};

class vtkMPIController : public vtkMultiProcessController
{
public:
  static vtkMPIController *New() { return new vtkMPIController(); }
  const char *GetClassName() const { return "vtkMPIController"; }
  void SetCommunicator(vtkMPICommunicator *) {}
  int Send(vtkDataObject *, int, int) { return 1; }
  int Receive(vtkDataObject *, int, int) { return 1; }
  vtkMPIController *PartitionController(int, int)
  {
    vtkSmartPointer<vtkMPIController> controller = vtkSmartPointer<vtkMPIController>::New();
    owned_.push_back(controller);
    return controller.GetPointer();
  }

private:
  std::vector<vtkSmartPointer<vtkMPIController> > owned_;
};

class vtkCellDataToPointData : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkCellDataToPointData"; }
  void SetPassCellData(int pass) { pass_cell_data_ = pass != 0; }
  void SetInputData(vtkSmartPointer<vtkPolyData> input) { input_ = input; }
  void Update() { output_ = input_; if (!pass_cell_data_ && output_) output_->GetCellData()->arrays().clear(); }
  vtkDataObject *GetOutput() { return output_.GetPointer(); }
  vtkSmartPointer<vtkPolyData> GetPolyDataOutput() { return output_; }

private:
  bool pass_cell_data_ = true;
  vtkSmartPointer<vtkPolyData> input_;
  vtkSmartPointer<vtkPolyData> output_;
};

class vtkPointDataToCellData : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkPointDataToCellData"; }
  void SetPassPointData(int pass) { pass_point_data_ = pass != 0; }
  void SetInputData(vtkSmartPointer<vtkPolyData> input) { input_ = input; }
  void Update() { output_ = input_; if (!pass_point_data_ && output_) output_->GetPointData()->arrays().clear(); }
  vtkDataObject *GetOutput() { return output_.GetPointer(); }
  vtkSmartPointer<vtkPolyData> GetPolyDataOutput() { return output_; }

private:
  bool pass_point_data_ = true;
  vtkSmartPointer<vtkPolyData> input_;
  vtkSmartPointer<vtkPolyData> output_;
};

class vtkAppendPolyData : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkAppendPolyData"; }
  void AddInputData(vtkSmartPointer<vtkPolyData> input) { inputs_.push_back(input); }
  void Update()
  {
    output_ = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    vtkIdType offset = 0;
    for (std::size_t i = 0; i < inputs_.size(); ++i) {
      vtkSmartPointer<vtkPoints> in_points = inputs_[i]->GetPoints();
      for (int p = 0; p < in_points->GetNumberOfPoints(); ++p) {
        const double *pt = in_points->GetPoint(p);
        points->InsertNextPoint(pt[0], pt[1], pt[2]);
      }
      vtkSmartPointer<vtkCellArray> in_polys = inputs_[i]->GetPolys();
      if (in_polys) {
        const std::vector<vtkCellArray::Cell> &cells = in_polys->cells();
        for (std::size_t c = 0; c < cells.size(); ++c) {
          std::vector<vtkIdType> ids = cells[c].ids;
          for (std::size_t id = 0; id < ids.size(); ++id) ids[id] += offset;
          polys->InsertNextCell(static_cast<int>(ids.size()), ids.data());
        }
      }
      offset = points->GetNumberOfPoints();
    }
    output_->SetPoints(points);
    output_->SetPolys(polys);
  }
  vtkPolyData *GetOutput() { return output_.GetPointer(); }

private:
  std::vector<vtkSmartPointer<vtkPolyData> > inputs_;
  vtkSmartPointer<vtkPolyData> output_;
};

class vtkAlgorithmOutput : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkAlgorithmOutput"; }
};

class vtkXMLWriter : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkXMLWriter"; }
  static vtkSmartPointer<vtkXMLWriter> SafeDownCast(vtkObjectBase *obj)
  {
    return vtkSmartPointer<vtkXMLWriter>(dynamic_cast<vtkXMLWriter *>(obj));
  }
  void SetDataModeToBinary() {}
  void SetDataModeToAscii() {}
  void SetCompressorTypeToZLib() {}
  void SetCompressorTypeToLZ4() {}
  void SetCompressorTypeToNone() {}
};

class vtkDataWriter : public vtkObjectBase
{
public:
  const char *GetClassName() const { return "vtkDataWriter"; }
  static vtkSmartPointer<vtkDataWriter> SafeDownCast(vtkObjectBase *obj)
  {
    return vtkSmartPointer<vtkDataWriter>(dynamic_cast<vtkDataWriter *>(obj));
  }
  void SetFileTypeToBinary() {}
  void SetFileTypeToASCII() {}
};

#endif
