#
# \brief  Download and prepare Qt4 source code
# \author Christian Prochaska
# \author Norman Feske
# \date   2009-05-11
#

VERBOSE     ?= @
ECHO         = @echo
DOWNLOAD_DIR = download
CONTRIB_DIR  = $(REP_DIR)/contrib
PATCHES_DIR  = src/lib/qt5/patches

REP_DIR := $(realpath .)

include $(REP_DIR)/lib/mk/qt_version.inc

QT5_URL = http://download.qt-project.org/official_releases/qt/5.1/$(QT_VERSION)/single
QT5_TGZ = $(QT5).tar.gz

QTSCRIPTCLASSIC_URL = http://ftp.heanet.ie/mirrors/ftp.trolltech.com/pub/qt/solutions/lgpl
QTSCRIPTCLASSIC     = qtscriptclassic-1.0_1-opensource
QTSCRIPTCLASSIC_TGZ = $(QTSCRIPTCLASSIC).tar.gz

PATCHES = $(shell cat $(PATCHES_DIR)/series)

#
# Utility to check if a tool is installed
#
check_tool = $(if $(shell which $(1)),,$(error Need to have '$(1)' installed.))

$(call check_tool,wget)
$(call check_tool,patch)
$(call check_tool,bison)
$(call check_tool,perl)
$(call check_tool,python)
$(call check_tool,sed)

help:
	$(ECHO)
	$(ECHO) "Prepare the Qt5 repository"
	$(ECHO)
	$(ECHO) "--- available commands ---"
	$(ECHO) "prepare  - download and extract the Qt4 source code archive and build the host tools"
	$(ECHO) "clean    - clean everything except downloaded archives"
	$(ECHO) "cleanall - clean everything including downloaded archives"
	$(ECHO)

$(DOWNLOAD_DIR):
	$(VERBOSE)mkdir -p $(DOWNLOAD_DIR)

$(DOWNLOAD_DIR)/$(QT5_TGZ): $(DOWNLOAD_DIR)
	$(VERBOSE)wget -c -P $(DOWNLOAD_DIR) $(QT5_URL)/$(QT5_TGZ) && touch $@

$(DOWNLOAD_DIR)/$(QTSCRIPTCLASSIC_TGZ): $(DOWNLOAD_DIR)
	$(VERBOSE)wget -c -P $(DOWNLOAD_DIR) $(QTSCRIPTCLASSIC_URL)/$(QTSCRIPTCLASSIC_TGZ) && touch $@

$(CONTRIB_DIR):
	$(VERBOSE)mkdir -p $(CONTRIB_DIR)

$(CONTRIB_DIR)/$(QT5): $(CONTRIB_DIR) $(DOWNLOAD_DIR)/$(QT5_TGZ)
#	$(VERBOSE)tar xzf $(DOWNLOAD_DIR)/$(QT5_TGZ) -C $(CONTRIB_DIR)
#	$(VERBOSE)touch $(CONTRIB_DIR)/$(QT5)
#	$(VERBOSE)for p in $(PATCHES); do \
#	            patch -d $(CONTRIB_DIR)/$(QT5) -p1 -i ../../$(PATCHES_DIR)/$$p; done

$(CONTRIB_DIR)/$(QTSCRIPTCLASSIC): $(CONTRIB_DIR) $(DOWNLOAD_DIR)/$(QTSCRIPTCLASSIC_TGZ)
#	$(VERBOSE)tar xzf $(DOWNLOAD_DIR)/$(QTSCRIPTCLASSIC_TGZ) -C $(CONTRIB_DIR)
#	$(VERBOSE)touch $(CONTRIB_DIR)/$(QTSCRIPTCLASSIC)
#	$(VERBOSE)patch -d $(CONTRIB_DIR)/$(QTSCRIPTCLASSIC) -p1 -i ../../$(PATCHES_DIR)/qtscriptclassic_qt5.patch


