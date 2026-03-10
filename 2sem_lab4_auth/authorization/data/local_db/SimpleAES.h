//
// Created by lv on 3/10/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_SIMPLEAES_H
#define LAB6_LOGIN_REGISTRATION_SIMPLEAES_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

using namespace std;

class SimpleAES {
private:
    string encryptionPassword;  // Store password in class

    // Derive a key from password using SHA256
    vector<unsigned char> deriveKey(const string& password) {
        vector<unsigned char> key(EVP_MAX_KEY_LENGTH);
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, password.c_str(), password.length());
        SHA256_Final(key.data(), &sha256);
        key.resize(32); // AES-256 needs 32 bytes
        return key;
    }

    // NEW: Convert binary string to hex string
    string bytesToHex(const string& binary) {
        stringstream ss;
        ss << hex << setfill('0');
        for (unsigned char c : binary) {
            ss << setw(2) << (int)c;
        }
        return ss.str();
    }

    // NEW: Convert hex string to binary string
    string hexToBytes(const string& hex) {
        string result;
        for (size_t i = 0; i < hex.length(); i += 2) {
            string byteString = hex.substr(i, 2);
            char c = (char)strtol(byteString.c_str(), nullptr, 16);
            result += c;
        }
        return result;
    }

public:
    // Constructor to store encryption password
    SimpleAES(const string& pass) : encryptionPassword(pass) {}

    // Modified: encrypt now returns hex string (safe for TEXT columns)
    string encrypt(const string& plaintext) {
        // Derive key from password
        vector<unsigned char> key = deriveKey(encryptionPassword);

        // Generate random IV
        unsigned char iv[16];
        RAND_bytes(iv, sizeof(iv));

        // Initialize encryption
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv);

        // Encrypt
        vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
        int len;
        EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                         (const unsigned char*)plaintext.c_str(), plaintext.length());
        int ciphertext_len = len;

        EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
        ciphertext_len += len;

        // Clean up
        EVP_CIPHER_CTX_free(ctx);

        // Prepend IV to ciphertext
        string binaryResult((char*)iv, 16);
        binaryResult.append((char*)ciphertext.data(), ciphertext_len);

        // Return as hex string (safe for database)
        return bytesToHex(binaryResult);
    }

    // Modified: decrypt now accepts hex string from database
    string decrypt(const string& encryptedHex) {
        // Convert hex back to binary first
        string ciphertext = hexToBytes(encryptedHex);

        if (ciphertext.length() < 16) return "";

        // Extract IV (first 16 bytes)
        unsigned char iv[16];
        memcpy(iv, ciphertext.c_str(), 16);

        // Derive key from password
        vector<unsigned char> key = deriveKey(encryptionPassword);

        // Initialize decryption
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv);

        // Decrypt
        vector<unsigned char> plaintext(ciphertext.length());
        int len;
        EVP_DecryptUpdate(ctx, plaintext.data(), &len,
                         (const unsigned char*)ciphertext.c_str() + 16,
                         ciphertext.length() - 16);
        int plaintext_len = len;

        EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
        plaintext_len += len;

        // Clean up
        EVP_CIPHER_CTX_free(ctx);

        return string((char*)plaintext.data(), plaintext_len);
    }
};


#endif //LAB6_LOGIN_REGISTRATION_SIMPLEAES_H