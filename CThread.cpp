#include "CThread.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

CThread::CThread(deatachsatate_e attribute) 
{
    bExit = false;

    if(attribute == JOINABLE) {
        detachstate = PTHREAD_CREATE_JOINABLE;
    } else {
        detachstate = PTHREAD_CREATE_DETACHED;
    }

    if(detachstate == PTHREAD_CREATE_DETACHED) {
        pthread_attr_t    attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,detachstate);
        pthread_attr_destroy(&attr);
    }
}

class work : public CThread {
private:
    string s;
public:
    work(string str)
    {
        s = str;
    }
    virtual void do_work()
    {
        //printf("%ul\n",getpid());
        while(1) {
            //cout << s << endl;
        }
    }
};

#if 0
int main()
{
    work  obj1("obj1");
    work    obj2("obj2");

    obj1.start();
    obj2.start();


    printf("%lu\n",(unsigned long)obj1.getThreadId());
    printf("%lu\n",(unsigned long int)obj2.getThreadId());
    //cout << "Thread 1: " << obj1.getThreadId() << endl;
    //cout << "Thread 2: " << obj2.getThreadId() << endl;
    //CThread::sleep(15);  
    //.Sleep(1,"obj1 aaaaaaaaaaaaaaa","obj1 bbbbbbbbbbbbb");
    //obj2.Sleep(1,"obj2 ccccccccccccccc","obj2 ddddddddddddd");
    //CThread::Sleep(1,"main ggggggggggg","main jjjjjjjjjjjjj");

    while(1) {
        //cout << "main" << endl;
    }
    return 0;
}
#endif

