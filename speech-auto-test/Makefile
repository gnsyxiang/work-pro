# ===============================================================
# 
# Release under GPLv2.
# 
# @file    Makefile
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    28/02 2018 17:02
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        28/02 2018      create the file
# 
#     last modified: 28/02 2018 17:02
# ===============================================================

# --------------
# target setting
# --------------
TARGET_DEMO 	?= main
TARGET_LIB_NAME ?= work-pro

# ------------------
# output information
# ------------------
MSG_CC 	?= CC
MSG_LD 	?= LD
MSG_LIB ?= LIB_COPY
MSG_INC ?= INC_COPY

# ----------------
# cmd redefinition
# ----------------
Q 		:= @

RM 		:= $(Q)rm -rf
ECHO 	:= $(Q)echo
MKDIR 	:= $(Q)mkdir -p
LN 		:= $(Q)ln -s
CP 		:= $(Q)cp -ar

# -------
# version
# -------
MAJOR_VERSION ?= 1
MINOR_VERSION ?= 0
BUG_VERSION   ?= 0

TARGET_LIB 	  := lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION).$(MINOR_VERSION).$(BUG_VERSION)

# ----------
# output dir
# ----------
OBJ_DIR ?= objs
LIB_DIR ?= lib
INC_DIR ?= include
SRC_DIR ?= src
TST_DIR ?= test

TARGET_PATH := $(LIB_DIR)/$(TARGET_LIB)

# --------
# compiler
# --------
#TARGET_SYSTEM   := x1800

ifeq ($(TARGET_SYSTEM), x1800)
	GCC_PATH 	:= ~/office/ingenic/gcc/mips-gcc520-32bit/bin
	GCC_NAME 	:= mips-linux-gnu-

	CROSS_TOOL 	:= $(GCC_PATH)/$(GCC_NAME)
	LDFLAGS 	:= -T configs/ldscript-mips.lds
else
	LDFLAGS 	:= -T configs/ldscript.lds
endif

CC 	 	:= $(Q)$(CROSS_TOOL)gcc
CXX 	:= $(Q)$(CROSS_TOOL)g++
STRIP  	:= $(Q)$(CROSS_TOOL)strip

# ------
# cflags
# ------
#DEBUG_SWITCH := debug

ifeq ($(DEBUG_SWITCH), debug)
	CFLAGS     := -g
else
	CFLAGS     := -O2 -Wno-error=unused-result -Werror=return-type
endif

CFLAGS     += -Wall -Werror -std=gnu99
CFLAGS     += -I$(INC_DIR) -I../../base-lib/include
LIB_CFLAGS := $(CFLAGS) -fPIC

# -------
# ldflags
# -------
SO_NAME 	:= -Wl,-soname,lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION)

LDFLAGS 	+= -Wl,-rpath=./lib
LDFLAGS 	+= -L./lib
LDFLAGS 	+= -l$(TARGET_LIB_NAME) -lbase-lib
LDFLAGS 	+= -lpthread
LIB_LDFLAGS := $(SO_NAME) -shared

# -------
# h files
# -------
INC_C := $(wildcard $(INC_DIR)/*.h)

# -------
# c files
# -------
SRC_C := $(wildcard $(SRC_DIR)/*.c)
OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_C))
DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(SRC_C))
DEPS  ?= $(DEP_C)
OBJS  ?= $(OBJ_C)

TARGET_DEMO_SRC_C := $(wildcard $(TST_DIR)/*.c)
TARGET_DEMO_OBJ_C := $(patsubst %.c, $(OBJ_DIR)/%.o, $(TARGET_DEMO_SRC_C))
TARGET_DEMO_DEP_C := $(patsubst %.c, $(OBJ_DIR)/%.d, $(TARGET_DEMO_SRC_C))
TARGET_DEMO_DEPS  ?= $(TARGET_DEMO_DEP_C)
TARGET_DEMO_OBJS  ?= $(TARGET_DEMO_OBJ_C)

#################################################

all: $(TARGET_PATH) handle_lib $(TARGET_DEMO)

$(TARGET_PATH): $(OBJS)
	$(ECHO) $(MSG_LD) $@
	$(MKDIR) $(LIB_DIR)
	$(CC) $^ $(LIB_LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

$(TARGET_DEMO): $(TARGET_DEMO_OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $^ $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

handle_lib: clean_lib ln_lib

clean_lib:
	$(RM) $(LIB_DIR)/lib$(TARGET_LIB_NAME).so
	$(RM) $(LIB_DIR)/lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION)

ln_lib:
	$(LN) $(TARGET_LIB) $(LIB_DIR)/lib$(TARGET_LIB_NAME).so
	$(LN) $(TARGET_LIB) $(LIB_DIR)/lib$(TARGET_LIB_NAME).so.$(MAJOR_VERSION)

# --------
# make *.c
# --------
$(OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(LIB_CFLAGS) $< -o $@

$(DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(LIB_CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(DEPS)

$(TARGET_DEMO_OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET_DEMO_DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(TARGET_DEMO_DEPS)

#################################################

debug:
	echo $(SRC_DIR)

push:
	adb push ./lib /xia/base_lib/lib/
	adb push $(TARGET_DEMO) /xia/base_lib/

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TARGET_DEMO_OBJS)
	$(RM) $(TARGET_DEMO_DEPS)
	$(RM) $(TARGET_DEMO)

distclean: clean index-clean
	$(RM) $(OBJ_DIR)
	$(RM) $(LIB_DIR)

index: index-clean
	$(ECHO) generate index
	$(Q)ctags -R
	$(Q)cscope -Rbkq

index-clean:
	$(RM) *.out
	$(RM) tags

note:
	doxygen configs/Doxyfile

.PHONY: all clean distclean debug

