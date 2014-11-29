TARGET = plantmonitor

# See https://github.com/victorhurdugaci/SerialProtocol 
# for installing SerialProtocol
SERIAL_PROTOCOL_DIR = ../lib

CPP_SOURCES = $(SERIAL_PROTOCOL_DIR)/*.cpp $(TARGET).cpp

all: build

build: $(TARGET)

clean: 
	rm $(TARGET)

$(TARGET): 
	g++ -I$(SERIAL_PROTOCOL_DIR) $(CPP_SOURCES) -o $(TARGET)
	

