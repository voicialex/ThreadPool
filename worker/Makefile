CC = g++
TARGET = main_test

INC_DIRS	+= ../TaskQueue/
SOURCES		+= worker_main.cpp

$(TARGET) : $(SOURCES)
	$(info generating TARGET)
	$(CC) $^ --std=c++11 -I$(INC_DIRS)  -lpthread -o $@

clean:
	@rm $(TARGET)