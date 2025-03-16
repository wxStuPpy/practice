CXX=g++

CXXFLAGS=-Wall -std=c++11 -lrt -lpthread

create_test:create_test.cpp
	-${CXX} ${CXXFLAGS} $^ -o $@
	-./$@
    
mutex:mutex.cpp
	-${CXX} ${CXXFLAGS} $^ -o $@
	-./$@
	-rm $@

rwlock:rwlock.cpp
	-${CXX} ${CXXFLAGS} $^ -o $@
	-./$@
	-rm $@

write_hungry:write_hungry.cpp
	-${CXX} ${CXXFLAGS} $^ -o $@
	-./$@
	-rm $@	
condition_var:condition_var.cpp
	-${CXX} ${CXXFLAGS} $^ -o $@
	-./$@
	-rm $@	