# Packfall Sphinx configuration.
#
# RST sources in this directory are the canonical documentation inputs.

import os


extensions = []
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'Manual'

project = u'Packfall'
version = 'v1.X'
release = 'v1.X'

exclude_patterns = ['_build']
pygments_style = 'sphinx'


# -- Options for HTML output ----------------------------------------------

html_theme = "sphinx_rtd_theme"
html_theme_path = []

if os.path.isdir('_themes'):
    html_theme_path.append('_themes')

try:
    import sphinx_rtd_theme
except ImportError:
    pass
else:
    html_theme_path.insert(0, sphinx_rtd_theme.get_html_theme_path())

html_static_path = ['_static']
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
