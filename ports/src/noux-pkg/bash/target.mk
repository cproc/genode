NOUX_CONFIGURE_ARGS = --without-bash-malloc
NOUX_CFLAGS += -Dsh_xfree=free

#
# Do not expect the environment to be passed as third argument
# to the main function. The Genode startup code does only
# deliver 'argc' and 'argv'.
#
NOUX_CFLAGS += -DNO_MAIN_ENV_ARG=1

# Prevent interactions with nonexisting tty driver
NOUX_CFLAGS += -DNO_TTY_DRIVER

LIBS += libc_stat_cache

include $(REP_DIR)/mk/noux.mk
