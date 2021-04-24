TARGET   = nitpicker
LIBS     = base blit
SRC_CC   = $(notdir $(wildcard $(PRG_DIR)/*.cc))
SRC_BIN  = default.tff

CC_WARN += -Wno-error=narrowing
