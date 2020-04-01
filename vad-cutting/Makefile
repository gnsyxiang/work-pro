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

# ----------
# output dir
# ----------
OBJ_DIR ?= objs
LIB_DIR ?= lib
INC_DIR ?= .
SRC_DIR ?= .
TST_DIR ?= test

# --------
# compiler
# --------
SYSTEM_32_64 	?= -m32
#TARGET_SYSTEM   := x1800

ifeq ($(TARGET_SYSTEM), x1800)
	GCC_PATH 	:= ~/office/ingenic/gcc/mips-gcc520-32bit/bin
	GCC_NAME 	:= mips-linux-gnu-

	CROSS_TOOL 	:= $(GCC_PATH)/$(GCC_NAME)
	LDFLAGS 	:=
else
	LDFLAGS 	:=
endif

CC 	 	:= $(Q)$(CROSS_TOOL)gcc
CXX 	:= $(Q)$(CROSS_TOOL)g++
STRIP  	:= $(Q)$(CROSS_TOOL)strip

# ------
# cflags
# ------

CFLAGS     	:= -Wall -Werror -std=gnu99 $(SYSTEM_32_64)
CFLAGS 		+= -I$(INC_DIR) -I../include -I../include/base-lib

DEBUG_SWITCH := debug

ifeq ($(DEBUG_SWITCH), debug)
	CFLAGS 	+= -g
else
	CFLAGS  += -O2 -Wno-error=unused-result -Werror=return-type
endif

# -------
# ldflags
# -------
LDFLAGS 	+= -Wl,-rpath=../lib $(SYSTEM_32_64)
LDFLAGS 	+= -L../lib
LDFLAGS 	+= -lbase-lib
LDFLAGS 	+= -lMNPCVAD -lm
LDFLAGS 	+= -lpthread

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

#################################################

all: $(TARGET_DEMO)

$(TARGET_DEMO): $(OBJS)
	$(ECHO) $(MSG_LD) $@
	$(CC) $^ $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

# --------
# make *.c
# --------
$(OBJ_C): $(OBJ_DIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(ECHO) $(MSG_CC) $<
	$(CC) -c $(CFLAGS) $< -o $@

$(DEP_C): $(OBJ_DIR)/%.d : %.c
	$(MKDIR) $(dir $@);
	$(CC) -MM $(CFLAGS) -MT $(@:%.d=%.o) $< >$@

sinclude $(DEPS)

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
	$(RM) vad_cutting

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