$(REP_DIR)/src/lib/qt5/qtwebkit/Source/JavaScriptCore/generated/generated.tag:

	mkdir -p $(dir $@)

	# create_hash_table
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/ArrayConstructor.cpp -i  > $(dir $@)/ArrayConstructor.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/ArrayPrototype.cpp -i    > $(dir $@)/ArrayPrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/BooleanPrototype.cpp -i  > $(dir $@)/BooleanPrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/DateConstructor.cpp -i   > $(dir $@)/DateConstructor.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/DatePrototype.cpp -i     > $(dir $@)/DatePrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/ErrorPrototype.cpp -i    > $(dir $@)/ErrorPrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/JSGlobalObject.cpp -i    > $(dir $@)/JSGlobalObject.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/JSONObject.cpp -i        > $(dir $@)/JSONObject.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/MathObject.cpp -i        > $(dir $@)/MathObject.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/NamePrototype.cpp -i     > $(dir $@)/NamePrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/NumberConstructor.cpp -i > $(dir $@)/NumberConstructor.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/NumberPrototype.cpp -i   > $(dir $@)/NumberPrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/ObjectConstructor.cpp -i > $(dir $@)/ObjectConstructor.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/ObjectPrototype.cpp -i   > $(dir $@)/ObjectPrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/RegExpConstructor.cpp -i > $(dir $@)/RegExpConstructor.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/RegExpObject.cpp -i      > $(dir $@)/RegExpObject.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/RegExpPrototype.cpp -i   > $(dir $@)/RegExpPrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/StringConstructor.cpp -i > $(dir $@)/StringConstructor.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/runtime/StringPrototype.cpp -i   > $(dir $@)/StringPrototype.lut.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_hash_table $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/parser/Keywords.table -i         > $(dir $@)/Lexer.lut.h
	
	# KeywordLookupGenerator.py
	python $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/KeywordLookupGenerator.py $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/parser/Keywords.table  > $(dir $@)/KeywordLookup.h
	
	# create_regex_tables 
	python $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/JavaScriptCore/create_regex_tables                                                                                   > $(dir $@)/RegExpJitTables.h
	
	touch $@


# command names used by some of the extracted generator commands
DEL_FILE := rm
MOVE := mv

