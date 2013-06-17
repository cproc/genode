IMPORT_QT4_INC=$(call select_from_repositories,lib/import/import-qt5.inc)

include $(IMPORT_QT4_INC)

QT4_INC_DIR += $(QT4_REP_DIR)/include/qt5/qtwebkit \
               $(QT4_REP_DIR)/contrib/$(QT4)/qtwebkit/include \
               $(QT4_REP_DIR)/include/qt5/qtwebkit/QtWebKit \
               $(QT4_REP_DIR)/contrib/$(QT4)/qtwebkit/include/QtWebKit \
               $(QT4_REP_DIR)/include/qt5/qtwebkit/QtWebKitWidgets \
               $(QT4_REP_DIR)/contrib/$(QT4)/qtwebkit/include/QtWebKitWidgets \
