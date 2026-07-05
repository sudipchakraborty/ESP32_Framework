#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <Arduino.h>

class Encryption
{
public:
    Encryption();

    void begin(const String &key);   // pass secret (will be hashed to 256-bit)

    String encrypt(String plainText);
    String decrypt(String cipherText);

private:
    uint8_t aesKey[32];  // 256-bit key

    void generateKey(const String &inputKey);
    void getRandomIV(uint8_t *iv);

    String base64Encode(uint8_t *data, size_t len);
    int base64Decode(const String &input, uint8_t *output);
};

#endif