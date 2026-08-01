"""
ParaView programmable-filter helper for Packfall superquadric dumps.

Usage in ParaView:
1. Open `dump*.superq.vtk` or `dump*.superq.vtp`.
2. Apply a `Programmable Filter`.
3. Set `Output Data Set Type` to `PolyData`.
4. Paste the contents of this file into the filter script.
5. Click `Apply`.

The filter reconstructs each particle as:
- a polygonal superquadric surface
- an ellipsoid approximation using the same semi-axes/orientation
- three local body-axis line segments
- particle-center markers
- exact floor-support markers
- exact floor-clearance guide lines
- a reference floor plane at `z = 0`

It also copies useful particle quantities such as velocity and force onto the
generated geometry as cell data.
"""

import math
import vtk

# Toggle optional debug geometry here.
RENDER_SUPERQUADRIC = True
RENDER_ELLIPSOID = False
RENDER_AXES = False
RENDER_CENTERS = False
RENDER_EXACT_SUPPORT = False
RENDER_FLOOR = False

# Visual lengths for the body-axis guides, relative to particle semi-axes.
AXIS_SCALE = 1.15
CENTER_MARKER_SCALE = 0.35
FLOOR_PADDING = 0.01
SUPPORT_MARKER_SCALE = 0.25
SUPERQ_THETA_RESOLUTION = 48
SUPERQ_PHI_RESOLUTION = 48


def sq_sign(value):
    if value > 0.0:
        return 1.0
    if value < 0.0:
        return -1.0
    return 0.0


def sq_pow_abs(value, exponent):
    return abs(value) ** exponent


def sq_signed_power(value, exponent):
    return sq_sign(value) * (abs(value) ** exponent)


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


def add_cell_string(poly, name, value):
    arr = vtk.vtkStringArray()
    arr.SetName(name)
    ncells = poly.GetNumberOfCells()
    for j in range(ncells):
        arr.InsertNextValue(str(value))
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


def add_point_scalar(poly, name, value):
    arr = vtk.vtkDoubleArray()
    arr.SetName(name)
    npts = poly.GetNumberOfPoints()
    arr.SetNumberOfTuples(npts)
    for j in range(npts):
        arr.SetTuple1(j, float(value))
    poly.GetPointData().AddArray(arr)


def add_point_int(poly, name, value):
    arr = vtk.vtkIntArray()
    arr.SetName(name)
    npts = poly.GetNumberOfPoints()
    arr.SetNumberOfTuples(npts)
    for j in range(npts):
        arr.SetTuple1(j, int(value))
    poly.GetPointData().AddArray(arr)


def add_point_string(poly, name, value):
    arr = vtk.vtkStringArray()
    arr.SetName(name)
    npts = poly.GetNumberOfPoints()
    for j in range(npts):
        arr.InsertNextValue(str(value))
    poly.GetPointData().AddArray(arr)


def add_point_vec3(poly, name, vec):
    arr = vtk.vtkDoubleArray()
    arr.SetName(name)
    arr.SetNumberOfComponents(3)
    npts = poly.GetNumberOfPoints()
    arr.SetNumberOfTuples(npts)
    for j in range(npts):
        arr.SetTuple3(j, float(vec[0]), float(vec[1]), float(vec[2]))
    poly.GetPointData().AddArray(arr)


