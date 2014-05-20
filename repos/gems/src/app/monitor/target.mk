TARGET   = monitor
SRC_CC   = main.cc \
           aggregate.cc \
           type.cc \
           type_registry.cc \
           type_arg.cc \
           resource.cc \
           resource_registry.cc \
           top_level_type_registry.cc \
           binding.cc \
           instance.cc
SRC_BIN  = top_level_types.xml
LIBS     = base server config
INC_DIR += $(PRG_DIR)
