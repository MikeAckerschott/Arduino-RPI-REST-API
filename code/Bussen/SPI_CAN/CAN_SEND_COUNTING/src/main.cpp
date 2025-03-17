#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
MCP2515 mcp2515(10);
int32_t count = 0;

//union to split int32 into 4 bytes
typedef union
{
  int32_t i32;
  uint8_t u8[4];
} union_32;

union_32 u32_1;

void setup()
{
  u32_1.i32 = 0;

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

void updateCanMsg()
{
  canMsg1.data[4] = u32_1.u8[3];
  canMsg1.data[5] = u32_1.u8[2];
  canMsg1.data[6] = u32_1.u8[1];
  canMsg1.data[7] = u32_1.u8[0];
  u32_1.i32++;
  mcp2515.sendMessage(&canMsg1);
}

void loop()
{
  updateCanMsg();
  updateCanMsg();
  updateCanMsg();
  updateCanMsg();
  updateCanMsg();
  
  delay(10);
}
