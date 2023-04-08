#ifndef MQTT_CALLBACK_H
#define MQTT_CALLBACK_H

#include <iostream>
#include <cstring>

#include <mqtt/async_client.h>

class callback : public virtual mqtt::callback
{
    virtual void message_arrived(mqtt::const_message_ptr msg) override
    {
        this->new_msg_arrived = true;
        this->msg = msg->to_string();
    }

 public :
    bool IsMsgArrived()
    {
        return this->new_msg_arrived;
    }

    std::string GetSubTopicMsg()
    {
        return this->msg;
    }

    void ClearSubTopicMsg()
    {
        this->new_msg_arrived = false;
        this->msg.clear();
    }

 private:
    std::string msg;
    bool new_msg_arrived = false;
};


class SubscribeCallback : public virtual mqtt::iaction_listener
{
public:
    void on_failure(const mqtt::token& tok) override
    {
        std::cout << "Mqtt subscribe failed" << tok.get_message_id() << std::endl;
    }

    void on_success(const mqtt::token& tok) override
    {
        std::cout << "Mqtt subscribe success" << tok.get_message_id() << std::endl;
    }
};

#endif
