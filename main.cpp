#include <iostream>
#include <nimbus.h>
#include <nim.h>
#include <port.h>

using namespace std;

#ifdef __cplusplus
extern "C"{
#endif

void nimInit(int);
void nimEnd(int);
void nimLogSet(string,string,int,int);
void nimLogTruncateSize(int);
void nimLog(int,string);

#ifdef __cplusplus
};
#endif

int main()
{
    nimInit(0);

    nimLogSet("example_probe.log","example_probe", 0, 0);
    nimLogTruncateSize(100 * 1024);

    nimLog(0,"****************[ Starting ]****************");
    nimLog(0,"Hello world!!!");

    cout << "Hello world!" << endl;
    nimEnd(0);
    return 0;
}
