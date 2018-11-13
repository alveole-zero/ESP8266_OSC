float highfloat = 1.0;

void loop()
{  
  OSC_Decode();
  // is configuration portal requested?
  if ( (initialConfig))
  {
    WifiPortal();
  }
  if (Serial.available())
  {
    if(Serial.read() == 'c')
    {
      initialConfig = true;
      Serial.println("SerialConfig");
    }
  }
  if(digitalRead(TRIGGER_PIN) == LOW) 
  {
    Serial.println("TRIGGER_PIN Config");
    initialConfig = true;
  }
  if(digitalRead(pinIn) == LOW)
  {
    IPAddress remip(RemoteIP[0], RemoteIP[1], RemoteIP[2], RemoteIP[3]);
    OSCMessage msg("/oscControl/button1");
    msg.add(highfloat);
    Udp.beginPacket(remip,10000);
    msg.send(Udp);
    Udp.endPacket();
    delay(10);
    if (DEBUG_LEVEL >= 2) Serial.println("TRIGGER_PIN");
    pin_accu = 1000;
  }
  if (pin_accu > 0)
  {
    if (pin_accu-- > 1) digitalWrite(D5, HIGH);
    else digitalWrite(D5, LOW);
    delay(1);
  }
  
}
