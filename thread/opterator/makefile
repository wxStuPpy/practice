CXX=g++

CXXFLAGS=-Wall -std=c++11 -lrt -lpthread

create_test:create_test.cpp
	${CXX} ${CXXFLAGS} $^ -o $@
	./$@


terminate_test:terminate_test.cpp
	${CXX} ${CXXFLAGS} $^ -o $@
	./$@
	
detach_test:detach_test.cpp
	-${CXX} ${CXXFLAGS} $^ -o $@
	-./$@
	-rm $@

cancel:cancel.cpp
	-${CXX} ${CXXFLAGS} -g $^ -o $@
	-./$@
	-rm $@

asy:asy.cpp
	-${CXX} ${CXXFLAGS}  $^ -o $@
	-./$@
	-rm $@	