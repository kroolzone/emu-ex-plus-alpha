ENV := android
CROSS_COMPILE := 1

android_ndkSysroot := $(ANDROID_NDK_PATH)/platforms/android-$(android_minSDK)/arch-$(android_ndkArch)

android_hasSDK9 := $(shell expr $(android_minSDK) \>= 9)

ifndef targetDir
 ifdef O_RELEASE
  targetDir := target/android-$(android_minSDK)/libs-release/$(android_abi)
 else
  targetDir := target/android-$(android_minSDK)/libs-debug/$(android_abi)
 endif
endif

ifeq ($(android_hasSDK9), 1)
 android_soName := main
else
 android_soName := imagine
endif

ifeq ($(config_compiler),clang)
 # TODO: not 100% working yet
 ifeq ($(origin CC), default)
  CC := clang
 endif
 include $(currPath)/clang.mk
else
 include $(currPath)/gcc.mk
endif

ifndef android_stdcxx
 ifdef cxxExceptions
  android_stdcxx := gnu
 else
  android_stdcxx := stlport
 endif
endif

ifeq ($(android_stdcxx), gnu)
 android_stdcxxLib := $(ANDROID_NDK_PATH)/sources/cxx-stl/gnu-libstdc++/$(gccVersion)/libs/$(android_abi)/libgnustl_static.a
else
 android_stdcxxLib := $(ANDROID_NDK_PATH)/sources/cxx-stl/stlport/libs/$(android_abi)/libstlport_static.a -lstdc++
endif

ifdef ANDROID_APK_SIGNATURE_HASH
 CPPFLAGS += -DANDROID_APK_SIGNATURE_HASH=$(ANDROID_APK_SIGNATURE_HASH)
endif

include $(IMAGINE_PATH)/make/package/stdc++-headers.mk

#BASE_CXXFLAGS += -fno-use-cxa-atexit
# -fstack-protector
COMPILE_FLAGS += -ffunction-sections -fdata-sections \
-Wa,--noexecstack $(android_cpuFlags)
ASMFLAGS += -Wa,--noexecstack $(android_cpuFlags)
LDFLAGS += $(android_cpuFlags)
WARNINGS_CFLAGS += -Wno-psabi
LDFLAGS += -Wl,--no-undefined,-z,noexecstack,-z,relro,-z,now,-soname,lib$(android_soName).so -shared
LDLIBS += -L$(android_ndkSysroot)/usr/lib -lm

CPPFLAGS += -DANDROID --sysroot=$(android_ndkSysroot)
LDFLAGS += -s -Wl,-O1,--gc-sections
OPTIMIZE_LDFLAGS +=