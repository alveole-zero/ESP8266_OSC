int packetSize;
char incomingPacket[255];

void button1(OSCMessage &msg)
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
    digitalWrite(D1, LOW);
    delay(5);
    digitalWrite(D1, HIGH);
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

void slider1(OSCMessage &msg)
{
  float pipi;
  int stepstogo=0;
  pipi = msg.getFloat(0);
  Serial.print("stepstogo=");
  stepstogo = pipi*stepsPerRevolution;
  Serial.print(stepstogo);
  for(int i=0;i<stepstogo;i++)
  {
    digitalWrite(D1, LOW);
    delay(1);
    digitalWrite(D1, HIGH);
  }
  //myStepper.setSpeed(60);
  //myStepper.step(stepstogo);
  Serial.println(" Done");
}

void slider2(OSCMessage &msg)
{
  float pipi;
  pipi = msg.getFloat(0);
  Serial.print("slider2=");
  Serial.println(pipi);  
}

void slider3(OSCMessage &msg)
{
  float pipi;
  pipi = msg.getFloat(0);
  Serial.print("slider3=");
  Serial.println(pipi);  
}

void slider4(OSCMessage &msg)
{
  float pipi;
  pipi = msg.getFloat(0);
  Serial.print("slider4=");
  Serial.println(pipi);  
}

void OSC_Decode()
{
//Read UDP Frame and parse OSC
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
    msg.dispatch("/oscControl/slider1 ", slider1);
    msg.dispatch("/oscControl/slider2", slider2);
    msg.dispatch("/oscControl/slider3", slider3);
    msg.dispatch("/oscControl/slider4", slider4);
    msg.dispatch("/oscControl/button1", button1);
    if (DEBUG_LEVEL >= 1) Serial.println(" ");
  }
}
