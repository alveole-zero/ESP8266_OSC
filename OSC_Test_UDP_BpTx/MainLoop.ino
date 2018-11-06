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
    if(Serial.read() == 'c') initialConfig = true;
  }
  if(digitalRead(TRIGGER_PIN) == LOW)
  {
    IPAddress remip(192, 168, 43, 108);
    //digitalWrite(PIN_LED, LOW);
    OSCMessage msg("/oscControl/button1");
    msg.add(1);
    //msg.send(Serial);
    Udp.beginPacket(remip,10000);
    msg.send(Udp);
    Udp.endPacket();
    delay(10);
  }
  else
  {
    //digitalWrite(PIN_LED, HIGH);
  }
  
}
