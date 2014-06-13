CC=gcc
CFLAGS=

SRCS=BestFirstStack.cpp BlockDB.cpp ClassicNodeTree.cpp ClassicTree.cpp ContingencyTable.cpp DB.cpp JavaDB.cpp Learner.cpp Node.cpp NodeTest.cpp Probas.cpp Result.cpp Score.cpp ScoreInfo.cpp Scorer.cpp SimpleNodeTest.cpp Stack.cpp TreeNodeTest.cpp TrueBestFirstStack.cpp VimpList.cpp Bagging.cpp ExtraTrees.cpp ExtraTTrees.cpp RandomForest.cpp RandomTTForest.cpp GroupLearner.cpp Logs.cpp GStump.cpp RandomStack.cpp
OBJS=$(SRCS:.cpp=.o)

TARGET=TTree

all:
	@echo 'Generating object files'
	@echo '-----------------------'
	gcc -c -O3 $(SRCS);
	@echo ''
	@echo 'Linking and Building executable'
	@echo '-------------------------------'
	gcc -o $(TARGET) main.cpp *.o -lstdc++ -lm
	@echo ''
	@echo 'Removing object files'
	@echo '---------------------'
	rm *.o

clean:
	rm *.o