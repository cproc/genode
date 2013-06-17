include $(REP_DIR)/lib/import/import-icu.mk

ICU_DIR = $(REP_DIR)/contrib/icu

# extracted from $(ICU_DIR)/source/common/Makefile.in and renamed variable to ICU_OBJECTS  

ICU_OBJECTS = errorcode.o putil.o umath.o utypes.o uinvchar.o umutex.o ucln_cmn.o \
uinit.o uobject.o cmemory.o charstr.o \
udata.o ucmndata.o udatamem.o umapfile.o udataswp.o ucol_swp.o utrace.o \
uhash.o uhash_us.o uenum.o ustrenum.o uvector.o ustack.o uvectr32.o uvectr64.o \
ucnv.o ucnv_bld.o ucnv_cnv.o ucnv_io.o ucnv_cb.o ucnv_err.o ucnvlat1.o \
ucnv_u7.o ucnv_u8.o ucnv_u16.o ucnv_u32.o ucnvscsu.o ucnvbocu.o \
ucnv_ext.o ucnvmbcs.o ucnv2022.o ucnvhz.o ucnv_lmb.o ucnvisci.o ucnvdisp.o ucnv_set.o ucnv_ct.o \
uresbund.o ures_cnv.o uresdata.o resbund.o resbund_cnv.o \
messagepattern.o ucat.o locmap.o uloc.o locid.o locutil.o locavailable.o locdispnames.o loclikely.o locresdata.o \
bytestream.o stringpiece.o \
stringtriebuilder.o bytestriebuilder.o \
bytestrie.o bytestrieiterator.o \
ucharstrie.o ucharstriebuilder.o ucharstrieiterator.o \
dictionarydata.o \
appendable.o ustr_cnv.o unistr_cnv.o unistr.o unistr_case.o unistr_props.o \
utf_impl.o ustring.o ustrcase.o ucasemap.o ucasemap_titlecase_brkiter.o cstring.o ustrfmt.o ustrtrns.o ustr_wcs.o utext.o \
unistr_case_locale.o ustrcase_locale.o unistr_titlecase_brkiter.o ustr_titlecase_brkiter.o \
normalizer2impl.o normalizer2.o filterednormalizer2.o normlzr.o unorm.o unormcmp.o unorm_it.o \
chariter.o schriter.o uchriter.o uiter.o \
patternprops.o uchar.o uprops.o ucase.o propname.o ubidi_props.o ubidi.o ubidiwrt.o ubidiln.o ushape.o \
uscript.o uscript_props.o usc_impl.o unames.o \
utrie.o utrie2.o utrie2_builder.o bmpset.o unisetspan.o uset_props.o uniset_props.o uniset_closure.o uset.o uniset.o usetiter.o ruleiter.o caniter.o unifilt.o unifunct.o \
uarrsort.o brkiter.o ubrk.o brkeng.o dictbe.o \
rbbi.o rbbidata.o rbbinode.o rbbirb.o rbbiscan.o rbbisetb.o rbbistbl.o rbbitblb.o \
serv.o servnotf.o servls.o servlk.o servlkf.o servrbf.o servslkf.o \
uidna.o usprep.o uts46.o punycode.o \
util.o util_props.o parsepos.o locbased.o cwchar.o wintz.o mutex.o dtintrv.o ucnvsel.o propsvec.o \
ulist.o uloc_tag.o icudataver.o icuplug.o listformatter.o

SRC_C = $(filter $(ICU_OBJECTS:.o=.c), $(notdir $(wildcard $(ICU_DIR)/source/common/*.c)))
$(info SRC_C = $(SRC_C))

SRC_CC = $(filter $(ICU_OBJECTS:.o=.cpp), $(notdir $(wildcard $(ICU_DIR)/source/common/*.cpp)))
$(info SRC_CC = $(SRC_CC))

CC_OPT += -DU_COMMON_IMPLEMENTATION

LIBS = stdcxx

vpath % $(ICU_DIR)/source/common
