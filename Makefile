CXX = g++
CXXFLAGS = -Wall -std=c++17
CXXFLAGS += -I/home/centos/shubhansu/HFT/HFT_DEV/shubh/abans/abx_client/include
CXXFLAGS += -I/usr/local/include
CXXFLAGS += -I/root/.local/include 
SRC = src/main.cpp src/client.cpp src/packet_handler.cpp src/json_writer.cpp
OBJ = $(SRC:.cpp=.o)

abx_client: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o abx_client  # Removed -lnlohmann_json

clean:
	rm -f $(OBJ) abx_client