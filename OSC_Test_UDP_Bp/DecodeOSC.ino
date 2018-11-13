int packetSize;
char incomingPacket[255];

void button1(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  if(msg.isBoolean(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Float received");
    bool val = msg.getBoolean(0);
    Serial.print("button1 = ");
    Serial.print(val);
    Serial.println("");
  }
  else if(msg.isFloat(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Float received");
    float val = msg.getFloat(0);
    Serial.print("button1 = ");
    Serial.print(val);
    Serial.println("");
    if(val == 1.0)
    {
      pin_accu = 1000;
    }
  }
  else if(msg.isInt(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Int received");
    int val = msg.getInt(0);
    Serial.print("button1 = ");
    Serial.print(val);
    Serial.println("");

  }  
  else if(msg.isDouble(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Double received");
    double val = msg.getDouble(0);
    Serial.print("button1 = ");
    Serial.print(val);
    Serial.println("");
  }
}

void button2(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  if(msg.isBoolean(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Float received");
    bool val = msg.getBoolean(0);
    Serial.print("button2 = ");
    Serial.print(val);
    Serial.println("");
  }
  else if(msg.isFloat(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Float received");
    float val = msg.getFloat(0);
    Serial.print("button2 = ");
    Serial.print(val);
    Serial.println("");
    if(val == 1.0) digitalWrite(D2, LOW);
    else if (val == 0.0) digitalWrite(D2, HIGH);
  }
  else if(msg.isInt(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Int received");
    int val = msg.getInt(0);
    Serial.print("button2 = ");
    Serial.print(val);
    Serial.println("");

  }  
  else if(msg.isDouble(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Double received");
    double val = msg.getDouble(0);
    Serial.print("button2 = ");
    Serial.print(val);
    Serial.println("");
  }
}

void OSC_Decode()
//This function shall be called periodicaly to check if UDP packet have been received and decode it
{
  packetSize = Udp.parsePacket();
  if (packetSize)
  {
    if (DEBUG_LEVEL >= 1) Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    OSCMessage msg;
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    if (DEBUG_LEVEL >= 1) Serial.print("UDP packet contents: ");
    for(int i=0;i<len;i++)
    {
      msg.fill(incomingPacket[i]);
      if (DEBUG_LEVEL >= 1) Serial.print(incomingPacket[i]);
    }
    if (DEBUG_LEVEL >= 1) Serial.println(" ");

//Dispatch OSC messages to actions (modify code bellow)
    msg.dispatch("/oscControl/button1", button1);
    if (DEBUG_LEVEL >= 1) Serial.println(" ");
  }
}