def add_placeholder_arrays(poly):
    add_cell_int(poly, "id", -1)
    add_cell_int(poly, "type", -1)
    add_cell_scalar(poly, "mass", 0.0)
    add_cell_scalar(poly, "radius", 0.0)
    add_cell_scalar(poly, "shapex", 0.0)
    add_cell_scalar(poly, "shapey", 0.0)
    add_cell_scalar(poly, "shapez", 0.0)
    add_cell_scalar(poly, "blockiness1", 0.0)
    add_cell_scalar(poly, "blockiness2", 0.0)
    add_cell_scalar(poly, "center_x", 0.0)
    add_cell_scalar(poly, "center_y", 0.0)
    add_cell_scalar(poly, "center_z", 0.0)
    add_cell_scalar(poly, "approx_vertical_extent", 0.0)
    add_cell_scalar(poly, "approx_z_min", 0.0)
    add_cell_scalar(poly, "approx_z_max", 0.0)
    add_cell_scalar(poly, "exact_z_min", 0.0)
    add_cell_scalar(poly, "exact_floor_clearance", 0.0)
    add_cell_vec3(poly, "velocity", (0.0, 0.0, 0.0))
    add_cell_vec3(poly, "force", (0.0, 0.0, 0.0))
    add_cell_vec3(poly, "omega", (0.0, 0.0, 0.0))
    add_cell_vec3(poly, "angmom", (0.0, 0.0, 0.0))
    add_cell_vec3(poly, "torque", (0.0, 0.0, 0.0))

    if poly.GetNumberOfPoints() > 0:
        add_point_int(poly, "id", -1)
        add_point_int(poly, "type", -1)
        add_point_scalar(poly, "mass", 0.0)
        add_point_scalar(poly, "radius", 0.0)
        add_point_scalar(poly, "shapex", 0.0)
        add_point_scalar(poly, "shapey", 0.0)
        add_point_scalar(poly, "shapez", 0.0)
        add_point_scalar(poly, "blockiness1", 0.0)
        add_point_scalar(poly, "blockiness2", 0.0)
        add_point_scalar(poly, "center_x", 0.0)
        add_point_scalar(poly, "center_y", 0.0)
        add_point_scalar(poly, "center_z", 0.0)
        add_point_scalar(poly, "approx_vertical_extent", 0.0)
        add_point_scalar(poly, "approx_z_min", 0.0)
        add_point_scalar(poly, "approx_z_max", 0.0)
        add_point_scalar(poly, "exact_z_min", 0.0)
        add_point_scalar(poly, "exact_floor_clearance", 0.0)
        add_point_vec3(poly, "velocity", (0.0, 0.0, 0.0))
        add_point_vec3(poly, "force", (0.0, 0.0, 0.0))
        add_point_vec3(poly, "omega", (0.0, 0.0, 0.0))
        add_point_vec3(poly, "angmom", (0.0, 0.0, 0.0))
        add_point_vec3(poly, "torque", (0.0, 0.0, 0.0))


def make_superquadric_poly(a, b, c, n1, n2, transform):
    eps1 = 2.0 / n1
    eps2 = 2.0 / n2
    ntheta = int(SUPERQ_THETA_RESOLUTION)
    if ntheta < 8:
        ntheta = 8
    nphi = int(SUPERQ_PHI_RESOLUTION)
    if nphi < 8:
        nphi = 8

    points = vtk.vtkPoints()
    polys = vtk.vtkCellArray()

    for itheta in range(ntheta + 1):
        theta = -0.5 * math.pi + math.pi * float(itheta) / float(ntheta)
        ctheta = math.cos(theta)
        stheta = math.sin(theta)
        ctheta_term = sq_signed_power(ctheta, eps1)
        stheta_term = sq_signed_power(stheta, eps1)

        for iphi in range(nphi):
            phi = -math.pi + 2.0 * math.pi * float(iphi) / float(nphi)
            cphi = math.cos(phi)
            sphi = math.sin(phi)

            x = a * ctheta_term * sq_signed_power(cphi, eps2)
            y = b * ctheta_term * sq_signed_power(sphi, eps2)
            z = c * stheta_term

            gx, gy, gz = transform.TransformPoint(x, y, z)
            points.InsertNextPoint(gx, gy, gz)

    for itheta in range(ntheta):
        row0 = itheta * nphi
        row1 = (itheta + 1) * nphi
        for iphi in range(nphi):
            i0 = row0 + iphi
            i1 = row0 + ((iphi + 1) % nphi)
            i2 = row1 + ((iphi + 1) % nphi)
            i3 = row1 + iphi

            tri1 = vtk.vtkTriangle()
            tri1.GetPointIds().SetId(0, i0)
            tri1.GetPointIds().SetId(1, i1)
            tri1.GetPointIds().SetId(2, i2)
            polys.InsertNextCell(tri1)

            tri2 = vtk.vtkTriangle()
            tri2.GetPointIds().SetId(0, i0)
            tri2.GetPointIds().SetId(1, i2)
            tri2.GetPointIds().SetId(2, i3)
            polys.InsertNextCell(tri2)

    poly = vtk.vtkPolyData()
    poly.SetPoints(points)
    poly.SetPolys(polys)

    normals = vtk.vtkPolyDataNormals()
    normals.SetInputData(poly)
    normals.ConsistencyOn()
    normals.SplittingOff()
    normals.Update()

    out = vtk.vtkPolyData()
    out.ShallowCopy(normals.GetOutput())
    return out


