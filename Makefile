CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

INCLUDES = -I/usr/include/openssl

SRCDIR = src
INCDIR = include
BUILDDIR = build
TARGET = bin/file-flex

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCFILES))

LDFLAGS = -lssl -lcrypto

all: $(TARGET)

$(TARGET): $(OBJFILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean
