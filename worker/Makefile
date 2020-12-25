CC = g++
TARGET = main_test

SOURCES += worker_main.cpp

$(TARGET) : $(SOURCES)
	$(info generate TARGET)
	$(CC) $^ -lpthread -o $@
	$(info generate TARGET successfully)

clean:
	@rm $(TARGET)