#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
struct can_frame canMsg2;
MCP2515 mcp2515(10);
int count = 0;

void setup()
{

  Serial.begin(9600);

  while (!Serial)
  {
    ;
  }

  canMsg1.can_id = 0x0F6;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 'z';
  canMsg1.data[1] = 'e';
  canMsg1.data[2] = 'n';
  canMsg1.data[3] = 'd';
  canMsg1.data[4] = ' ';
  canMsg1.data[5] = ' ';
  canMsg1.data[6] = ' ';
  canMsg1.data[7] = ' ';

  canMsg2 = canMsg1;

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
}

void loop()
{
  String countStr = String(count);
  String countStr2 = String(count + 1);
  for (int i = 0; i < 3 && i < countStr.length(); i++) // parse it as ASCII. only 3 bytes left in the canframe. problem when > 999
  {
    int iterator = 5 + i;
    canMsg1.data[iterator] = countStr[i];
  }

  for (int i = 0; i < 3 && i < countStr2.length(); i++) // parse it as ASCII. only 3 bytes left in the canframe. problem when > 999
  {
    int iterator = 5 + i;
    canMsg2.data[iterator] = countStr2[i];
  }

  mcp2515.sendMessage(&canMsg1);
  mcp2515.sendMessage(&canMsg2);

  Serial.println("Messages sent");
  count += 2;

  delay(100);
}
