REQUIRES = updated_ali_files

LSC_RUNTIME := spark
include $(call select_from_repositories,lib/mk/libsparkcrypto.inc)
