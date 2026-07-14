Math Notation Cheat Sheet
=========================

This page is a quick reference for converting old Packfall
``parsed-literal`` math notation into Sphinx ``.. math::`` and inline
``:math:`` LaTeX notation.


Old Style vs New Style
----------------------

The old docs often used visual reStructuredText styling instead of real
math markup.

+------------------------+-------------------------------+----------------------------------+
| Old notation           | Meaning                       | LaTeX form                       |
+========================+===============================+==================================+
| ``**v**``              | vector ``v``                  | ``\vec{v}``                      |
+------------------------+-------------------------------+----------------------------------+
| ``**r**``              | vector ``r``                  | ``\vec{r}``                      |
+------------------------+-------------------------------+----------------------------------+
| ``**f**``              | vector ``f``                  | ``\vec{f}``                      |
+------------------------+-------------------------------+----------------------------------+
| ``**p**``              | vector ``p``                  | ``\vec{p}``                      |
+------------------------+-------------------------------+----------------------------------+
| ``**a**``              | vector ``a``                  | ``\vec{a}``                      |
+------------------------+-------------------------------+----------------------------------+
| ``**c**``              | vector ``c``                  | ``\vec{c}``                      |
+------------------------+-------------------------------+----------------------------------+
| ``**x**\ _i``          | vector with subscript         | ``\vec{x}_i``                    |
+------------------------+-------------------------------+----------------------------------+
| ``**x**\ _{ij}``       | vector with subscript         | ``\vec{x}_{ij}``                 |
+------------------------+-------------------------------+----------------------------------+
| ``**x**\ '_{ij}``      | primed vector with subscript  | ``\vec{x}'_{ij}``                |
+------------------------+-------------------------------+----------------------------------+
| ``rho_i``              | rho subscript i               | ``\rho_i``                       |
+------------------------+-------------------------------+----------------------------------+
| ``sigma_{i,ab}``       | sigma tensor component        | ``\sigma_{i,ab}``                |
+------------------------+-------------------------------+----------------------------------+
| ``epsilon_{i,ab}``     | epsilon tensor component      | ``\epsilon_{i,ab}``              |
+------------------------+-------------------------------+----------------------------------+
| ``phi(...)``           | phi function                  | ``\phi(...)``                    |
+------------------------+-------------------------------+----------------------------------+
| ``Omega(...)``         | capital omega                 | ``\Omega(...)``                  |
+------------------------+-------------------------------+----------------------------------+
| ``pi``                 | pi                            | ``\pi``                          |
+------------------------+-------------------------------+----------------------------------+
| ``sum_j``              | summation over j              | ``\sum_j``                       |
+------------------------+-------------------------------+----------------------------------+
| ``sum_{j,k}``          | summation over j, k           | ``\sum_{j,k}``                   |
+------------------------+-------------------------------+----------------------------------+
| ``integral_0^1``       | integral from 0 to 1          | ``\int_0^1``                     |
+------------------------+-------------------------------+----------------------------------+
| ``grad_phi(...)``      | gradient of phi               | ``\nabla \phi(...)``             |
+------------------------+-------------------------------+----------------------------------+
| ``\|r\|``              | norm of r                     | ``\lVert r \rVert``              |
+------------------------+-------------------------------+----------------------------------+


Inline Math
-----------

Use the Sphinx inline math role:

.. code-block:: rst

   and :math:`\vec{c}_{jk}` is the contact point

Examples:

* ``:math:`\vec{r}_{ij}```
* ``:math:`\rho_i```
* ``:math:`\sigma_{i,ab}```


Display Math
------------

Use the Sphinx math directive:

.. code-block:: rst

   .. math::

      \epsilon_{i,ab} = \frac{1}{2 \rho_i} \sum_{j,k} m_j m_k \phi(\vec{r}_{ij}) \, \Delta t
      \left( \vec{v}_{jk,a} \nabla \phi(\vec{r}_{ik},b) +
             \vec{v}_{jk,b} \nabla \phi(\vec{r}_{ik},a) \right)


Multi-Line Equations
--------------------

For aligned multi-line equations:

.. code-block:: rst

   .. math::

      \sigma_{i,ab} &= -\frac{1}{2} \sum_{j,k} f_{jk,a} r_{jk,b} \int_0^1 \phi(\vec{r}_i - \vec{r}_j + s \vec{r}_{jk}) \, ds \\
                    &\quad - \sum_j m_j v'_{ij,a} v'_{ij,b} \phi(r_i - r_j)


Style Notes
-----------

* This cheat sheet uses ``\vec{}`` for vectors.
* If you prefer bold vectors instead, replace ``\vec{x}`` with
  ``\mathbf{x}`` consistently.
* The old ``**x**`` notation only meant "draw this symbol in bold" in
  reStructuredText.  It was not semantic math markup.
* Escaped underscores like ``\ _{ij}`` in old docs were reST workarounds;
  in LaTeX math, write normal subscripts directly.


Common Decisions
----------------

* ``dt``:

  * use ``\Delta t`` if you mean a timestep increment
  * use ``dt`` if you literally mean a differential

* vectors:

  * use ``\vec{v}`` for arrow notation
  * use ``\mathbf{v}`` if you want to preserve the old bold-vector look

