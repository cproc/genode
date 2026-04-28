TARGET   = linux_slirp_nic
REQUIRES = linux
LIBS     = lx_hybrid nic_driver net
INC_DIR += $(PRG_DIR)
SRC_CC   = main.cc
LX_LIBS += slirp
HOST_INC_DIR += /usr/local/include
