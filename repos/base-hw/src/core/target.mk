#
# \brief  Build config for Genodes core process
# \author Martin Stein
# \date   2011-12-16
#

# set target name that this configuration applies to
TARGET = core

# library that provides the whole configuration
LIBS += core

# add empty source to trigger build though all config is provided via lib
SRC_CC += dummy.cc
