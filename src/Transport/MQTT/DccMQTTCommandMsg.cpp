#include <Arduino.h>

#include <Transport/MQTT/DccMQTTCommandMsg.h>

#include <Diag/DIAG.h>

DccMQCmdMsg DccMQTTCommandMsg::msg[MAXMSG];

// Retrieve the index of a pool item we can reuse; -1 if there is none;
int freeMsg()
{
    int i = 0;
    for (i = 0; i < MAXMSG; i++)
    {
        if (DccMQTTCommandMsg::msg[i].free == true)
        {
            return i;
        }
    }
    return -1;
}

uint8_t DccMQTTCommandMsg::setMsg(Commands c, char *cid)
{
    int i = freeMsg();
    if (i == -1)
    {
        return -1;
    }
    DccMQTTCommandMsg::msg[i].cmd = c;
    DccMQTTCommandMsg::msg[i]._cid[0] = '\0';
    strcpy(DccMQTTCommandMsg::msg[i]._cid, cid);

    return i;
}

void DccMQTTCommandMsg::printCmdMsg(DccMQCmdMsg *cm)
{
    Serial.println(F("-----------------------"));
    Serial.print(F("cmd     :"));
    Serial.println(cm->cmd);
    Serial.print(F("free    :"));
    Serial.println(cm->free);
    Serial.print(F("mIdx    :"));
    Serial.println(cm->mIdx);
    Serial.print(F("msgId   :"));
    Serial.println(cm->msgId);
    Serial.print(F("result  :"));
    Serial.println(cm->result);
    Serial.print(F("cid     :"));
    Serial.println(cm->_cid);
    Serial.print(F("nParams     :"));
    Serial.println(cm->nParams);
    Serial.println(F("-----------------------"));
}

void DccMQTTCommandMsg::printCmdMsgPool()
{
    INFO(F("---------------------------"));
    for (int i = 0; i < MAXMSG; i++)
    {
        INFO(F("--%d------------------------"), i);
        Serial.print(DccMQTTCommandMsg::msg[i].mIdx);
        Serial.print(" ");
        Serial.print(DccMQTTCommandMsg::msg[i].free);
        Serial.print(" ");
        Serial.print(DccMQTTCommandMsg::msg[i].msgId);
        Serial.println();
    }
    INFO(F("---------------------------"));
}

DccMQTTCommandMsg::DccMQTTCommandMsg()
{
}

DccMQTTCommandMsg::~DccMQTTCommandMsg()
{
}