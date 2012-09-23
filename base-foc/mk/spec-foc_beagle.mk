SPECS += foc_arm platform_beagle

include $(call select_from_repositories,mk/spec-platform_beagle.mk)
include $(call select_from_repositories,mk/spec-foc_arm.mk)
