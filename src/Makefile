# export LD_LIBRARY_PATH=./jsoncpp:.

APP_NAME = simplenote.exe
STATIC_LIB_NAME = libsimplenote.a
SHARED_LIB_NAME = libsimplenote.so
TEST_RUNNER_NAME = runner

CXX = g++
AR = ar
TEST = cxxtestgen

CXX_FLAGS = -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x
LD_FLAGS = -L. -lsimplenote
DBG_FLAGS = -g -DDEBUG
AR_FLAGS = crvs
SHARED_FLAGS = -shared -fPIC
TEST_FLAGS = -o $(TEST_RUNNER_NAME).cpp --error-printer

SRC_FILES = simplenote.cpp note.cpp base64.cpp helpers.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
DBG_FILES = main.cpp
TEST_FILES = tests/simplenote_tests.cpp

DEPS = -lcurl -L./jsoncpp -ljsoncpp

debug: CXX_FLAGS += $(DBG_FLAGS)
debug: lib
	$(CXX) $(CXX_FLAGS) $(DBG_FILES) $(LD_FLAGS) -o $(APP_NAME)

tests: test

test: lib
	$(TEST) $(TEST_FLAGS) $(TEST_FILES)
	$(CXX) $(CXX_FLAGS) $(TEST_RUNNER_NAME).cpp -o $(TEST_RUNNER_NAME).exe $(LD_FLAGS)

lib: static shared

static: $(OBJ_FILES)
	$(AR) $(AR_FLAGS) $(STATIC_LIB_NAME) $?

shared:
	$(CXX) $(CXX_FLAGS) $(SHARED_FLAGS) $(SRC_FILES) -o $(SHARED_LIB_NAME) $(DEPS)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@ # compile the .cpp files into .o files

.PHONY: clean
clean:
	rm -f *.{out,o,i,exe,swp,a,so}
	rm -rf *~