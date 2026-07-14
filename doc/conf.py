# Packfall Sphinx configuration.
#
# RST sources in this directory are the canonical documentation inputs.

import os


extensions = []
templates_path = ['_templates']
source_suffix = {'.rst': 'restructuredtext'}
master_doc = 'Manual'

project = u'Packfall'
version = 'v1.X'
release = 'v1.X'

exclude_patterns = ['_build']
pygments_style = 'sphinx'


# -- Options for HTML output ----------------------------------------------

html_theme = "sphinx_rtd_theme"
if os.path.isdir('_themes'):
    html_theme_path = ['_themes']

if os.path.isdir('_static'):
    html_static_path = ['_static']
else:
    html_static_path = []
html_show_copyright = False
htmlhelp_basename = 'Packfalldoc'


# -- Options for LaTeX output ---------------------------------------------

latex_elements = {}

latex_documents = [
    ('Manual', 'Packfall.tex', u'Packfall Documentation',
     u'David Pinson', 'manual'),
]


# -- Options for manual page output ---------------------------------------

man_pages = [
    ('Manual', 'packfall', u'Packfall Documentation',
     [u'David Pinson'], 1)
]


# -- Options for Texinfo output -------------------------------------------

texinfo_documents = [
    ('Manual', 'Packfall', u'Packfall Documentation',
     u'David Pinson', 'Packfall', 'One line description of project.',
     'Miscellaneous'),
]
