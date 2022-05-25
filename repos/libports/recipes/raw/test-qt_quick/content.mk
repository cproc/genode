content: test-qt_quick.tar

test-qt_quick.tar:
	tar --owner=0 --group=0 --numeric-owner --mode='go=' --mtime='1970-01-01 00:00+00' -cf $@ -C $(REP_DIR)/src/test/qt5/qt_quick redstone.png
