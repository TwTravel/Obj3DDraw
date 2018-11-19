CXX = g++ -g -pg  -fprofile-arcs -ftest-coverage
CFLAGS = -fPIC
MYFLAGS = -DTIME_INFO -Wno-deprecated
INC = -I./algebra  -I./audio -I./base -I./decoding \
	-I./dynamicdecoder  -I./config -I./hmm -I./io \
	-I./param   ${INCS_DIR_CBLAS} ${INCS_DIR_LAPACK}

CXXFLAGS = $(INC)
LDFLAGS =

SOURCES = \
	./config/ParameterManager.cpp \
	./config/ConfigurationFile.cpp \
	./param/FeatureExtractor.cpp \
	./param/ConfigurationFeatures.cpp \
	./hmm/PhoneticRulesManager.cpp \
	./hmm/HMMManager.cpp \
	./hmm/HMMStateDecoding.cpp \
	./hmm/ContextDecisionTree.cpp \
	./mainDynamicDecoder.cpp \
	./audio/AudioFile.cpp \
	./base/LMFSM.cpp \
	./base/TimeUtils.cpp \
	./base/PhoneSet.cpp \
	./base/LexUnitsFile.cpp \
	./base/LexiconManager.cpp \
	./base/LogMessage.cpp \
	./base/LMManager.cpp \
	./base/BatchFile.cpp \
	./base/FileUtils.cpp \
	./algebra/MatrixBase.cpp \
	./decoding/BestPath.cpp \
	./decoding/FillerManager.cpp \
	./io/IOBase.cpp \
	./io/FileInput.cpp \
	./io/FileOutput.cpp \
	./dynamicdecoder/DynamicNetworkX.cpp \
	./dynamicdecoder/DynamicDecoderX.cpp

OBJ = \
	./config/ParameterManager.o \
	./config/ConfigurationFile.o \
	./param/FeatureExtractor.o \
	./param/ConfigurationFeatures.o \
	./hmm/PhoneticRulesManager.o \
	./hmm/HMMManager.o \
	./hmm/HMMStateDecoding.o \
	./hmm/ContextDecisionTree.o \
	./mainDynamicDecoder.o \
	./audio/AudioFile.o \
	./base/LMFSM.o \
	./base/TimeUtils.o \
	./base/PhoneSet.o \
	./base/LexUnitsFile.o \
	./base/LexiconManager.o \
	./base/LogMessage.o \
	./base/LMManager.o \
	./base/BatchFile.o \
	./base/FileUtils.o \
	./algebra/MatrixBase.o \
	./decoding/BestPath.o \
	./decoding/FillerManager.o \
	./io/IOBase.o \
	./io/FileInput.o \
	./io/FileOutput.o \
	./dynamicdecoder/DynamicNetworkX.o \
	./dynamicdecoder/DynamicDecoderX.o

Decoder: $(OBJ) mainDynamicDecoder.o
	$(CXX) $(CXXFLAGS) -o KhDecoder $(OBJ) $(LDFLAGS)  -ldl -lpthread

clean:
	-rm KhDecoder $(OBJ)
