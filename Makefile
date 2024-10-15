# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
DEBUGFLAGS = -g
RELEASEFLAGS = -O3

# Paths to source files and include directories
SRCDIR = NEPath-master
INCLUDES = -I$(SRCDIR)

# Source files
SRCS = main.cpp \
       demos.cpp \
       $(SRCDIR)/Basic.cpp \
       $(SRCDIR)/clipper.cpp \
       $(SRCDIR)/ContourParallel.cpp \
       $(SRCDIR)/Curve.cpp \
       $(SRCDIR)/DirectionParallel.cpp \
       $(SRCDIR)/FileAgent.cpp \
       $(SRCDIR)/NonEquidistant.cpp \
       $(SRCDIR)/path.cpp \
       $(SRCDIR)/NEPathPlanner.cpp \
       $(SRCDIR)/PlanningOptions.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = nepath

# Default build: release
all: release

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Release build
release: CXXFLAGS += $(RELEASEFLAGS)
release: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)
