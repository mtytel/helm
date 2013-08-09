# Copyright 2013 Little IO
#
# laf is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# laf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with laf.  If not, see <http://www.gnu.org/licenses/>.

LAF_LIBRARY = laf.a
CXXFLAGS += -g -Wall -Wextra -O3
OUTPUT_DIR = debug
SOURCE_DIR = src

# Defines for building tests
TEST_EXEC = laf_test
TEST_DIR = test
TESTS_DIR = $(TEST_DIR)/tests
GTEST_DIR = $(TEST_DIR)/gtest
TEST_CPPFLAGS += -isystem $(GTEST_DIR)/include
TEST_CXXFLAGS += -g -Wall -Wextra

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

OBJS := $(patsubst $(SOURCE_DIR)/%.cpp,$(OUTPUT_DIR)/$(SOURCE_DIR)/%.o, \
        $(wildcard $(SOURCE_DIR)/*.cpp))

TEST_OBJS := $(patsubst $(TESTS_DIR)/%.cpp,$(OUTPUT_DIR)/$(TESTS_DIR)/%.o, \
             $(wildcard $(TESTS_DIR)/*.cpp))

all: directory $(LAF_LIBRARY)

tests: test_directory all $(TEST_EXEC)

clean:
	rm -rf $(OUTPUT_DIR) $(LAF_LIBRARY) $(TEST_EXEC)

# Build Directories
directory:
	@mkdir -p $(OUTPUT_DIR)/$(SOURCE_DIR)

test_directory:
	@mkdir -p $(OUTPUT_DIR)/$(TESTS_DIR)

# Building
$(LAF_LIBRARY): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $^
	@echo 'Complete'
	@echo ' '

$(OUTPUT_DIR)/$(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@echo 'Building $@'
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Testing
$(TEST_EXEC): $(TEST_OBJS) $(LAF_LIBRARY) $(GTEST_DIR)/gtest_main.a
	@echo 'Building Testing Executable: $@'
	@$(CXX) $(TEST_CPPFLAGS) $(TEST_CXXFLAGS) -lpthread $^ -o $@
	@echo 'Complete'
	@echo ' '

$(OUTPUT_DIR)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.cpp $(GTEST_HEADERS)
	@echo 'Building $@'
	@$(CXX) $(TEST_CPPFLAGS) $(TEST_CXXFLAGS) -I $(SOURCE_DIR) -c $< -o $@
