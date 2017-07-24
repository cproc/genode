CC_OPT  = -DINCLUDE_SUFFIX_CPU=_x86 -DAMD64 -DHOTSPOT_LIB_ARCH='"amd64"'

INC_DIR = $(call select_from_ports,jdk)/src/app/jdk/hotspot/src/cpu/x86/vm

SRC_CONE = cpu/x86/vm/c1_CodeStubs_x86.cpp \
           cpu/x86/vm/c1_FpuStackSim_x86.cpp \
           cpu/x86/vm/c1_FrameMap_x86.cpp \
           cpu/x86/vm/c1_LinearScan_x86.cpp \
           cpu/x86/vm/c1_LIRAssembler_x86.cpp \
           cpu/x86/vm/c1_LIRGenerator_x86.cpp \
           cpu/x86/vm/c1_LIR_x86.cpp \
           cpu/x86/vm/c1_MacroAssembler_x86.cpp \
           cpu/x86/vm/c1_Runtime1_x86.cpp

SRC_CTWO = cpu/x86/vm/c2_init_x86.cpp

SRC_CC = cpu/x86/vm/abstractInterpreter_x86.cpp \
         cpu/x86/vm/assembler_x86.cpp \
         cpu/x86/vm/compiledIC_x86.cpp \
         cpu/x86/vm/debug_x86.cpp \
         cpu/x86/vm/depChecker_x86.cpp \
         cpu/x86/vm/frame_x86.cpp \
         cpu/x86/vm/icache_x86.cpp \
         cpu/x86/vm/icBuffer_x86.cpp \
         cpu/x86/vm/interp_masm_x86.cpp \
         cpu/x86/vm/interpreterRT_x86_64.cpp \
         cpu/x86/vm/jniFastGetField_x86_64.cpp \
         cpu/x86/vm/jvmciCodeInstaller_x86.cpp \
         cpu/x86/vm/macroAssembler_x86_cos.cpp \
         cpu/x86/vm/macroAssembler_x86.cpp \
         cpu/x86/vm/macroAssembler_x86_exp.cpp \
         cpu/x86/vm/macroAssembler_x86_log10.cpp \
         cpu/x86/vm/macroAssembler_x86_log.cpp \
         cpu/x86/vm/macroAssembler_x86_pow.cpp \
         cpu/x86/vm/macroAssembler_x86_sha.cpp \
         cpu/x86/vm/macroAssembler_x86_sin.cpp \
         cpu/x86/vm/macroAssembler_x86_tan.cpp \
         cpu/x86/vm/metaspaceShared_x86_64.cpp \
         cpu/x86/vm/methodHandles_x86.cpp \
         cpu/x86/vm/nativeInst_x86.cpp \
         cpu/x86/vm/register_definitions_x86.cpp \
         cpu/x86/vm/registerMap_x86.cpp \
         cpu/x86/vm/register_x86.cpp \
         cpu/x86/vm/relocInfo_x86.cpp \
         cpu/x86/vm/runtime_x86_64.cpp \
         cpu/x86/vm/sharedRuntime_x86_64.cpp \
         cpu/x86/vm/sharedRuntime_x86.cpp \
         cpu/x86/vm/stubGenerator_x86_64.cpp \
         cpu/x86/vm/stubRoutines_x86_64.cpp \
         cpu/x86/vm/stubRoutines_x86.cpp \
         cpu/x86/vm/templateInterpreterGenerator_x86_64.cpp \
         cpu/x86/vm/templateInterpreterGenerator_x86.cpp \
         cpu/x86/vm/templateTable_x86.cpp \
         cpu/x86/vm/vmreg_x86.cpp \
         cpu/x86/vm/vm_version_x86.cpp \
         cpu/x86/vm/vtableStubs_x86_64.cpp \
         os_cpu/bsd_x86/vm/assembler_bsd_x86.cpp \
         os_cpu/bsd_x86/vm/os_bsd_x86.cpp \
         os_cpu/bsd_x86/vm/thread_bsd_x86.cpp \
         os_cpu/bsd_x86/vm/vm_version_bsd_x86.cpp

include $(REP_DIR)/lib/mk/jvm.inc
