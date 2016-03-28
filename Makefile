APP = $(notdir $(CURDIR))
SOURCES = $(wildcard [^_]*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
DEPENDENCIES = $(SOURCES:%.cpp=%.d)

PRECOMPLIED = std.hpp.gch
DEPENDENCIES += $(PRECOMPLIED:%.gch=%.d)

CXXFLAGS += -pipe -MMD -std=c++11

ifndef DEBUG
	DEBUG = 0
endif
ifeq ($(DEBUG), 0)
CXXFLAGS += -O2 -DNDEBUG
else
CXXFLAGS += -g -O0 -Wall
endif

.PHONY: all clean run

all: $(APP)

$(APP): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(APP)

$(PRECOMPLIED): $(PRECOMPLIED:%.gch=%)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp $(PRECOMPLIED)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(DEPENDENCIES) $(PRECOMPLIED) $(APP)

run: $(APP)
	./$(APP)

-include $(DEPENDENCIES)
