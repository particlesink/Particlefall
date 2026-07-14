"""
ParaView programmable-filter helper for debugging Packfall/LIGGGHTS
superquadric contact against the floor.

Usage in ParaView:
1. Open `dump*.superq.vtk` or `dump*.superq.vtp`.
2. Apply a `Programmable Filter`.
3. Set `Output Data Set Type` to `PolyData`.
4. Paste the contents of this file into the filter script.
5. Click `Apply`.

What it creates:
- reconstructed superquadric surfaces
- center points for each particle
- a vertical line from each center down to z = 0

This makes it much easier to compare the rendered geometry with the actual
particle center positions used by the simulation.
"""

import vtk


def add_cell_scalar(poly, name, value):
    arr = vtk.vtkDoubleArray()
    arr.SetName(name)
    ncells = poly.GetNumberOfCells()
    arr.SetNumberOfTuples(ncells)
    for j in range(ncells):
        arr.SetTuple1(j, float(value))
    poly.GetCellData().AddArray(arr)


def add_cell_int(poly, name, value):
    arr = vtk.vtkIntArray()
    arr.SetName(name)
    ncells = poly.GetNumberOfCells()
    arr.SetNumberOfTuples(ncells)
    for j in range(ncells):
        arr.SetTuple1(j, int(value))
    poly.GetCellData().AddArray(arr)


def add_cell_vec3(poly, name, vec):
    arr = vtk.vtkDoubleArray()
    arr.SetName(name)
    arr.SetNumberOfComponents(3)
    ncells = poly.GetNumberOfCells()
    arr.SetNumberOfTuples(ncells)
    for j in range(ncells):
        arr.SetTuple3(j, float(vec[0]), float(vec[1]), float(vec[2]))
    poly.GetCellData().AddArray(arr)


inp = self.GetInputDataObject(0, 0)
if inp is None:
    raise RuntimeError("No input dataset")

points = inp.GetPoints()
pdata = inp.GetPointData()

shape_x = pdata.GetArray("shapex")
shape_y = pdata.GetArray("shapey")
shape_z = pdata.GetArray("shapez")
block1 = pdata.GetArray("blockiness1")
block2 = pdata.GetArray("blockiness2")
tensor = pdata.GetArray("TENSOR")

pid = pdata.GetArray("id")
ptype = pdata.GetArray("type")
mass = pdata.GetArray("mass")
radius = pdata.GetArray("radius")
vel = pdata.GetArray("v")
force = pdata.GetArray("f")
omega = pdata.GetArray("omega")
angmom = pdata.GetArray("angmom")
torque = pdata.GetArray("tq")

if points is None:
    raise RuntimeError("Input has no points")
if shape_x is None or shape_y is None or shape_z is None:
    raise RuntimeError("Missing shape arrays")
if block1 is None or block2 is None:
    raise RuntimeError("Missing blockiness arrays")

append = vtk.vtkAppendPolyData()
npts = points.GetNumberOfPoints()

# Center points
center_pts = vtk.vtkPoints()
center_verts = vtk.vtkCellArray()
center_poly = vtk.vtkPolyData()

center_id = vtk.vtkIntArray()
center_id.SetName("id")
center_type = vtk.vtkIntArray()
center_type.SetName("type")
center_z = vtk.vtkDoubleArray()
center_z.SetName("center_z")

# Vertical reference lines from center to floor plane z=0
line_pts = vtk.vtkPoints()
line_cells = vtk.vtkCellArray()
line_poly = vtk.vtkPolyData()

line_id = vtk.vtkIntArray()
line_id.SetName("id")
line_type = vtk.vtkIntArray()
line_type.SetName("type")
line_center_z = vtk.vtkDoubleArray()
line_center_z.SetName("center_z")

for i in range(npts):
    x, y, z = points.GetPoint(i)

    a = float(shape_x.GetTuple1(i))
    b = float(shape_y.GetTuple1(i))
    c = float(shape_z.GetTuple1(i))

    n1 = float(block1.GetTuple1(i))
    n2 = float(block2.GetTuple1(i))
    if n1 < 2.0:
        n1 = 2.0
    if n2 < 2.0:
        n2 = 2.0

    theta_roundness = 2.0 / n1
    phi_roundness = 2.0 / n2

    src = vtk.vtkSuperquadricSource()
    src.SetToroidal(False)
    src.SetScale(a, b, c)
    src.SetThetaRoundness(theta_roundness)
    src.SetPhiRoundness(phi_roundness)
    src.SetThetaResolution(48)
    src.SetPhiResolution(48)
    src.Update()

    mat = vtk.vtkMatrix4x4()
    mat.Identity()

    if tensor is not None and tensor.GetNumberOfComponents() == 9:
        t = tensor.GetTuple(i)
        mat.SetElement(0, 0, t[0])
        mat.SetElement(0, 1, t[1])
        mat.SetElement(0, 2, t[2])
        mat.SetElement(1, 0, t[3])
        mat.SetElement(1, 1, t[4])
        mat.SetElement(1, 2, t[5])
        mat.SetElement(2, 0, t[6])
        mat.SetElement(2, 1, t[7])
        mat.SetElement(2, 2, t[8])

    mat.SetElement(0, 3, x)
    mat.SetElement(1, 3, y)
    mat.SetElement(2, 3, z)

    transform = vtk.vtkTransform()
    transform.SetMatrix(mat)

    tf = vtk.vtkTransformPolyDataFilter()
    tf.SetTransform(transform)
    tf.SetInputConnection(src.GetOutputPort())
    tf.Update()

    poly = vtk.vtkPolyData()
    poly.ShallowCopy(tf.GetOutput())

    add_cell_scalar(poly, "shapex", a)
    add_cell_scalar(poly, "shapey", b)
    add_cell_scalar(poly, "shapez", c)
    add_cell_scalar(poly, "blockiness1", n1)
    add_cell_scalar(poly, "blockiness2", n2)
    add_cell_scalar(poly, "center_z", z)

    if pid is not None:
        add_cell_int(poly, "id", pid.GetTuple1(i))
    if ptype is not None:
        add_cell_int(poly, "type", ptype.GetTuple1(i))
    if mass is not None:
        add_cell_scalar(poly, "mass", mass.GetTuple1(i))
    if radius is not None:
        add_cell_scalar(poly, "radius", radius.GetTuple1(i))
    if vel is not None:
        add_cell_vec3(poly, "velocity", vel.GetTuple3(i))
    if force is not None:
        add_cell_vec3(poly, "force", force.GetTuple3(i))
    if omega is not None:
        add_cell_vec3(poly, "omega", omega.GetTuple3(i))
    if angmom is not None:
        add_cell_vec3(poly, "angmom", angmom.GetTuple3(i))
    if torque is not None:
        add_cell_vec3(poly, "torque", torque.GetTuple3(i))

    append.AddInputData(poly)

    # center point geometry
    center_idx = center_pts.InsertNextPoint(x, y, z)
    center_verts.InsertNextCell(1)
    center_verts.InsertCellPoint(center_idx)
    center_id.InsertNextTuple1(int(pid.GetTuple1(i)) if pid is not None else i + 1)
    center_type.InsertNextTuple1(int(ptype.GetTuple1(i)) if ptype is not None else 0)
    center_z.InsertNextTuple1(float(z))

    # debug line from center to floor z=0
    p0 = line_pts.InsertNextPoint(x, y, z)
    p1 = line_pts.InsertNextPoint(x, y, 0.0)
    line = vtk.vtkLine()
    line.GetPointIds().SetId(0, p0)
    line.GetPointIds().SetId(1, p1)
    line_cells.InsertNextCell(line)
    line_id.InsertNextTuple1(int(pid.GetTuple1(i)) if pid is not None else i + 1)
    line_type.InsertNextTuple1(int(ptype.GetTuple1(i)) if ptype is not None else 0)
    line_center_z.InsertNextTuple1(float(z))

append.Update()

center_poly.SetPoints(center_pts)
center_poly.SetVerts(center_verts)
center_poly.GetCellData().AddArray(center_id)
center_poly.GetCellData().AddArray(center_type)
center_poly.GetCellData().AddArray(center_z)

line_poly.SetPoints(line_pts)
line_poly.SetLines(line_cells)
line_poly.GetCellData().AddArray(line_id)
line_poly.GetCellData().AddArray(line_type)
line_poly.GetCellData().AddArray(line_center_z)

append_all = vtk.vtkAppendPolyData()
append_all.AddInputData(append.GetOutput())
append_all.AddInputData(center_poly)
append_all.AddInputData(line_poly)
append_all.Update()

output.ShallowCopy(append_all.GetOutput())
