#ifndef MQTT_CALLBACK_HPP
#define MQTT_CALLBACK_HPP

#include <iostream>
#include <cstring>

#include "mqtt/async_client.h"

class callback : public virtual mqtt::callback
{
    void connection_lost(const std::string& cause) override;
    void delivery_complete(mqtt::delivery_token_ptr tok) override;
    //void message_arrived(const mqtt::const_message_ptr& msg) override;
};

#endif // MQTT_CALLBACK_HPP
