CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET  = SmartCampus
SRCS    = main.cpp Resource.cpp LabHardware.cpp CafeteriaPerishable.cpp \
          BookstoreMedia.cpp User.cpp Student.cpp Staff.cpp \
          Order.cpp Store.cpp
OBJS    = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
