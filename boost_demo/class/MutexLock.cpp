#include <iostream>
#include <assert.h>
#include <thread>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

class MutexLock : public boost::noncopyable {
public:
MutexLock():m_holder(0){
    pthread_mutex_init(&m_mutex,nullptr);
}
~MutexLock(){
    assert(m_holder==0);
    pthread_mutex_destroy(&m_mutex);
}
bool isLockedByThisThread(){
    return m_holder == CurrentThread::tid();
}
private:
pthread_mutex_t m_mutex;
pid_t m_holder;
};

int main() {
    
}