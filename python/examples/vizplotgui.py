#!/usr/bin/env python3

"""Qt visualization and plotting GUI for Packfall.

Syntax:
    python vizplotgui.py in.script Nfreq compute-ID
"""

from __future__ import annotations

import sys
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
PYTHON_DIR = THIS_DIR.parent
if str(PYTHON_DIR) not in sys.path:
    sys.path.insert(0, str(PYTHON_DIR))

try:
    from PySide6.QtCore import QPointF, QTimer, Qt
    from PySide6.QtGui import QBrush, QColor, QPainter, QPen, QPolygonF
    from PySide6.QtWidgets import (
        QApplication,
        QDoubleSpinBox,
        QGridLayout,
        QHBoxLayout,
        QLabel,
        QLineEdit,
        QMainWindow,
        QMessageBox,
        QPushButton,
        QPlainTextEdit,
        QWidget,
    )
except ImportError:
    try:
        from PyQt5.QtCore import QTimer, Qt, QPointF
        from PyQt5.QtGui import QBrush, QColor, QPainter, QPen, QPolygonF
        from PyQt5.QtWidgets import (
            QApplication,
            QDoubleSpinBox,
            QGridLayout,
            QHBoxLayout,
            QLabel,
            QLineEdit,
            QMainWindow,
            QMessageBox,
            QPushButton,
            QPlainTextEdit,
            QWidget,
        )
    except ImportError as exc:
        raise SystemExit("Install PySide6 or PyQt5 to use vizplotgui.py") from exc

from packfall import packfall


class AtomView(QWidget):
    def __init__(self) -> None:
        super().__init__()
        self.atoms: list[tuple[float, float]] = []
        self.bounds = (0.0, 1.0, 0.0, 1.0)
        self.setMinimumHeight(240)

    def set_scene(self, atoms: list[tuple[float, float]], bounds: tuple[float, float, float, float]) -> None:
        self.atoms = atoms
        self.bounds = bounds
        self.update()

    def paintEvent(self, event) -> None:  # noqa: N802
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing, True)
        painter.fillRect(self.rect(), QColor("#f3f0e7"))

        margin = 16
        frame = self.rect().adjusted(margin, margin, -margin, -margin)
        painter.setPen(QPen(QColor("#4b4b4b"), 1))
        painter.drawRect(frame)

        xlo, xhi, ylo, yhi = self.bounds
        xrange = max(xhi - xlo, 1.0e-12)
        yrange = max(yhi - ylo, 1.0e-12)

        painter.setPen(Qt.NoPen)
        painter.setBrush(QBrush(QColor("#c65d3b")))
        for x, y in self.atoms:
            px = frame.left() + (x - xlo) / xrange * frame.width()
            py = frame.bottom() - (y - ylo) / yrange * frame.height()
            painter.drawEllipse(int(px - 4), int(py - 4), 8, 8)

        painter.setPen(QPen(QColor("#2f2f2f"), 1))
        painter.drawText(frame.adjusted(8, 8, -8, -8), Qt.AlignTop | Qt.AlignLeft, f"Atoms: {len(self.atoms)}")


class PlotView(QWidget):
    def __init__(self) -> None:
        super().__init__()
        self.points: list[tuple[float, float]] = []
        self.title = "Compute"
        self.setMinimumHeight(200)

    def set_series(self, title: str, points: list[tuple[float, float]]) -> None:
        self.title = title
        self.points = points
        self.update()

    def paintEvent(self, event) -> None:  # noqa: N802
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing, True)
        painter.fillRect(self.rect(), QColor("#fcfbf7"))

        margin = 24
        frame = self.rect().adjusted(margin, margin, -margin, -margin)
        painter.setPen(QPen(QColor("#4b4b4b"), 1))
        painter.drawRect(frame)
        painter.drawText(frame.adjusted(8, 8, -8, -8), Qt.AlignTop | Qt.AlignLeft, self.title)

        if len(self.points) < 2:
            return

        xs = [p[0] for p in self.points]
        ys = [p[1] for p in self.points]
        xmin, xmax = min(xs), max(xs)
        ymin, ymax = min(ys), max(ys)
        if xmax == xmin:
            xmax = xmin + 1.0
        if ymax == ymin:
            ymax = ymin + 1.0

        polygon = QPolygonF()
        for x, y in self.points:
            px = frame.left() + (x - xmin) / (xmax - xmin) * frame.width()
            py = frame.bottom() - (y - ymin) / (ymax - ymin) * frame.height()
            polygon.append(QPointF(px, py))

        painter.setPen(QPen(QColor("#1f6f8b"), 2))
        painter.drawPolyline(polygon)


