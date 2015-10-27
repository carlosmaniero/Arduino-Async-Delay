all:
	g++ *.cpp -I . -o scheduler
run:
	g++ *.cpp -I . -o scheduler
	./scheduler
library:
	zip Scheduler.zip Scheduler.cpp Scheduler.h Task.cpp Task.h kwywords.txt
