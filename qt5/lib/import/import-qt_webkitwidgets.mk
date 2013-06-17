IMPORT_QT4_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT4_INC)

#QT4_INC_DIR += $(QT4_REP_DIR)/include/qt5/qtbase/QtSql \
#               $(QT4_REP_DIR)/contrib/$(QT4)/qtbase/include/QtSql \
