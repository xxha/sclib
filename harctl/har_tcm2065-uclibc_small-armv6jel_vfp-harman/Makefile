# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

JOBS?=1
TARGET_JOBS?=$(JOBS)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = har_tcm2065-uclibc_small-armv6jel_vfp-harman
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := /home/xxha/WindRiver/workspace
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/harctl



#Global Build Macros
PROJECT_TYPE = APP


#BuildSpec specific Build Macros
ARCH = arm
TOOL_PATH = /home/xxha/WindRiver/workspace/tcm0105_prj/export/sysroot/har_tcm2065-glibc_small/x86-linux2/
CROSS_COMPILE = armv6jel_vfp-target-linux-gnueabi-
CROSS_COMPILE_KERNEL = arm-wrs-linux-gnueabi-
ADAPT_DEPS = 
LINUX_KERNEL_VERSION = 2.6.34
CPU_VARIANT = armv6jel_vfp
ROOTFS = uclibc_small
LIBPATH = 
LIBS = 
LAYER = /home/sean/WindRiver/workspace/harman_prj/export/sysroot/har_tcm2065-uclibc_small

IDE_INCLUDES = -I$(PRJ_ROOT_DIR)/include 

IDE_LIBRARIES = 

IDE_DEFINES = 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_Assembler = -g
DEBUGFLAGS_C++-Compiler = -g -mcpu=arm1136jf-s -O0 -fno-omit-frame-pointer -mapcs-frame -pipe
DEBUGFLAGS_C++-Linker = -g
DEBUGFLAGS_C-Compiler = -g -mcpu=arm1136jf-s -O0 -fno-omit-frame-pointer -mapcs-frame -pipe
DEBUGFLAGS_C-Linker = -g
DEBUGFLAGS_Librarian = -g
DEBUGFLAGS_Shared-Library-Linker = -g
else
DEBUGFLAGS_Assembler = 
DEBUGFLAGS_C++-Compiler = -mcpu=arm1136jf-s -Os -fomit-frame-pointer -pipe
DEBUGFLAGS_C++-Linker = 
DEBUGFLAGS_C-Compiler = -mcpu=arm1136jf-s -Os -fomit-frame-pointer -pipe
DEBUGFLAGS_C-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Shared-Library-Linker = 
endif


#Project Targets
PROJECT_TARGETS = harctl/$(MODE_DIR)/harctl.out


#Rules

# harctl
ifeq ($(DEBUG_MODE),1)
harctl/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g -mcpu=arm1136jf-s -O0 -fno-omit-frame-pointer -mapcs-frame -pipe
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C++-Linker = -g
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g -mcpu=arm1136jf-s -O0 -fno-omit-frame-pointer -mapcs-frame -pipe
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C-Linker = -g
harctl/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = -g
harctl/$(MODE_DIR)/% : DEBUGFLAGS_Shared-Library-Linker = -g
else
harctl/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = 
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -mcpu=arm1136jf-s -Os -fomit-frame-pointer -pipe
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C++-Linker = 
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -mcpu=arm1136jf-s -Os -fomit-frame-pointer -pipe
harctl/$(MODE_DIR)/% : DEBUGFLAGS_C-Linker = 
harctl/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
harctl/$(MODE_DIR)/% : DEBUGFLAGS_Shared-Library-Linker = 
endif
harctl/$(MODE_DIR)/% : IDE_INCLUDES = -I$(PRJ_ROOT_DIR)/include 
harctl/$(MODE_DIR)/% : IDE_LIBRARIES = 
harctl/$(MODE_DIR)/% : IDE_DEFINES = 
harctl/$(MODE_DIR)/% : PROJECT_TYPE = APP
harctl/$(MODE_DIR)/% : ARCH = arm
harctl/$(MODE_DIR)/% : TOOL_PATH = /home/xxha/WindRiver/workspace/tcm0105_prj/export/sysroot/har_tcm2065-glibc_small/x86-linux2/
harctl/$(MODE_DIR)/% : CROSS_COMPILE = armv6jel_vfp-target-linux-gnueabi-
harctl/$(MODE_DIR)/% : CROSS_COMPILE_KERNEL = arm-wrs-linux-gnueabi-
harctl/$(MODE_DIR)/% : ADAPT_DEPS = 
harctl/$(MODE_DIR)/% : LINUX_KERNEL_VERSION = 2.6.34
harctl/$(MODE_DIR)/% : CPU_VARIANT = armv6jel_vfp
harctl/$(MODE_DIR)/% : ROOTFS = uclibc_small
harctl/$(MODE_DIR)/% : LIBPATH = 
harctl/$(MODE_DIR)/% : LIBS = 
harctl/$(MODE_DIR)/% : LAYER = /home/sean/WindRiver/workspace/harman_prj/export/sysroot/har_tcm2065-uclibc_small
harctl/$(MODE_DIR)/% : OBJ_DIR := harctl/$(MODE_DIR)

