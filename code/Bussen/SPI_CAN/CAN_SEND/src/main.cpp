#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
// struct can_frame canMsg2;
MCP2515 mcp2515(10);

void setup()
{

  Serial.begin(9600);

  while (!Serial)
  {
    ;
  }

  canMsg1.can_id = 0x0F6;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 255;
  canMsg1.data[1] = 0;
  canMsg1.data[2] = 0;
  canMsg1.data[3] = 0;
  canMsg1.data[4] = 0;
  canMsg1.data[5] = 0;
  canMsg1.data[6] = 0;
  canMsg1.data[7] = 255;

  // canMsg2.can_id  = 0x036;
  // canMsg2.can_dlc = 8;
  // canMsg2.data[0] = 0x0E;
  // canMsg2.data[1] = 0x00;
  // canMsg2.data[2] = 0x00;
  // canMsg2.data[3] = 0x08;
  // canMsg2.data[4] = 0x01;
  // canMsg2.data[5] = 0x00;
  // canMsg2.data[6] = 0x00;
  // canMsg2.data[7] = 0xA0;

  int result = 0;

  result = mcp2515.reset();

  if (result == MCP2515::ERROR_OK)
  {
    Serial.println("MCP2515 reset OK");
  }
  else
  {
    Serial.println("MCP2515 reset FAIL");
  }

  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);

  if (result == MCP2515::ERROR_OK)
  {
    Serial.println("MCP2515 setBitrate OK");
  }
  else
  {
    Serial.println("MCP2515 setBitrate FAIL");
  }

  result = mcp2515.setNormalMode();

  if (result == MCP2515::ERROR_OK)
  {
    Serial.println("MCP2515 setNormalMode OK");
  }
  else
  {
    Serial.println("MCP2515 setNormalMode FAIL");
  }

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

  // Serial.println("Example: Write to CAN");
}

void loop()
{
  while (mcp2515.sendMessage(&canMsg1) != MCP2515::ERROR_OK)
  {
    Serial.println("Error sending message");
  }

  delay(10);
  // mcp2515.sendMessage(&canMsg2);

  Serial.println("Messages sent");
}