QT_DEFINES = "LANGUAGE_JAVASCRIPT=1 ENABLE_3D_RENDERING=1 ENABLE_BLOB=1 ENABLE_CHANNEL_MESSAGING=1 ENABLE_CSS_BOX_DECORATION_BREAK=1 ENABLE_CSS_COMPOSITING=1 ENABLE_CSS_EXCLUSIONS=1 ENABLE_CSS_FILTERS=1 ENABLE_CSS_IMAGE_SET=1 ENABLE_CSS_REGIONS=1 ENABLE_CSS_STICKY_POSITION=1 ENABLE_DATALIST_ELEMENT=1 ENABLE_DETAILS_ELEMENT=1 ENABLE_FAST_MOBILE_SCROLLING=1 ENABLE_FILTERS=1 ENABLE_FTPDIR=1 ENABLE_GESTURE_EVENTS=1 ENABLE_ICONDATABASE=1 ENABLE_IFRAME_SEAMLESS=1 ENABLE_INPUT_TYPE_COLOR=1 ENABLE_INSPECTOR=1 ENABLE_INSPECTOR_SERVER=1 ENABLE_JAVASCRIPT_DEBUGGER=1 ENABLE_LEGACY_NOTIFICATIONS=1 ENABLE_LEGACY_VIEWPORT_ADAPTION=1 ENABLE_LEGACY_VENDOR_PREFIXES=1 ENABLE_LINK_PREFETCH=1 ENABLE_METER_ELEMENT=1 ENABLE_MHTML=1 ENABLE_MUTATION_OBSERVERS=1 ENABLE_NOTIFICATIONS=1 ENABLE_PAGE_VISIBILITY_API=1 ENABLE_PROGRESS_ELEMENT=1 ENABLE_RESOLUTION_MEDIA_QUERY=1 ENABLE_REQUEST_ANIMATION_FRAME=1 ENABLE_SHARED_WORKERS=1 ENABLE_SMOOTH_SCROLLING=1 ENABLE_SQL_DATABASE=1 ENABLE_SVG=1 ENABLE_SVG_FONTS=1 ENABLE_TOUCH_ADJUSTMENT=1 ENABLE_TOUCH_EVENTS=1 ENABLE_WEB_SOCKETS=1 ENABLE_WEB_TIMING=1 ENABLE_WORKERS=1 ENABLE_XHR_TIMEOUT=1 ENABLE_TOUCH_SLIDER=1 ENABLE_ACCELERATED_2D_CANVAS=0 ENABLE_ANIMATION_API=0 ENABLE_BATTERY_STATUS=0 ENABLE_CSP_NEXT=0 ENABLE_CSS_GRID_LAYOUT=0 ENABLE_CSS_HIERARCHIES=0 ENABLE_CSS_IMAGE_ORIENTATION=0 ENABLE_CSS_IMAGE_RESOLUTION=0 ENABLE_CSS_SHADERS=0 ENABLE_CSS_VARIABLES=0 ENABLE_CSS3_BACKGROUND=0 ENABLE_CSS3_CONDITIONAL_RULES=0 ENABLE_CSS3_TEXT=0 ENABLE_DASHBOARD_SUPPORT=0 ENABLE_DATAGRID=0 ENABLE_DATA_TRANSFER_ITEMS=0 ENABLE_DEVICE_ORIENTATION=0 ENABLE_DIRECTORY_UPLOAD=0 ENABLE_DOWNLOAD_ATTRIBUTE=0 ENABLE_FILE_SYSTEM=0 ENABLE_FULLSCREEN_API=0 ENABLE_GAMEPAD=0 ENABLE_GEOLOCATION=0 ENABLE_HIGH_DPI_CANVAS=0 ENABLE_INDEXED_DATABASE=0 ENABLE_INPUT_SPEECH=0 ENABLE_INPUT_TYPE_DATE=0 ENABLE_INPUT_TYPE_DATETIME=0 ENABLE_INPUT_TYPE_DATETIMELOCAL=0 ENABLE_INPUT_TYPE_MONTH=0 ENABLE_INPUT_TYPE_TIME=0 ENABLE_INPUT_TYPE_WEEK=0 ENABLE_LEGACY_CSS_VENDOR_PREFIXES=0 ENABLE_LINK_PRERENDER=0 ENABLE_MATHML=0 ENABLE_MEDIA_SOURCE=0 ENABLE_MEDIA_STATISTICS=0 ENABLE_MEDIA_STREAM=0 ENABLE_MICRODATA=0 ENABLE_NAVIGATOR_CONTENT_UTILS=0 ENABLE_NETSCAPE_PLUGIN_API=0 ENABLE_NETWORK_INFO=0 ENABLE_ORIENTATION_EVENTS=0 ENABLE_PROXIMITY_EVENTS=0 ENABLE_QUOTA=0 ENABLE_SCRIPTED_SPEECH=0 ENABLE_SHADOW_DOM=0 ENABLE_STYLE_SCOPED=0 ENABLE_SVG_DOM_OBJC_BINDINGS=0 ENABLE_TEXT_AUTOSIZING=0 ENABLE_TEXT_NOTIFICATIONS_ONLY=0 ENABLE_TOUCH_ICON_LOADING=0 ENABLE_VIBRATION=0 ENABLE_VIDEO=0 ENABLE_VIDEO_TRACK=0 ENABLE_WEBGL=0 ENABLE_WEB_AUDIO=0 ENABLE_XSLT=0"
QT_EXTRA_DEFINES = "QT_NO_LIBUDEV QT_NO_XCB QT_NO_XKBCOMMON ENABLE_3D_RENDERING=1 ENABLE_BLOB=1 ENABLE_CHANNEL_MESSAGING=1 ENABLE_CSS_BOX_DECORATION_BREAK=1 ENABLE_CSS_COMPOSITING=1 ENABLE_CSS_EXCLUSIONS=1 ENABLE_CSS_FILTERS=1 ENABLE_CSS_IMAGE_SET=1 ENABLE_CSS_REGIONS=1 ENABLE_CSS_STICKY_POSITION=1 ENABLE_DATALIST_ELEMENT=1 ENABLE_DETAILS_ELEMENT=1 ENABLE_FAST_MOBILE_SCROLLING=1 ENABLE_FILTERS=1 ENABLE_FTPDIR=1 ENABLE_GESTURE_EVENTS=1 ENABLE_ICONDATABASE=1 ENABLE_IFRAME_SEAMLESS=1 ENABLE_INPUT_TYPE_COLOR=1 ENABLE_INSPECTOR=1 ENABLE_INSPECTOR_SERVER=1 ENABLE_JAVASCRIPT_DEBUGGER=1 ENABLE_LEGACY_NOTIFICATIONS=1 ENABLE_LEGACY_VIEWPORT_ADAPTION=1 ENABLE_LEGACY_VENDOR_PREFIXES=1 ENABLE_LINK_PREFETCH=1 ENABLE_METER_ELEMENT=1 ENABLE_MHTML=1 ENABLE_MUTATION_OBSERVERS=1 ENABLE_NOTIFICATIONS=1 ENABLE_PAGE_VISIBILITY_API=1 ENABLE_PROGRESS_ELEMENT=1 ENABLE_RESOLUTION_MEDIA_QUERY=1 ENABLE_REQUEST_ANIMATION_FRAME=1 ENABLE_SHARED_WORKERS=1 ENABLE_SMOOTH_SCROLLING=1 ENABLE_SQL_DATABASE=1 ENABLE_SVG=1 ENABLE_SVG_FONTS=1 ENABLE_TOUCH_ADJUSTMENT=1 ENABLE_TOUCH_EVENTS=1 ENABLE_WEB_SOCKETS=1 ENABLE_WEB_TIMING=1 ENABLE_WORKERS=1 ENABLE_XHR_TIMEOUT=1 WTF_USE_TILED_BACKING_STORE=1 HAVE_QTPRINTSUPPORT=1 HAVE_QSTYLE=1 HAVE_QTTESTLIB=1 WTF_USE_LIBJPEG=1 WTF_USE_LIBPNG=1 PLUGIN_ARCHITECTURE_UNSUPPORTED=1 ENABLE_TOUCH_SLIDER=1 ENABLE_ACCELERATED_2D_CANVAS=0 ENABLE_ANIMATION_API=0 ENABLE_BATTERY_STATUS=0 ENABLE_CSP_NEXT=0 ENABLE_CSS_GRID_LAYOUT=0 ENABLE_CSS_HIERARCHIES=0 ENABLE_CSS_IMAGE_ORIENTATION=0 ENABLE_CSS_IMAGE_RESOLUTION=0 ENABLE_CSS_SHADERS=0 ENABLE_CSS_VARIABLES=0 ENABLE_CSS3_BACKGROUND=0 ENABLE_CSS3_CONDITIONAL_RULES=0 ENABLE_CSS3_TEXT=0 ENABLE_DASHBOARD_SUPPORT=0 ENABLE_DATAGRID=0 ENABLE_DATA_TRANSFER_ITEMS=0 ENABLE_DEVICE_ORIENTATION=0 ENABLE_DIRECTORY_UPLOAD=0 ENABLE_DOWNLOAD_ATTRIBUTE=0 ENABLE_FILE_SYSTEM=0 ENABLE_FULLSCREEN_API=0 ENABLE_GAMEPAD=0 ENABLE_GEOLOCATION=0 ENABLE_HIGH_DPI_CANVAS=0 ENABLE_INDEXED_DATABASE=0 ENABLE_INPUT_SPEECH=0 ENABLE_INPUT_TYPE_DATE=0 ENABLE_INPUT_TYPE_DATETIME=0 ENABLE_INPUT_TYPE_DATETIMELOCAL=0 ENABLE_INPUT_TYPE_MONTH=0 ENABLE_INPUT_TYPE_TIME=0 ENABLE_INPUT_TYPE_WEEK=0 ENABLE_LEGACY_CSS_VENDOR_PREFIXES=0 ENABLE_LINK_PRERENDER=0 ENABLE_MATHML=0 ENABLE_MEDIA_SOURCE=0 ENABLE_MEDIA_STATISTICS=0 ENABLE_MEDIA_STREAM=0 ENABLE_MICRODATA=0 ENABLE_NAVIGATOR_CONTENT_UTILS=0 ENABLE_NETSCAPE_PLUGIN_API=0 ENABLE_NETWORK_INFO=0 ENABLE_ORIENTATION_EVENTS=0 ENABLE_PROXIMITY_EVENTS=0 ENABLE_QUOTA=0 ENABLE_SCRIPTED_SPEECH=0 ENABLE_SHADOW_DOM=0 ENABLE_STYLE_SCOPED=0 ENABLE_SVG_DOM_OBJC_BINDINGS=0 ENABLE_TEXT_AUTOSIZING=0 ENABLE_TEXT_NOTIFICATIONS_ONLY=0 ENABLE_TOUCH_ICON_LOADING=0 ENABLE_VIBRATION=0 ENABLE_VIDEO=0 ENABLE_VIDEO_TRACK=0 ENABLE_WEBGL=0 ENABLE_WEB_AUDIO=0 ENABLE_XSLT=0" 
GENERATE_BINDINGS_PL = export "SOURCE_ROOT=$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore" && perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts/generate-bindings.pl --defines $(QT_DEFINES) --generator JS --include Modules/filesystem --include Modules/geolocation --include Modules/indexeddb --include Modules/mediasource --include Modules/notifications --include Modules/quota --include Modules/webaudio --include Modules/webdatabase --include Modules/websockets --include css --include dom --include editing --include fileapi --include html --include html/canvas --include html/shadow --include html/track --include inspector --include loader/appcache --include page --include plugins --include storage --include svg --include testing --include workers --include xml --outputDir $(dir $@) --supplementalDependencyFile $(dir $@)/supplemental_dependency.tmp --preprocessor "$(REP_DIR)/tool/moc/moc -E"
	 