harctl/$(MODE_DIR)/Objects/harctl/src/harctl/main.o : $(PRJ_ROOT_DIR)/src/harctl/main.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo  "building $@ "; $(TOOL_PATH)$(CROSS_COMPILE)gcc $(DEBUGFLAGS_C-Compiler)  $(ADDED_CFLAGS) $(IDE_DEFINES) $(IDE_INCLUDES) $(ADDED_INCLUDES) $(DEFINES) -MD -MP -c -DARCH=$(ARCH) -o "$@" "$<"


harctl/$(MODE_DIR)/Objects/harctl/src/lib/api.o : $(PRJ_ROOT_DIR)/src/lib/api.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo  "building $@ "; $(TOOL_PATH)$(CROSS_COMPILE)gcc $(DEBUGFLAGS_C-Compiler)  $(ADDED_CFLAGS) $(IDE_DEFINES) $(IDE_INCLUDES) $(ADDED_INCLUDES) $(DEFINES) -MD -MP -c -DARCH=$(ARCH) -o "$@" "$<"


harctl/$(MODE_DIR)/Objects/harctl/src/lib/crc32.o : $(PRJ_ROOT_DIR)/src/lib/crc32.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo  "building $@ "; $(TOOL_PATH)$(CROSS_COMPILE)gcc $(DEBUGFLAGS_C-Compiler)  $(ADDED_CFLAGS) $(IDE_DEFINES) $(IDE_INCLUDES) $(ADDED_INCLUDES) $(DEFINES) -MD -MP -c -DARCH=$(ARCH) -o "$@" "$<"


harctl/$(MODE_DIR)/Objects/harctl/src/lib/env.o : $(PRJ_ROOT_DIR)/src/lib/env.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo  "building $@ "; $(TOOL_PATH)$(CROSS_COMPILE)gcc $(DEBUGFLAGS_C-Compiler)  $(ADDED_CFLAGS) $(IDE_DEFINES) $(IDE_INCLUDES) $(ADDED_INCLUDES) $(DEFINES) -MD -MP -c -DARCH=$(ARCH) -o "$@" "$<"


harctl/$(MODE_DIR)/Objects/harctl/src/lib/nand.o : $(PRJ_ROOT_DIR)/src/lib/nand.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo  "building $@ "; $(TOOL_PATH)$(CROSS_COMPILE)gcc $(DEBUGFLAGS_C-Compiler)  $(ADDED_CFLAGS) $(IDE_DEFINES) $(IDE_INCLUDES) $(ADDED_INCLUDES) $(DEFINES) -MD -MP -c -DARCH=$(ARCH) -o "$@" "$<"


OBJECTS_harctl = harctl/$(MODE_DIR)/Objects/harctl/src/harctl/main.o \
	 harctl/$(MODE_DIR)/Objects/harctl/src/lib/api.o \
	 harctl/$(MODE_DIR)/Objects/harctl/src/lib/crc32.o \
	 harctl/$(MODE_DIR)/Objects/harctl/src/lib/env.o \
	 harctl/$(MODE_DIR)/Objects/harctl/src/lib/nand.o

ifeq ($(TARGET_JOBS),1)
harctl/$(MODE_DIR)/harctl.out : $(OBJECTS_harctl)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo  "building $@ "; $(TOOL_PATH)$(CROSS_COMPILE)gcc $(DEBUGFLAGS_C-Linker) -static -o "$@" $(OBJECTS_harctl) $(ADDED_OBJECTS) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)

else
harctl/$(MODE_DIR)/harctl.out : harctl/$(MODE_DIR)/harctl.out_jobs

endif
harctl/$(MODE_DIR)/harctl_compile_file : $(FILE) ;

_clean :: harctl/$(MODE_DIR)/harctl_clean

harctl/$(MODE_DIR)/harctl_clean : 
	$(TRACE_FLAG)if [ -d "harctl" ]; then cd "harctl"; rm -rf $(MODE_DIR); fi

force : 

TARGET_JOBS_RULE?=echo update makefile template
%_jobs : 
	$(TRACE_FLAG)$(TARGET_JOBS_RULE)

DEP_FILES := harctl/$(MODE_DIR)/Objects/harctl/src/harctl/main.d harctl/$(MODE_DIR)/Objects/harctl/src/lib/api.d harctl/$(MODE_DIR)/Objects/harctl/src/lib/crc32.d \
	 harctl/$(MODE_DIR)/Objects/harctl/src/lib/env.d harctl/$(MODE_DIR)/Objects/harctl/src/lib/nand.d
-include $(DEP_FILES)


WIND_SCOPETOOLS_BASE := $(subst \,/,$(WIND_SCOPETOOLS_BASE))

clean_scopetools :
	$(TRACE_FLAG)rm -rf $(PRJ_ROOT_DIR)/.coveragescope/db

CLEAN_STEP := clean_scopetools


-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

TARGET_JOBS_RULE=$(MAKE) -f $(MAKEFILE) --jobs $(TARGET_JOBS) $(MFLAGS) $* TARGET_JOBS=1
ifeq ($(JOBS),1)
main_all : external_build  $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"
else
main_all : external_build 
	@$(MAKE) -f $(MAKEFILE) --jobs $(JOBS) $(MFLAGS) $(PROJECT_TARGETS) TARGET_JOBS=1 &&\
	echo "make: built targets of `pwd`"
endif 

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
