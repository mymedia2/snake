APP = $(notdir $(CURDIR))
SOURCES = $(wildcard [^_]*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
DEPENDENCIES = $(SOURCES:%.cpp=%.d)

CXXFLAGS += -g -MMD -std=c++11

.PHONY: all clean run

all: $(APP)

$(APP): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(APP)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(DEPENDENCIES) $(APP)

run: $(APP)
	./$(APP)

-include $(DEPENDENCIES)
