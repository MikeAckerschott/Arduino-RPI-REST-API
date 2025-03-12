#include <Arduino.h>
#include <Speck.h>

Speck speck;
// uint8_t plaintext[] = {1, 2, 3, 4};
uint8_t plaintext128[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
// uint8_t ciphertext[4];
uint8_t ciphertext128[16];
// uint8_t decrypted[4];
// uint8_t decrypted128[16];
uint8_t key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

void setup() {
    speck.setKey(key, 8);
    pinMode(13, OUTPUT);
}

void loop() {
    PORTB = PORTB ^ B00100000;
    PORTB = PORTB ^ B00100000;
    speck.encryptBlock(ciphertext128, plaintext128);
    // speck.decryptBlock(decrypted128, ciphertext128);
    // for (int i = 0; i < 16; i++) {
    //     Serial.print(decrypted128[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();
}
