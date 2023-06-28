#include "message.h"

namespace message1
{
msgbuffer::msgbuffer()
{
}

void msgbuffer::addMessage(string m)
{
    msg_container.push_back(m);
    if (msg_container.size() > 4)
        deleteMessage();
}

void msgbuffer::deleteMessage()
{
    msg_container.erase(msg_container.begin());
}

void msgbuffer::deleteAllMessages()
{
    msg_container.clear();
}

string msgbuffer::getMessage(int i)
{
    return msg_container[i];
}

int msgbuffer::getBufferSize()
{
    return msg_container.size();
}

};
