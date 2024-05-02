# Compiler options
CXXFLAGS = -O3 -march=native

# List of source files
SOURCES = bgd.cpp ccsd_multisize.cpp ccsd_multisize_opt.cpp ccsd_onesize.cpp ccsd_onesize_opt.cpp example1.cpp example1_opt.cpp example2.cpp example2_opt.cpp fmri.cpp fmri_opt.cpp fuse.cpp fuse_opt.cpp pde.cpp pde_opt.cpp priv2.cpp priv2_opt.cpp ssymm.cpp ssymm_opt.cpp

# Default target: build all executables
all: $(SOURCES:.cpp=)

# Rule to compile each source file into an executable
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean target: remove executables
clean:
	rm -f $(SOURCES:.cpp=)