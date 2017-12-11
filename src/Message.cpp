#include "Message.hpp"

Message::Message(std::string loc, float *data, int size)
{   
    j["loc"] = loc;
    j["mag"] = {};
    for (auto i = 0; i < size; i++)
    {
        j["mag"].push_back(data[i]);
    }

    msg = j.dump();
    length = msg.length();
}

Message::Message(std::string loc, float *data, int size, bool ctime)
{
    j["loc"] = loc;
    j["mag"] = {};
    for (auto i = 0; i < size; i++)
    {
        j["mag"].push_back(data[i]);
    }

    if (ctime)
    {
        auto t = time(nullptr);
        j["time"] = t;
    }

    msg = j.dump();
    length = msg.length();
}

Message::Message(std::string loc, float *data, int size, bool ctime, int temperature)
{
    j["loc"] = loc;
    j["mag"] = {};
    for (auto i = 0; i < size; i++)
    {
        j["mag"].push_back(data[i]);
    }

    if (ctime)
    {
        auto t = time(nullptr);
        j["time"] = t;
    }

    j["temp"] = temperature;

    msg = j.dump();
    length = msg.length();
}

const char* Message::get_message()
{
    return msg.data();
}

std::string Message::get_string()
{
    return msg;
}

std::ostream& operator<<(std::ostream& os, const Message& m)
{
    os << m.j;
    return os;
}


int Message::get_length()
{
    return length;
}