def add_point_common_arrays(poly, a, b, c, n1, n2, x, y, z, exact_floor_point):
    approx_vertical_extent = 0.0
    if poly.GetNumberOfPoints() > 0:
        add_point_scalar(poly, "shapex", a)
        add_point_scalar(poly, "shapey", b)
        add_point_scalar(poly, "shapez", c)
        add_point_scalar(poly, "blockiness1", n1)
        add_point_scalar(poly, "blockiness2", n2)
        add_point_scalar(poly, "center_x", x)
        add_point_scalar(poly, "center_y", y)
        add_point_scalar(poly, "center_z", z)
        add_point_scalar(poly, "approx_vertical_extent", approx_vertical_extent)
        add_point_scalar(poly, "approx_z_min", z)
        add_point_scalar(poly, "approx_z_max", z)
        add_point_scalar(poly, "exact_z_min", exact_floor_point[2])
        add_point_scalar(poly, "exact_floor_clearance", exact_floor_point[2])


def add_common_arrays(poly, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i):
    rz0 = mat.GetElement(2, 0)
    rz1 = mat.GetElement(2, 1)
    rz2 = mat.GetElement(2, 2)
    approx_vertical_extent = (rz0 * rz0 * a * a + rz1 * rz1 * b * b + rz2 * rz2 * c * c) ** 0.5
    exact_floor_point = exact_support_point(x, y, z, a, b, c, n1, n2, mat, (0.0, 0.0, -1.0))

    add_cell_scalar(poly, "shapex", a)
    add_cell_scalar(poly, "shapey", b)
    add_cell_scalar(poly, "shapez", c)
    add_cell_scalar(poly, "blockiness1", n1)
    add_cell_scalar(poly, "blockiness2", n2)
    add_cell_scalar(poly, "center_x", x)
    add_cell_scalar(poly, "center_y", y)
    add_cell_scalar(poly, "center_z", z)
    add_cell_scalar(poly, "approx_vertical_extent", approx_vertical_extent)
    add_cell_scalar(poly, "approx_z_min", z - approx_vertical_extent)
    add_cell_scalar(poly, "approx_z_max", z + approx_vertical_extent)
    add_cell_scalar(poly, "exact_z_min", exact_floor_point[2])
    add_cell_scalar(poly, "exact_floor_clearance", exact_floor_point[2])
    add_point_common_arrays(poly, a, b, c, n1, n2, x, y, z, exact_floor_point)

    if pid is not None:
        add_cell_int(poly, "id", pid.GetTuple1(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_int(poly, "id", pid.GetTuple1(i))
    if ptype is not None:
        add_cell_int(poly, "type", ptype.GetTuple1(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_int(poly, "type", ptype.GetTuple1(i))
    if mass is not None:
        add_cell_scalar(poly, "mass", mass.GetTuple1(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_scalar(poly, "mass", mass.GetTuple1(i))
    if radius is not None:
        add_cell_scalar(poly, "radius", radius.GetTuple1(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_scalar(poly, "radius", radius.GetTuple1(i))

    if vel is not None:
        add_cell_vec3(poly, "velocity", vel.GetTuple3(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_vec3(poly, "velocity", vel.GetTuple3(i))
    if force is not None:
        add_cell_vec3(poly, "force", force.GetTuple3(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_vec3(poly, "force", force.GetTuple3(i))
    if omega is not None:
        add_cell_vec3(poly, "omega", omega.GetTuple3(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_vec3(poly, "omega", omega.GetTuple3(i))
    if angmom is not None:
        add_cell_vec3(poly, "angmom", angmom.GetTuple3(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_vec3(poly, "angmom", angmom.GetTuple3(i))
    if torque is not None:
        add_cell_vec3(poly, "torque", torque.GetTuple3(i))
        if poly.GetNumberOfPoints() > 0:
            add_point_vec3(poly, "torque", torque.GetTuple3(i))


def make_center_poly(x, y, z, marker_radius, i, pid, ptype):
    src = vtk.vtkSphereSource()
    src.SetCenter(x, y, z)
    src.SetRadius(marker_radius)
    src.SetThetaResolution(18)
    src.SetPhiResolution(18)
    src.Update()

    poly = vtk.vtkPolyData()
    poly.ShallowCopy(src.GetOutput())

    point_poly = vtk.vtkPolyData()
    point_pts = vtk.vtkPoints()
    point_pts.InsertNextPoint(x, y, z)
    verts = vtk.vtkCellArray()
    verts.InsertNextCell(1)
    verts.InsertCellPoint(0)
    point_poly.SetPoints(point_pts)
    point_poly.SetVerts(verts)

    point_id = int(pid.GetTuple1(i)) if pid is not None else i + 1
    point_type = int(ptype.GetTuple1(i)) if ptype is not None else 0

    add_point_scalar(point_poly, "center_x", x)
    add_point_scalar(point_poly, "center_y", y)
    add_point_scalar(point_poly, "center_z", z)
    add_point_int(point_poly, "id", point_id)
    add_point_int(point_poly, "type", point_type)
    add_point_int(point_poly, "render_mode", 3)
    add_point_string(point_poly, "render_mode_name", "center")

    return poly, point_poly


def make_support_poly(x, y, z, support_point, marker_radius, i, pid, ptype):
    sx, sy, sz = support_point

    src = vtk.vtkSphereSource()
    src.SetCenter(sx, sy, sz)
    src.SetRadius(marker_radius)
    src.SetThetaResolution(18)
    src.SetPhiResolution(18)
    src.Update()

    support_poly = vtk.vtkPolyData()
    support_poly.ShallowCopy(src.GetOutput())

    line_pts = vtk.vtkPoints()
    line_pts.InsertNextPoint(sx, sy, sz)
    line_pts.InsertNextPoint(sx, sy, 0.0)
    line = vtk.vtkLine()
    line.GetPointIds().SetId(0, 0)
    line.GetPointIds().SetId(1, 1)
    line_cells = vtk.vtkCellArray()
    line_cells.InsertNextCell(line)
    line_poly = vtk.vtkPolyData()
    line_poly.SetPoints(line_pts)
    line_poly.SetLines(line_cells)

    point_poly = vtk.vtkPolyData()
    point_pts = vtk.vtkPoints()
    point_pts.InsertNextPoint(sx, sy, sz)
    verts = vtk.vtkCellArray()
    verts.InsertNextCell(1)
    verts.InsertCellPoint(0)
    point_poly.SetPoints(point_pts)
    point_poly.SetVerts(verts)

    support_id = int(pid.GetTuple1(i)) if pid is not None else i + 1
    support_type = int(ptype.GetTuple1(i)) if ptype is not None else 0
    clearance = sz

    add_point_scalar(point_poly, "support_x", sx)
    add_point_scalar(point_poly, "support_y", sy)
    add_point_scalar(point_poly, "support_z", sz)
    add_point_scalar(point_poly, "exact_floor_clearance", clearance)
    add_point_int(point_poly, "id", support_id)
    add_point_int(point_poly, "type", support_type)
    add_point_int(point_poly, "render_mode", 5)
    add_point_string(point_poly, "render_mode_name", "exact_support")

    add_cell_scalar(line_poly, "support_z", sz)
    add_cell_scalar(line_poly, "exact_floor_clearance", clearance)
    add_point_scalar(line_poly, "support_z", sz)
    add_point_scalar(line_poly, "exact_floor_clearance", clearance)
    add_cell_int(line_poly, "id", support_id)
    add_cell_int(line_poly, "type", support_type)
    add_point_int(line_poly, "id", support_id)
    add_point_int(line_poly, "type", support_type)
    add_cell_int(line_poly, "render_mode", 6)
    add_cell_string(line_poly, "render_mode_name", "exact_support_line")
    add_point_int(line_poly, "render_mode", 6)
    add_point_string(line_poly, "render_mode_name", "exact_support_line")

    return support_poly, point_poly, line_poly


def exact_support_point(x, y, z, a, b, c, n1, n2, mat, normal_global):
    nx = mat.GetElement(0, 0) * normal_global[0] + mat.GetElement(1, 0) * normal_global[1] + mat.GetElement(2, 0) * normal_global[2]
    ny = mat.GetElement(0, 1) * normal_global[0] + mat.GetElement(1, 1) * normal_global[1] + mat.GetElement(2, 1) * normal_global[2]
    nz = mat.GetElement(0, 2) * normal_global[0] + mat.GetElement(1, 2) * normal_global[1] + mat.GetElement(2, 2) * normal_global[2]

    if abs(nx) < 1.0e-10 and abs(ny) < 1.0e-10:
        px = 0.0
        py = 0.0
        pz = sq_sign(nz) * c
    else:
        if abs(nx) > abs(ny):
            alpha = sq_pow_abs((ny * b) / (nx * a), 1.0 / (n2 - 1.0))
            gamma1 = 1.0 + sq_pow_abs(alpha, n2)
            gamma = sq_pow_abs(gamma1, n1 / n2 - 1.0)
            beta = sq_pow_abs((nz * c) / (nx * a) * gamma, 1.0 / (n1 - 1.0))

            px = 1.0 / sq_pow_abs(sq_pow_abs(gamma1, n1 / n2) + sq_pow_abs(beta, n1), 1.0 / n1)
            py = alpha * px
            pz = beta * px
        else:
            alpha = sq_pow_abs((nx * a) / (ny * b), 1.0 / (n2 - 1.0))
            gamma1 = 1.0 + sq_pow_abs(alpha, n2)
            gamma = sq_pow_abs(gamma1, n1 / n2 - 1.0)
            beta = sq_pow_abs((nz * c) / (ny * b) * gamma, 1.0 / (n1 - 1.0))

            py = 1.0 / sq_pow_abs(sq_pow_abs(gamma1, n1 / n2) + sq_pow_abs(beta, n1), 1.0 / n1)
            px = alpha * py
            pz = beta * py

        px *= a * sq_sign(nx)
        py *= b * sq_sign(ny)
        pz *= c * sq_sign(nz)

    gx = mat.GetElement(0, 0) * px + mat.GetElement(0, 1) * py + mat.GetElement(0, 2) * pz + x
    gy = mat.GetElement(1, 0) * px + mat.GetElement(1, 1) * py + mat.GetElement(1, 2) * pz + y
    gz = mat.GetElement(2, 0) * px + mat.GetElement(2, 1) * py + mat.GetElement(2, 2) * pz + z
    return (gx, gy, gz)


def make_floor_poly(bounds):
    xmin, xmax, ymin, ymax = bounds
    pts = vtk.vtkPoints()
    pts.InsertNextPoint(xmin, ymin, 0.0)
    pts.InsertNextPoint(xmax, ymin, 0.0)
    pts.InsertNextPoint(xmax, ymax, 0.0)
    pts.InsertNextPoint(xmin, ymax, 0.0)

    quad = vtk.vtkQuad()
    quad.GetPointIds().SetId(0, 0)
    quad.GetPointIds().SetId(1, 1)
    quad.GetPointIds().SetId(2, 2)
    quad.GetPointIds().SetId(3, 3)

    cells = vtk.vtkCellArray()
    cells.InsertNextCell(quad)

    poly = vtk.vtkPolyData()
    poly.SetPoints(pts)
    poly.SetPolys(cells)
    add_placeholder_arrays(poly)
    add_cell_int(poly, "render_mode", 4)
    add_cell_string(poly, "render_mode_name", "floor")
    add_point_int(poly, "render_mode", 4)
    add_point_string(poly, "render_mode_name", "floor")
    return poly


def make_axis_poly(x, y, z, a, b, c, mat, i, pid, ptype):
    pts = vtk.vtkPoints()
    lines = vtk.vtkCellArray()
    poly = vtk.vtkPolyData()

    origin = (x, y, z)
    axes_local = [
        (AXIS_SCALE * a, 0.0, 0.0),
        (0.0, AXIS_SCALE * b, 0.0),
        (0.0, 0.0, AXIS_SCALE * c),
    ]
    axis_names = ["axis_x", "axis_y", "axis_z"]

    axis_type = vtk.vtkIntArray()
    axis_type.SetName("axis_index")
    axis_name = vtk.vtkStringArray()
    axis_name.SetName("axis_name")
    axis_id = vtk.vtkIntArray()
    axis_id.SetName("id")
    axis_ptype = vtk.vtkIntArray()
    axis_ptype.SetName("type")

    for axis_index, local_tip in enumerate(axes_local):
        tx = mat.GetElement(0, 0) * local_tip[0] + mat.GetElement(0, 1) * local_tip[1] + mat.GetElement(0, 2) * local_tip[2] + x
        ty = mat.GetElement(1, 0) * local_tip[0] + mat.GetElement(1, 1) * local_tip[1] + mat.GetElement(1, 2) * local_tip[2] + y
        tz = mat.GetElement(2, 0) * local_tip[0] + mat.GetElement(2, 1) * local_tip[1] + mat.GetElement(2, 2) * local_tip[2] + z

        p0 = pts.InsertNextPoint(origin)
        p1 = pts.InsertNextPoint(tx, ty, tz)

        line = vtk.vtkLine()
        line.GetPointIds().SetId(0, p0)
        line.GetPointIds().SetId(1, p1)
        lines.InsertNextCell(line)

        axis_type.InsertNextTuple1(axis_index)
        axis_name.InsertNextValue(axis_names[axis_index])
        axis_id.InsertNextTuple1(int(pid.GetTuple1(i)) if pid is not None else i + 1)
        axis_ptype.InsertNextTuple1(int(ptype.GetTuple1(i)) if ptype is not None else 0)

    poly.SetPoints(pts)
    poly.SetLines(lines)
    poly.GetCellData().AddArray(axis_type)
    poly.GetCellData().AddArray(axis_name)
    poly.GetCellData().AddArray(axis_id)
    poly.GetCellData().AddArray(axis_ptype)
    return poly


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
xmin = None
xmax = None
ymin = None
ymax = None

for i in range(npts):
    x, y, z = points.GetPoint(i)
    xmin = x if xmin is None or x < xmin else xmin
    xmax = x if xmax is None or x > xmax else xmax
    ymin = y if ymin is None or y < ymin else ymin
    ymax = y if ymax is None or y > ymax else ymax

    a = float(shape_x.GetTuple1(i))
    b = float(shape_y.GetTuple1(i))
    c = float(shape_z.GetTuple1(i))

    n1 = float(block1.GetTuple1(i))
    n2 = float(block2.GetTuple1(i))
    if n1 < 2.0:
        n1 = 2.0
    if n2 < 2.0:
        n2 = 2.0

    # Packfall blockiness=2 maps to the ellipsoidal case. ParaView's
    # vtkSuperquadricSource uses roundness=1 for the same geometry.
    theta_roundness = 2.0 / n1
    phi_roundness = 2.0 / n2

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

    if RENDER_SUPERQUADRIC:
        poly = vtk.vtkPolyData()
        poly.ShallowCopy(make_superquadric_poly(a, b, c, n1, n2, transform))
        add_common_arrays(poly, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        add_cell_int(poly, "render_mode", 0)
        add_cell_string(poly, "render_mode_name", "superquadric")
        append.AddInputData(poly)

    if RENDER_ELLIPSOID:
        ellipsoid = vtk.vtkParametricEllipsoid()
        ellipsoid.SetXRadius(a)
        ellipsoid.SetYRadius(b)
        ellipsoid.SetZRadius(c)

        ellipsoid_src = vtk.vtkParametricFunctionSource()
        ellipsoid_src.SetParametricFunction(ellipsoid)
        ellipsoid_src.SetUResolution(48)
        ellipsoid_src.SetVResolution(48)
        ellipsoid_src.Update()

        tf_ell = vtk.vtkTransformPolyDataFilter()
        tf_ell.SetTransform(transform)
        tf_ell.SetInputConnection(ellipsoid_src.GetOutputPort())
        tf_ell.Update()

        poly_ell = vtk.vtkPolyData()
        poly_ell.ShallowCopy(tf_ell.GetOutput())
        add_common_arrays(poly_ell, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        add_cell_int(poly_ell, "render_mode", 1)
        add_cell_string(poly_ell, "render_mode_name", "ellipsoid")
        append.AddInputData(poly_ell)

    if RENDER_AXES:
        axis_poly = make_axis_poly(x, y, z, a, b, c, mat, i, pid, ptype)
        add_common_arrays(axis_poly, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        add_cell_int(axis_poly, "render_mode", 2)
        add_cell_string(axis_poly, "render_mode_name", "axes")
        if axis_poly.GetNumberOfPoints() > 0:
            add_point_int(axis_poly, "render_mode", 2)
            add_point_string(axis_poly, "render_mode_name", "axes")
        append.AddInputData(axis_poly)

    if RENDER_CENTERS:
        smallest_axis = a
        if b < smallest_axis:
            smallest_axis = b
        if c < smallest_axis:
            smallest_axis = c
        marker_radius = CENTER_MARKER_SCALE * smallest_axis
        center_poly, center_point = make_center_poly(x, y, z, marker_radius, i, pid, ptype)
        add_common_arrays(center_poly, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        add_cell_int(center_poly, "render_mode", 3)
        add_cell_string(center_poly, "render_mode_name", "center")
        add_common_arrays(center_point, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        append.AddInputData(center_poly)
        append.AddInputData(center_point)

    if RENDER_EXACT_SUPPORT:
        support_point = exact_support_point(x, y, z, a, b, c, n1, n2, mat, (0.0, 0.0, -1.0))
        smallest_axis = a
        if b < smallest_axis:
            smallest_axis = b
        if c < smallest_axis:
            smallest_axis = c
        support_radius = SUPPORT_MARKER_SCALE * smallest_axis
        support_poly, support_point_poly, support_line_poly = make_support_poly(x, y, z, support_point, support_radius, i, pid, ptype)
        add_common_arrays(support_poly, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        add_cell_int(support_poly, "render_mode", 5)
        add_cell_string(support_poly, "render_mode_name", "exact_support")
        add_common_arrays(support_point_poly, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        add_common_arrays(support_line_poly, a, b, c, n1, n2, x, y, z, mat, pid, ptype, mass, radius, vel, force, omega, angmom, torque, i)
        append.AddInputData(support_poly)
        append.AddInputData(support_point_poly)
        append.AddInputData(support_line_poly)

if RENDER_FLOOR and xmin is not None:
    floor_poly = make_floor_poly((xmin - FLOOR_PADDING, xmax + FLOOR_PADDING, ymin - FLOOR_PADDING, ymax + FLOOR_PADDING))
    append.AddInputData(floor_poly)

append.Update()
output.ShallowCopy(append.GetOutput())
