/* License Info */
static const char qt_configure_licensee_str          [256 + 12] = "qt_lcnsuser=Open Source";
static const char qt_configure_licensed_products_str [256 + 12] = "qt_lcnsprod=OpenSource";

/* Installation date */
static const char qt_configure_installation          [12+11]    = "qt_instdate=2013-05-23";

/* Installation Info */
static const char qt_configure_prefix_path_strs[][256 + 12] = {
    "qt_prfxpath=:/usr/local/Qt-5.0.2",
    "qt_docspath=:/usr/local/Qt-5.0.2/doc",
    "qt_hdrspath=:/usr/local/Qt-5.0.2/include",
    "qt_libspath=:/usr/local/Qt-5.0.2/lib",
    "qt_lbexpath=:/usr/local/Qt-5.0.2/libexec",
    "qt_binspath=:/usr/local/Qt-5.0.2/bin",
    "qt_plugpath=:/usr/local/Qt-5.0.2/plugins",
    "qt_impspath=:/usr/local/Qt-5.0.2/imports",
    "qt_qml2path=:/usr/local/Qt-5.0.2/qml",
    "qt_adatpath=:/usr/local/Qt-5.0.2",
    "qt_datapath=:/usr/local/Qt-5.0.2",
    "qt_trnspath=:/usr/local/Qt-5.0.2/translations",
    "qt_xmplpath=:/usr/local/Qt-5.0.2/examples",
    "qt_tstspath=:/usr/local/Qt-5.0.2/tests",
#ifdef QT_BUILD_QMAKE
    "qt_ssrtpath=",
    "qt_hpfxpath=:/usr/local/Qt-5.0.2",
    "qt_hbinpath=:/usr/local/Qt-5.0.2/bin",
    "qt_hdatpath=:/usr/local/Qt-5.0.2",
    "qt_targspec=genode-g++",
    "qt_hostspec=linux-g++-64",
#endif
};
static const char qt_configure_settings_path_str[256 + 12] = "qt_stngpath=:/usr/local/Qt-5.0.2/etc/xdg";

/* strlen( "qt_lcnsxxxx" ) == 12 */
#define QT_CONFIGURE_LICENSEE qt_configure_licensee_str + 12;
#define QT_CONFIGURE_LICENSED_PRODUCTS qt_configure_licensed_products_str + 12;

#define QT_CONFIGURE_SETTINGS_PATH qt_configure_settings_path_str + 12;