$(REP_DIR)/src/lib/qt5/qtwebkit/Source/WebCore/generated/generated.tag:

	mkdir -p $(dir $@)
	
	bison -d -p xpathyy $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XPathGrammar.y -o $(dir $@)/XPathGrammar.tab.c && $(MOVE) $(dir $@)/XPathGrammar.tab.c $(dir $@)/XPathGrammar.cpp && $(MOVE) $(dir $@)/XPathGrammar.tab.h $(dir $@)/XPathGrammar.h

	# preprocess-idls.pl
	sed -e "s,^,$(CONTRIB_DIR)/$(QT5)/,g" $(dir $@)/../idl_files > $(dir $@)/idl_files.tmp
	touch $(dir $@)/supplemental_dependency.tmp 
	export "CONTRIB_DIR=$(CONTRIB_DIR)" && export "QT5=$(QT5)" && perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts/preprocess-idls.pl --defines $(QT_DEFINES) --idlFilesList $(dir $@)/idl_files.tmp --supplementalDependencyFile $(dir $@)/supplemental_dependency.tmp --idlAttributesFile $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts/IDLAttributes.txt --preprocessor "$(REP_DIR)/tool/moc/moc -E"

	# generate-bindings.pl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/DOMFileSystem.idl 
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/DOMFileSystemSync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/DOMWindowFileSystem.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/DirectoryEntry.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/DirectoryEntrySync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/DirectoryReader.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/DirectoryReaderSync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/EntriesCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/Entry.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/EntryArray.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/EntryArraySync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/EntryCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/EntrySync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/ErrorCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/FileCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/FileEntry.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/FileEntrySync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/FileSystemCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/FileWriter.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/FileWriterCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/Metadata.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/MetadataCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/filesystem/WorkerContextFileSystem.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/geolocation/Geolocation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/geolocation/Geoposition.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/geolocation/NavigatorGeolocation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/geolocation/PositionCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/geolocation/PositionError.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/geolocation/PositionErrorCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/DOMWindowIndexedDatabase.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBAny.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBCursor.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBDatabaseException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBDatabase.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBFactory.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBIndex.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBKey.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBKeyRange.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBObjectStore.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBRequest.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/IDBTransaction.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/indexeddb/WorkerContextIndexedDatabase.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/notifications/DOMWindowNotifications.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/notifications/Notification.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/notifications/NotificationCenter.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/notifications/NotificationPermissionCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/notifications/WorkerContextNotifications.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/quota/DOMWindowQuota.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/quota/StorageInfo.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/quota/StorageInfoErrorCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/quota/StorageInfoQuotaCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/quota/StorageInfoUsageCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioBuffer.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioBufferCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioBufferSourceNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/ChannelMergerNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/ChannelSplitterNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioContext.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioDestinationNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioGain.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/GainNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioListener.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/PannerNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioParam.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioProcessingEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AudioSourceNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/BiquadFilterNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/ConvolverNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/DelayNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/DOMWindowWebAudio.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/DynamicsCompressorNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/ScriptProcessorNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/MediaElementAudioSourceNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/MediaStreamAudioSourceNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/OfflineAudioCompletionEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/OscillatorNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/AnalyserNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/WaveShaperNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webaudio/WaveTable.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/DOMWindowWebDatabase.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/Database.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/DatabaseCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/DatabaseSync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLError.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLResultSet.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLResultSetRowList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLStatementCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLStatementErrorCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLTransaction.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLTransactionCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLTransactionErrorCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLTransactionSync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/SQLTransactionSyncCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/webdatabase/WorkerContextWebDatabase.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/websockets/CloseEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/websockets/DOMWindowWebSocket.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/websockets/WebSocket.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/Modules/websockets/WorkerContextWebSocket.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/Counter.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSCharsetRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSFontFaceRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSImportRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSMediaRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSPageRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSPrimitiveValue.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSRuleList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSStyleDeclaration.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSStyleRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSStyleSheet.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSValue.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSValueList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/MediaList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/MediaQueryList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/Rect.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/RGBColor.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/StyleMedia.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/StyleSheet.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/StyleSheetList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSFilterValue.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSKeyframeRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSKeyframesRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSMatrix.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSMixFunctionValue.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSRegionRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSTransformValue.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitCSSViewportRule.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Attr.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/BeforeLoadEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/CharacterData.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/ClientRect.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/ClientRectList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Clipboard.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/CDATASection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Comment.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/CompositionEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/CustomEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DataTransferItem.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DataTransferItemList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DeviceMotionEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DeviceOrientationEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DocumentFragment.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Document.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DocumentType.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DOMCoreException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DOMError.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DOMImplementation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DOMStringList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DOMStringMap.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Element.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Entity.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/EntityReference.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/ErrorEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Event.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/EventException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/EventTarget.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/HashChangeEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/KeyboardEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MouseEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MessageChannel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MessageEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MessagePort.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MutationCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MutationEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MutationObserver.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/MutationRecord.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/NamedNodeMap.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Node.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/NodeFilter.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/NodeIterator.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/NodeList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Notation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/OverflowEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/PageTransitionEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/PopStateEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/ProcessingInstruction.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/ProgressEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/PropertyNodeList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/RangeException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Range.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/RequestAnimationFrameCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/ShadowRoot.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/StringCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Text.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/TextEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/Touch.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/TouchEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/TouchList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/TreeWalker.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/UIEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/WebKitAnimationEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/WebKitNamedFlow.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DOMNamedFlowCollection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/WebKitTransitionEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/WheelEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/fileapi/Blob.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/fileapi/File.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/fileapi/FileError.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/fileapi/FileException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/fileapi/FileList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/fileapi/FileReader.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/fileapi/FileReaderSync.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/ArrayBufferView.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/ArrayBuffer.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/DataView.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Int8Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Float32Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Float64Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/CanvasGradient.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Int32Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/CanvasPattern.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/CanvasRenderingContext.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/CanvasRenderingContext2D.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/EXTTextureFilterAnisotropic.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/OESStandardDerivatives.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/OESTextureFloat.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/OESVertexArrayObject.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/OESElementIndexUint.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLActiveInfo.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLBuffer.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLCompressedTextureS3TC.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLContextAttributes.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLContextEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLDebugRendererInfo.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLDebugShaders.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLDepthTexture.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLFramebuffer.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLLoseContext.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLProgram.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLRenderbuffer.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLRenderingContext.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLShader.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLShaderPrecisionFormat.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Int16Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLTexture.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLUniformLocation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/WebGLVertexArrayObjectOES.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Uint8Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Uint8ClampedArray.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Uint32Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/canvas/Uint16Array.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/DOMFormData.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/DOMSettableTokenList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/DOMTokenList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/DOMURL.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLAllCollection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLAudioElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLAnchorElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLAppletElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLAreaElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLBaseElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLBaseFontElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLBodyElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLBRElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLButtonElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLCanvasElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLCollection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLDataListElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLDetailsElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLDialogElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLDirectoryElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLDivElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLDListElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLDocument.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLEmbedElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLFieldSetElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLFontElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLFormControlsCollection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLFormElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLFrameElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLFrameSetElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLHeadElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLHeadingElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLHRElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLHtmlElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLIFrameElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLImageElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLInputElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLKeygenElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLLabelElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLLegendElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLLIElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLLinkElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLMapElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLMarqueeElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLMediaElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLMenuElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLMetaElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLMeterElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLModElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLObjectElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLOListElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLOptGroupElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLOptionElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLOptionsCollection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLOutputElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLParagraphElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLParamElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLPreElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLProgressElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLPropertiesCollection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLQuoteElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLScriptElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLSelectElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLSourceElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLSpanElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLStyleElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTableCaptionElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTableCellElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTableColElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTableElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTableRowElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTableSectionElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTextAreaElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTitleElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTrackElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLUListElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLUnknownElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLVideoElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/ImageData.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/MediaController.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/MediaError.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/MicroDataItemValue.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/RadioNodeList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/TextMetrics.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/TimeRanges.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/ValidityState.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/VoidCallback.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/shadow/HTMLContentElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/shadow/HTMLShadowElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/InjectedScriptHost.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/InspectorFrontendHost.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/JavaScriptCallFrame.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/ScriptProfile.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/ScriptProfileNode.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/loader/appcache/DOMApplicationCache.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/BarInfo.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Console.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Coordinates.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Crypto.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/DOMSecurityPolicy.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/DOMSelection.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/DOMWindow.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/EventSource.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/History.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Location.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/MemoryInfo.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Navigator.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Performance.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/PerformanceEntry.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/PerformanceEntryList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/PerformanceNavigation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/PerformanceResourceTiming.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/PerformanceTiming.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Screen.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/SpeechInputEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/SpeechInputResult.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/SpeechInputResultList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/WebKitAnimation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/WebKitAnimationList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/WebKitPoint.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/WorkerNavigator.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/plugins/DOMPlugin.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/plugins/DOMMimeType.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/plugins/DOMPluginArray.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/plugins/DOMMimeTypeArray.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/storage/Storage.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/storage/StorageEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/testing/Internals.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/testing/InternalSettings.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/testing/MallocStatistics.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/workers/AbstractWorker.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/workers/DedicatedWorkerContext.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/workers/SharedWorker.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/workers/SharedWorkerContext.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/workers/Worker.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/workers/WorkerContext.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/workers/WorkerLocation.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/DOMParser.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XMLHttpRequest.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XMLHttpRequestException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XMLHttpRequestProgressEvent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XMLHttpRequestUpload.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XMLSerializer.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XPathNSResolver.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XPathException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XPathExpression.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XPathResult.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XPathEvaluator.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/XSLTProcessor.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAltGlyphDefElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAltGlyphElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAltGlyphItemElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAngle.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimateColorElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimateMotionElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedAngle.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedBoolean.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedEnumeration.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedInteger.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedLength.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedLengthList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedNumber.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedNumberList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedPreserveAspectRatio.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedRect.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedString.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimatedTransformList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimateElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimateTransformElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGAnimationElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGCircleElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGClipPathElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGColor.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGComponentTransferFunctionElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGCursorElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGDefsElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGDescElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGDocument.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGElementInstance.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGElementInstanceList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGEllipseElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGException.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEBlendElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEColorMatrixElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEComponentTransferElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFECompositeElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEConvolveMatrixElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEDiffuseLightingElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEDisplacementMapElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEDistantLightElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEDropShadowElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEFloodElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEFuncAElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEFuncBElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEFuncGElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEFuncRElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEGaussianBlurElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEImageElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEMergeElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEMergeNodeElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEMorphologyElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEOffsetElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFEPointLightElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFESpecularLightingElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFESpotLightElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFETileElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFETurbulenceElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFilterElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFontElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFontFaceElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFontFaceFormatElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFontFaceNameElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFontFaceSrcElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGFontFaceUriElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGForeignObjectElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGGElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGGlyphElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGGlyphRefElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGGradientElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGHKernElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGImageElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGLength.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGLengthList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGLinearGradientElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGLineElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGMarkerElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGMaskElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGMatrix.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGMetadataElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGMissingGlyphElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGMPathElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGNumber.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGNumberList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPaint.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegArcAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegArcRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegClosePath.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoCubicAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoCubicRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoCubicSmoothAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoCubicSmoothRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoQuadraticAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoQuadraticRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoQuadraticSmoothAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegCurvetoQuadraticSmoothRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSeg.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegLinetoAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegLinetoHorizontalAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegLinetoHorizontalRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegLinetoRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegLinetoVerticalAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegLinetoVerticalRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegMovetoAbs.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPathSegMovetoRel.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPatternElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPoint.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPointList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPolygonElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPolylineElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGPreserveAspectRatio.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGRadialGradientElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGRectElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGRect.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGRenderingIntent.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGScriptElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGSetElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGStopElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGStringList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGStyleElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGSVGElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGSwitchElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGSymbolElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTextContentElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTextElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTextPathElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTextPositioningElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTitleElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTransform.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTransformList.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTRefElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGTSpanElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGUnitTypes.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGUseElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGViewElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGVKernElement.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGViewSpec.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGZoomAndPan.idl
	$(GENERATE_BINDINGS_PL) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/SVGZoomEvent.idl

	# generate-webkit-version.pl
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/../WebKit/scripts/generate-webkitversion.pl --config $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/../WebKit/mac/Configurations/Version.xcconfig --outputDir $(dir $@)/

	# make-css-file-arrays.pl
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/make-css-file-arrays.pl $(dir $@)/UserAgentStyleSheets.h $(dir $@)/UserAgentStyleSheetsData.cpp $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/html.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/quirks.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/mathml.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/svg.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/view-source.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/fullscreen.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/mediaControls.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/mediaControlsQt.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/mediaControlsQtFullscreen.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/themeQtNoListboxes.css $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/mobileThemeQt.css

	# make-dom-exceptions.pl	
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_dom_exceptions.pl --input $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/DOMExceptions.in --outputDir $(dir $@)
	
	# make_event_factory.pl
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_event_factory.pl --input $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/EventNames.in --outputDir $(dir $@)
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_event_factory.pl --input $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/EventTargetFactory.in --outputDir $(dir $@)
	
	# make-hash-tools.pl
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/make-hash-tools.pl $(dir $@) $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/platform/ColorData.gperf	
	
	# make_names.pl
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_names.pl --tags $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/mathml/mathtags.in --attrs $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/mathml/mathattrs.in          --extraDefines $(QT_EXTRA_DEFINES) --preprocessor "$(REP_DIR)/tool/moc/moc -E" --factory --wrapperFactory --outputDir $(dir $@)
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_names.pl --tags $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLTagNames.in --attrs $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/HTMLAttributeNames.in --extraDefines $(QT_EXTRA_DEFINES) --preprocessor "$(REP_DIR)/tool/moc/moc -E" --factory --wrapperFactory --outputDir $(dir $@)
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_names.pl --fonts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/WebKitFontFamilyNames.in --outputDir $(dir $@)
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_names.pl --tags $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/svgtags.in --attrs $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/svgattrs.in                  --extraDefines $(QT_EXTRA_DEFINES) --preprocessor "$(REP_DIR)/tool/moc/moc -E" --factory --wrapperFactory --outputDir $(dir $@)
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_names.pl --attrs $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/xmlnsattrs.in --preprocessor "$(REP_DIR)/tool/moc/moc -E" --outputDir $(dir $@)
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_names.pl --attrs $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/svg/xlinkattrs.in --preprocessor "$(REP_DIR)/tool/moc/moc -E" --outputDir $(dir $@)
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/dom/make_names.pl --attrs $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/xml/xmlattrs.in --preprocessor "$(REP_DIR)/tool/moc/moc -E" --outputDir $(dir $@)

	# make_settings.pl
	perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/make_settings.pl --input $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/page/Settings.in --outputDir $(dir $@)

	# makeprop.pl
	perl -ne "print $1" $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSPropertyNames.in $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/SVGCSSPropertyNames.in > $(dir $@)/CSSPropertyNames.in && cd $(dir $@) && perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/makeprop.pl --defines $(QT_DEFINES) --preprocessor "$(REP_DIR)/tool/moc/moc -E" $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSPropertyNames.in && $(DEL_FILE) CSSPropertyNames.in CSSPropertyNames.gperf

	# makegrammar.pl
	perl -I $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/makegrammar.pl --outputDir $(dir $@) --extraDefines $(QT_EXTRA_DEFINES) --preprocessor "$(REP_DIR)/tool/moc/moc -E" --symbolsPrefix cssyy $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSGrammar.y.in

	# makevalues.pl	
	perl -ne "print $1" $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSValueKeywords.in $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/SVGCSSValueKeywords.in > $(dir $@)/CSSValueKeywords.in && cd $(dir $@) && perl -I$(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/bindings/scripts $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/makevalues.pl --defines $(QT_DEFINES) --preprocessor "$(REP_DIR)/tool/moc/moc -E" $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/css/CSSValueKeywords.in && $(DEL_FILE) CSSValueKeywords.in CSSValueKeywords.gperf

	# xxd.pl
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/xxd.pl InspectorOverlayPage_html $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/InspectorOverlayPage.html $(dir $@)/InspectorOverlayPage.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/xxd.pl InjectedScriptSource_js $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/InjectedScriptSource.js $(dir $@)/InjectedScriptSource.h
	perl $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/xxd.pl InjectedScriptCanvasModuleSource_js $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/InjectedScriptCanvasModuleSource.js $(dir $@)/InjectedScriptCanvasModuleSource.h

	# CodeGeneratorInspector.py
	python $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/CodeGeneratorInspector.py $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/inspector/Inspector.json --output_h_dir $(dir $@) --output_cpp_dir $(dir $@)
	
	# create-html-entity-table
	python $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/parser/create-html-entity-table -o $(dir $@)/HTMLEntityTable.cpp $(CONTRIB_DIR)/$(QT5)/qtwebkit/Source/WebCore/html/parser/HTMLEntityNames.in

	touch $@


prepare: $(CONTRIB_DIR)/$(QT5) \
         $(CONTRIB_DIR)/$(QTSCRIPTCLASSIC) \
         tools \
         $(REP_DIR)/src/lib/qt5/qtwebkit/Source/JavaScriptCore/generated/generated.tag \
         $(REP_DIR)/src/lib/qt5/qtwebkit/Source/WebCore/generated/generated.tag

tools:
	$(VERBOSE)make -C tool

clean:
	$(VERBOSE)make -C tool clean
	$(VERBOSE)rm -rf $(CONTRIB_DIR)

cleanall: clean
	$(VERBOSE)make -C tool distclean
	$(VERBOSE)rm -rf $(DOWNLOAD_DIR)