class VizPlotGui(QMainWindow):
    def __init__(self, infile: Path, nfreq: int, compute_id: str) -> None:
        super().__init__()
        self.infile = infile
        self.nfreq = nfreq
        self.compute_id = compute_id
        self.lmp = packfall()
        self.runflag = False
        self.running = False
        self.damping_target = 0.1
        self.damping = self.damping_target
        self.initialized = False
        self.ntimestep = 0
        self.series: list[tuple[float, float]] = []

        self.setWindowTitle("Packfall Visualization")
        self.resize(900, 760)

        self.status_edit = QPlainTextEdit()
        self.status_edit.setReadOnly(True)
        self.atom_view = AtomView()
        self.plot_view = PlotView()

        self.path_edit = QLineEdit(str(self.infile))
        self.path_edit.setReadOnly(True)
        self.compute_edit = QLineEdit(self.compute_id)
        self.compute_edit.setReadOnly(True)

        self.run_button = QPushButton("Run")
        self.run_button.clicked.connect(self.run_requested)
        self.stop_button = QPushButton("Stop")
        self.stop_button.clicked.connect(self.stop_requested)
        self.quit_button = QPushButton("Quit")
        self.quit_button.clicked.connect(self.close)

        self.damping_spin = QDoubleSpinBox()
        self.damping_spin.setRange(0.0, 10.0)
        self.damping_spin.setSingleStep(0.05)
        self.damping_spin.setValue(self.damping_target)
        self.damping_spin.valueChanged.connect(self.set_damping)

        self.state_label = QLabel("Idle")
        self.state_label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        header = QGridLayout()
        header.addWidget(QLabel("Input"), 0, 0)
        header.addWidget(self.path_edit, 0, 1)
        header.addWidget(QLabel("Compute"), 1, 0)
        header.addWidget(self.compute_edit, 1, 1)

        controls = QHBoxLayout()
        controls.addWidget(self.run_button)
        controls.addWidget(self.stop_button)
        controls.addWidget(QLabel("Damping"))
        controls.addWidget(self.damping_spin)
        controls.addStretch(1)
        controls.addWidget(self.state_label)
        controls.addWidget(self.quit_button)

        root = QWidget()
        layout = QGridLayout(root)
        layout.addLayout(header, 0, 0)
        layout.addLayout(controls, 1, 0)
        layout.addWidget(self.atom_view, 2, 0)
        layout.addWidget(self.plot_view, 3, 0)
        layout.addWidget(self.status_edit, 4, 0)
        self.setCentralWidget(root)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.tick)
        self.timer.start(50)

        self.initialize_simulation()

    def append_status(self, message: str) -> None:
        self.status_edit.appendPlainText(message)

    def set_state(self, message: str) -> None:
        self.state_label.setText(message)

    def initialize_simulation(self) -> None:
        try:
            self.lmp.file(str(self.infile))
            self.lmp.command(f"thermo {self.nfreq}")
            self.lmp.command(f"fix 2 all viscous {self.damping}")
            self.record_state()
        except Exception as exc:
            QMessageBox.critical(self, "Packfall Error", str(exc))
            self.set_state("Error")
            return

        self.initialized = True
        self.append_status(f"Loaded {self.infile}")
        self.append_status(f"Tracking compute {self.compute_id} every {self.nfreq} steps")
        self.set_state("Ready")

    def run_requested(self) -> None:
        self.runflag = True
        self.append_status("Run requested")
        self.set_state("Running")

    def stop_requested(self) -> None:
        self.runflag = False
        self.append_status("Stop requested")

    def set_damping(self, value: float) -> None:
        self.damping_target = value
        self.append_status(f"Damping target set to {value:.2f}")

    def refresh_atoms(self) -> None:
        natoms = self.lmp.get_natoms()
        coords = self.lmp.gather_atoms("x", 1, 3)
        atoms = [(coords[3 * i], coords[3 * i + 1]) for i in range(natoms)]
        bounds = (
            self.lmp.extract_global("boxxlo", 1),
            self.lmp.extract_global("boxxhi", 1),
            self.lmp.extract_global("boxylo", 1),
            self.lmp.extract_global("boxyhi", 1),
        )
        self.atom_view.set_scene(atoms, bounds)

    def record_state(self) -> None:
        self.refresh_atoms()
        value = self.lmp.extract_compute(self.compute_id, 0, 0)
        self.series.append((float(self.ntimestep), float(value)))
        self.plot_view.set_series(self.compute_id, self.series)

    def tick(self) -> None:
        if not self.initialized:
            return

        try:
            if self.damping != self.damping_target:
                self.damping = self.damping_target
                self.lmp.command("unfix 2")
                self.lmp.command(f"fix 2 all viscous {self.damping}")
                self.running = False
                self.append_status(f"Updated viscous damping to {self.damping:.2f}")

            if self.runflag and self.running:
                self.lmp.command(f"run {self.nfreq} pre no post no")
                self.ntimestep += self.nfreq
                self.record_state()
                self.append_status(f"Advanced {self.nfreq} steps")
            elif self.runflag and not self.running:
                self.lmp.command(f"run {self.nfreq} pre yes post no")
                self.ntimestep += self.nfreq
                self.running = True
                self.record_state()
                self.append_status(f"Started run loop in {self.nfreq}-step chunks")
            elif (not self.runflag) and self.running:
                self.lmp.command(f"run {self.nfreq} pre no post yes")
                self.ntimestep += self.nfreq
                self.running = False
                self.record_state()
                self.append_status("Stopped run loop cleanly")
                self.set_state("Stopped")
        except Exception as exc:
            self.timer.stop()
            self.set_state("Error")
            QMessageBox.critical(self, "Packfall Error", str(exc))

    def closeEvent(self, event) -> None:  # noqa: N802
        self.timer.stop()
        try:
            self.lmp.close()
        except Exception:
            pass
        super().closeEvent(event)


def main() -> int:
    if len(sys.argv) != 4:
        print("Syntax: vizplotgui.py in.script Nfreq compute-ID")
        return 1

    infile = Path(sys.argv[1]).resolve()
    nfreq = int(sys.argv[2])
    compute_id = sys.argv[3]

    app = QApplication(sys.argv)
    window = VizPlotGui(infile, nfreq, compute_id)
    window.show()
    return app.exec()


if __name__ == "__main__":
    raise SystemExit(main())
