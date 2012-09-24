#
# \brief  Framebuffer driver specific for OMAP3 systems
# \author Martin Stein
# \date   2012-05-02
#

TARGET   = omap3_fb_drv
REQUIRES = omap3
SRC_CC   = main.cc
LIBS     = cxx env server
INC_DIR += $(PRG_DIR)

vpath main.cc $(PRG_DIR)

