#ifndef MESSAGE_H
#define MESSAGE_H

#include "globals.h"

namespace message1
{

class msgbuffer
{
public:
    msgbuffer();
    void addMessage(string m);
    void deleteMessage();
    void deleteAllMessages();
    string getMessage(int);
    int getBufferSize();
private:
    vector <string> msg_container;

};
}

#endif
