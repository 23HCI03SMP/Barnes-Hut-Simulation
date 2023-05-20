SRCDIR := src
BUILDDIR := build

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LDFLAGS := -L C:/msys64/mingw64/include
LDLIBS := -lgsl -lgslcblas -lm

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

$(BUILDDIR):
	mkdir $(BUILDDIR)

main.exe: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@ $(LDLIBS)	