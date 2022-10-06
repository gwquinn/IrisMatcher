
OPENCV_DIR = /usr/local/Cellar/opencv/4.5.3_2
OPENCV     = -I$(OPENCV_DIR)/include/opencv4/ -L$(OPENCV_DIR)/lib -lopencv_core -lopencv_imgproc -lopencv_features2D
CXX        = g++ -fPIC -std=c++11 $(OPENCV)
LIB        = ../lib/libsift.so

all: $(LIB)

$(LIB): featureExtractor.cpp comparator.cpp
	$(CXX) -I../include $^ -shared -o $@

clean:
	rm -f $(LIB)

