#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);

//union to split int32 into 4 bytes
typedef union
{
  int32_t i32;
  uint8_t u8[4];
} union_32;

union_32 u32_1;

void setup()
{
  Serial.begin(115200);

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop()
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {

    PORTD ^= 1 << 3;
    PORTD ^= 1 << 3;

    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" ");
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");

    for (int i = 0; i < 4; i++)
    { // print the data
      Serial.print((char)canMsg.data[i]);
    }

    u32_1.u8[0] = canMsg.data[7];
    u32_1.u8[1] = canMsg.data[6];
    u32_1.u8[2] = canMsg.data[5];
    u32_1.u8[3] = canMsg.data[4];

    Serial.print(" ");
    Serial.println(u32_1.i32);
  }
}
