# Created by Matthew Boyea on 2024-01-23
# Last Edited by Matthew Boyea on 2024-01-25

# file extensions
SRC_EXT := cpp
INC_EXT := h
OBJ_EXT := o
DLL_EXT := dll
EXE_EXT := exe

# project directories
SRC_DIR := src
OBJ_DIR := build
TARGET_DIR := bin
STATIC_DIR := static

# linux libs & flags
LIB_DIR := /usr
LIB_INC_DIRS := $(LIB_DIR)/include/SDL2
LIB_LIB_DIRS := $(LIB_DIR)/lib
LIB_DLL_DIRS :=

# windows libs & flags
LIB_DIR := lib
ifneq ($(wildcard $(LIB_DIR)/.),)
	LIB_INC_DIRS := $(shell find $(LIB_DIR) -type d -path "*x86_64*/include/SDL2")
	LIB_LIB_DIRS := $(shell find $(LIB_DIR) -type d -path "*x86_64*/lib")
	LIB_DLL_DIRS := $(shell find $(LIB_DIR) -type d -path "*x86_64*/bin")
	LDFLAGS := $(addprefix -L,-mwindows)
	LDLIBS := $(addprefix -l,mingw32)
endif

# cross-platform compilers & flags
CXX := g++
CXXFLAGS := -std=c++17 -g# -Wall
LDFLAGS := $(LDFLAGS) $(addprefix -L,$(LIB_LIB_DIRS))
LDLIBS := $(LDLIBS) $(addprefix -l,SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)

# target files
EXE_NAME := Game
EXE_PATH := $(TARGET_DIR)/$(EXE_NAME)
SRCS := $(shell find $(SRC_DIR) -type f -name "*.$(SRC_EXT)")
OBJS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SRCS:.$(SRC_EXT)=.$(OBJ_EXT)))
LIB_DLLS := $(shell find $(LIB_DLL_DIRS) -type f -name "*.$(DLL_EXT)")
DLLS := $(addprefix $(TARGET_DIR)/,$(notdir $(LIB_DLLS)))
STATICS := $(patsubst $(STATIC_DIR)/%,$(TARGET_DIR)/%,$(shell find $(STATIC_DIR) -type f -name "*"))

all : $(EXE_PATH).$(EXE_EXT) $(DLLS) $(STATICS)

$(TARGET_DIR)/%: $(STATIC_DIR)/%
	@mkdir -p $(TARGET_DIR)
	@echo "Copying $< into $@..."; install -D "$<" "$@"

define COPY_DLL_template
$(TARGET_DIR)/%.$(DLL_EXT) : $(1)/%.$(DLL_EXT)
	@mkdir -p $(TARGET_DIR)
	@echo "Copying $$(<) into $$(@) ..."; cp "$$(<)" "$$(@)"
endef
$(foreach LIB_DLL_DIR,$(LIB_DLL_DIRS),$(eval $(call COPY_DLL_template,$(LIB_DLL_DIR))))

$(EXE_PATH).$(EXE_EXT) : $(OBJS)
	@mkdir -p $(TARGET_DIR)
	@echo "Linking..."; $(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $(EXE_PATH)

$(OBJ_DIR)/%.$(OBJ_EXT) : $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."; $(CXX) $(CXXFLAGS) $(addprefix -I,$(LIB_INC_DIRS)) -c -o $@ $<

clean :
	@echo "Cleaning..."; $(RM) -r $(OBJ_DIR) $(TARGET_DIR)

.PHONY : all run clean

