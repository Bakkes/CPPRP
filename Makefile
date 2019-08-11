TARGET_BIN ?= test
BUILD_DIR ?= build
SRC_DIRS ?= CPPRP \
						CPPRPTest 
						
SRCS := $(shell find $(SRC_DIRS) -maxdepth 1 -name *.cpp) ${SRC_FILES}
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

RELEASE ?= 0
ifeq ($(RELEASE),1)
	DEBUG ?= 0
else
	DEBUG ?= 1
endif
BUILDFLAGS := -DDEBUG=$(DEBUG) -DRELEASE=$(RELEASE)
CXX = g++-9
CXXFLAGS ?= -fPIC -std=c++17 $(INC_FLAGS) -w -fopenmp -march=haswell -DPARSE_UNSAFE#-DPARSE_UNSAFE #-Wall -Wextra -Werror
LDFLAGS ?= ${LIB_FLAGS}
ifeq ($(RELEASE),0)
	CXXFLAGS += -O0 -ggdb
else
	CXXFLAGS += -O3 -s
endif
all: $(BUILD_DIR)/${TARGET_BIN}
$(BUILD_DIR)/${TARGET_BIN}: $(OBJS) 
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(LDFLAGS) -pthread
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(BUILDFLAGS) $(CXXFLAGS) -o $@ $<
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
