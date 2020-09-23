TARGET_BIN ?= test
BUILD_DIR ?= build
SRC_DIRS ?= CPPRP \
			CPPRP/exceptions \
						CPPRPJSON
						
										#		CPPRPTest \
						

INC_DIRS ?= CPPRP \
	./CPPRP/data \
	./CPPRP/exceptions \
	./CPPRP/generated \
	./CPPRP/bitreaders \
	./libs/rapidjson/include/ \
						CPPRPJSON

SRCS := $(shell find $(SRC_DIRS) -maxdepth 1 -name *.cpp) ${SRC_FILES}
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_FILES := $(shell find $(INC_DIRS) -name *.h) INC_FLAGS := $(INC_DIRS:%=-I%)
INC_FLAGS := $(INC_DIRS:%=-I%)

RELEASE ?= 0
ifeq ($(RELEASE),1)
	DEBUG ?= 0
else
	DEBUG ?= 1
endif
BUILDFLAGS := -DDEBUG=$(DEBUG) -DRELEASE=$(RELEASE)
CXX ?= g++-9
CXXFLAGS ?= -fPIC -std=c++17 -static-libstdc++ $(INC_FLAGS) -w  -Wfatal-errors -DCPPRP_PRETTYSUPPORT -lstdc++fs #-DPARSE_UNSAFE 
LDFLAGS ?= ${LIB_FLAGS}
ifeq ($(RELEASE),0)
	CXXFLAGS += -O0 -ggdb
else
	CXXFLAGS += -O3 -s -g -DNDEBUG #-s
endif
all: preprocess $(BUILD_DIR)/${TARGET_BIN}
$(BUILD_DIR)/${TARGET_BIN}: $(OBJS) 
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(LDFLAGS) -pthread
	
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(BUILDFLAGS) $(CXXFLAGS) -o $@ $<
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
preprocess:
	 > "./CPPRP/generated/GameClassMacros.h"
	 > "./CPPRP/generated/ClassExtensions.h"
	 > "./CPPRP/generated/NetworkDataParsersGenerated.h"
	 > "./CPPRPJSON/GeneratedSerializeFunctions.h"
	python3 "./scripts/preprocessgameclass.py" "./CPPRP/data/GameClasses.h" json > "./CPPRP/generated/GameClassMacros.h"
	python3 "./scripts/preprocessgameclass.py" "./CPPRP/data/GameClasses.h" classext > "./CPPRP/generated/ClassExtensions.h"
	python3 "./scripts/preprocessnetworkdata.py" "./CPPRP/data/NetworkData.h" consume > "./CPPRP/generated/NetworkDataParsersGenerated.h"
	python3 "./scripts/preprocessnetworkdata.py" "./CPPRP/data/NetworkData.h" json > "./CPPRPJSON/GeneratedSerializeFunctions.h"