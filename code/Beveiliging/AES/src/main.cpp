#include <Arduino.h>
#include <AES.h>

AES128 aes128;
uint8_t plaintext[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
uint8_t ciphertext[16];
// uint8_t decrypted[16];
uint8_t key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

void setup() {
    aes128.setKey(key, 16);
    pinMode(13, OUTPUT);

    // Turn this on to test working AES encryption and decryption
    // Serial.begin(9600);
    // aes128.encryptBlock(ciphertext, plaintext);
    // for (int i = 0; i < 16; i++) {
    //     Serial.print(ciphertext[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();
    // aes128.decryptBlock(decrypted, ciphertext);
    // for (int i = 0; i < 16; i++) {
    //     Serial.print(decrypted[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();
}

void loop() {
    PORTB = PORTB ^ B00100000;
    PORTB = PORTB ^ B00100000;
    aes128.encryptBlock(ciphertext, plaintext);
}