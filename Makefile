RACK_DIR ?= ../Rack-SDK

# The Rack plugin builds from the repo root: the VCV Library toolchain runs
# clean/cleandep/dep/dist here, and plugin.mk reads ./plugin.json. Everything
# else (tests, codegen, parity checks) lives in test.mk and needs no SDK.
HARNESS_TARGETS := test unittests goldens gen crosscheck layoutcheck artcheck \
                   labelcheck sizecheck ramcheck smoke patchsmoke eqcheck bench

ifneq ($(wildcard $(RACK_DIR)/plugin.mk),)

# The DROID engine is compiled into the plugin directly (one engine, two builds).
ENGINE_SOURCES := $(wildcard engine/src/*.cpp) \
                  $(wildcard engine/circuits/*.cpp) \
                  $(wildcard engine/gen/*.cpp)

FLAGS += -Iengine

SOURCES += $(wildcard plugin/src/*.cpp) $(ENGINE_SOURCES)
SOURCES += $(wildcard plugin/src/uatbridge/*.cpp)
FLAGS += -DVOIDBOT_GIT_HASH=\"$(shell git rev-parse --short HEAD)\"
# Tag if the build is exactly at one, else the branch name; empty on a
# detached HEAD that isn't at a tag. Shown in every module's context menu.
FLAGS += -DVOIDBOT_GIT_REF=\"$(shell git describe --tags --exact-match 2>/dev/null || git symbolic-ref --short -q HEAD)\"
DISTRIBUTABLES += res

include $(RACK_DIR)/plugin.mk

# plugin.mk's compile.mk appends -std=c++11 after our flags, so we must strip it
# and force C++17 here (after the include) or the engine sources won't compile.
CXXFLAGS := $(filter-out -std=c++11,$(CXXFLAGS)) -std=c++17

else

.DEFAULT_GOAL := all

all dist install dep cleandep:
	@echo "error: no Rack SDK at RACK_DIR=$(RACK_DIR); pass RACK_DIR=/path/to/Rack-SDK" >&2
	@exit 1

clean:
	rm -rf build

endif

$(HARNESS_TARGETS):
	@$(MAKE) -f test.mk $@

.PHONY: $(HARNESS_TARGETS)
