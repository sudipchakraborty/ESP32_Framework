#include "Encryption.h"
#include "mbedtls/aes.h"
#include "mbedtls/sha256.h"
#include "mbedtls/base64.h"
#include "esp_system.h"

// ---------------- Constructor ----------------
Encryption::Encryption() {}

// ---------------- Init ----------------
void Encryption::begin(const String &key)
{
    generateKey(key);
}

// ---------------- Key Derivation (SHA256) ----------------
void Encryption::generateKey(const String &inputKey)
{
    mbedtls_sha256((const unsigned char*)inputKey.c_str(),
                   inputKey.length(),
                   aesKey,
                   0);
}

// ---------------- Random IV ----------------
void Encryption::getRandomIV(uint8_t *iv)
{
    for (int i = 0; i < 16; i++)
        iv[i] = esp_random() & 0xFF;
}

// ---------------- Base64 Encode ----------------
String Encryption::base64Encode(uint8_t *data, size_t len)
{
    size_t outLen;
    unsigned char output[512];

    mbedtls_base64_encode(output, sizeof(output), &outLen, data, len);

    return String((char*)output);
}

// ---------------- Base64 Decode ----------------
int Encryption::base64Decode(const String &input, uint8_t *output)
{
    size_t outLen;
    return mbedtls_base64_decode(output, 512, &outLen,
                                 (const unsigned char*)input.c_str(),
                                 input.length());
}

// ---------------- Encrypt ----------------
String Encryption::encrypt(String plainText)
{
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);

    mbedtls_aes_setkey_enc(&aes, aesKey, 256);

    uint8_t iv[16];
    getRandomIV(iv);

    // PKCS7 Padding
    int inputLen = plainText.length();
    int paddedLen = ((inputLen / 16) + 1) * 16;

    uint8_t input[paddedLen];
    memset(input, 0, paddedLen);
    memcpy(input, plainText.c_str(), inputLen);

    uint8_t padVal = paddedLen - inputLen;
    for (int i = inputLen; i < paddedLen; i++)
        input[i] = padVal;

    uint8_t output[paddedLen];

    mbedtls_aes_crypt_cbc(&aes,
                          MBEDTLS_AES_ENCRYPT,
                          paddedLen,
                          iv,
                          input,
                          output);

    mbedtls_aes_free(&aes);

    // prepend IV to ciphertext
    uint8_t finalData[paddedLen + 16];
    memcpy(finalData, iv, 16);
    memcpy(finalData + 16, output, paddedLen);

    return base64Encode(finalData, paddedLen + 16);
}

// ---------------- Decrypt ----------------
String Encryption::decrypt(String cipherText)
{
    uint8_t buffer[512];

    size_t decodedLen;
    if (mbedtls_base64_decode(buffer, sizeof(buffer), &decodedLen,
                              (const unsigned char*)cipherText.c_str(),
                              cipherText.length()) != 0)
    {
        return "";
    }

    uint8_t iv[16];
    memcpy(iv, buffer, 16);

    uint8_t *cipher = buffer + 16;
    int cipherLen = decodedLen - 16;

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);

    mbedtls_aes_setkey_dec(&aes, aesKey, 256);

    uint8_t output[cipherLen];

    mbedtls_aes_crypt_cbc(&aes,
                          MBEDTLS_AES_DECRYPT,
                          cipherLen,
                          iv,
                          cipher,
                          output);

    mbedtls_aes_free(&aes);

    // Remove padding
    uint8_t pad = output[cipherLen - 1];
    int plainLen = cipherLen - pad;

    char result[plainLen + 1];
    memcpy(result, output, plainLen);
    result[plainLen] = '\0';

    return String(result);
}