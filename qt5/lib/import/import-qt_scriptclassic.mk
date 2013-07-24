IMPORT_QT4_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT4_INC)

QT4_INC_DIR += $(QT4_REP_DIR)/contrib/qtscriptclassic-1.0_1-opensource/include \
               $(QT4_REP_DIR)/contrib/qtscriptclassic-1.0_1-opensource/include/QtScript \
               $(QT4_REP_DIR)/contrib/qtscriptclassic-1.0_1-opensource/src \
