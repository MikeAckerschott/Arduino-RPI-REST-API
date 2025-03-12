#include <Arduino.h>

void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);
void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);

uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
uint32_t v[2] = {7777, 8888};

void setup() {
  pinMode(13, OUTPUT);

  // Turn this on to test working XTEA encryption and decryption
  // Serial.begin(9600);
  // encipher(1, v, key);
  // for (int i = 0; i < 2; i++) {
  //   Serial.println(v[i]);
  // }
  // decipher(1, v, key);
  // for (int i = 0; i < 2; i++) {
  //   Serial.println(v[i]);
  // }
}

void loop() {
  //check XTEA encipher speed
  PORTB = PORTB ^ B00100000;
  PORTB = PORTB ^ B00100000;
  encipher(32, v, key);
}

void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
  unsigned int i;
  uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
  for (i=0; i < num_rounds; i++) {
      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
      sum += delta;
      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
  }
  v[0]=v0; v[1]=v1;
}

void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
  unsigned int i;
  uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
  for (i=0; i < num_rounds; i++) {
      v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
      sum -= delta;
      v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
  }
  v[0]=v0; v[1]=v1; 
}
