/*********************************************************************
* Filename:   aes.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding AES implementation.
*********************************************************************/

#ifndef AES_H
#define AES_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define AES_BLOCK_SIZE 16               // AES operates on 16 bytes at a time

/**************************** DATA TYPES ****************************/
typedef unsigned char AES_BYTE;            // 8-bit byte
typedef unsigned int AES_WORD;             // 32-bit word, change to "long" for 16-bit machines

/*********************** FUNCTION DECLARATIONS **********************/
///////////////////
// AES
///////////////////
// Key setup must be done before any AES en/de-cryption functions can be used.
void aes_key_setup(const AES_BYTE key[],          // The key, must be 128, 192, or 256 bits
                   AES_WORD w[],                  // Output key schedule to be used later
                   int keysize);              // Bit length of the key, 128, 192, or 256

void aes_encrypt(const AES_BYTE in[],             // 16 bytes of plaintext
                 AES_BYTE out[],                  // 16 bytes of ciphertext
                 const AES_WORD key[],            // From the key setup
                 int keysize);                // Bit length of the key, 128, 192, or 256

void aes_decrypt(const AES_BYTE in[],             // 16 bytes of ciphertext
                 AES_BYTE out[],                  // 16 bytes of plaintext
                 const AES_WORD key[],            // From the key setup
                 int keysize);                // Bit length of the key, 128, 192, or 256

///////////////////
// AES - CBC
///////////////////
int aes_encrypt_cbc(const AES_BYTE in[],          // Plaintext
                    size_t in_len,            // Must be a multiple of AES_BLOCK_SIZE
                    AES_BYTE out[],               // Ciphertext, same length as plaintext
                    const AES_WORD key[],         // From the key setup
                    int keysize,              // Bit length of the key, 128, 192, or 256
                    const AES_BYTE iv[]);         // IV, must be AES_BLOCK_SIZE bytes long

int aes_decrypt_cbc(const AES_BYTE in[], size_t in_len, AES_BYTE out[], const AES_WORD key[], int keysize, const AES_BYTE iv[]);

// Only output the CBC-MAC of the input.
int aes_encrypt_cbc_mac(const AES_BYTE in[],      // plaintext
                        size_t in_len,        // Must be a multiple of AES_BLOCK_SIZE
                        AES_BYTE out[],           // Output MAC
                        const AES_WORD key[],     // From the key setup
                        int keysize,          // Bit length of the key, 128, 192, or 256
                        const AES_BYTE iv[]);     // IV, must be AES_BLOCK_SIZE bytes long

///////////////////
// AES - CTR
///////////////////
void increment_iv(AES_BYTE iv[],                  // Must be a multiple of AES_BLOCK_SIZE
                  int counter_size);          // Bytes of the IV used for counting (low end)

void aes_encrypt_ctr(const AES_BYTE in[],         // Plaintext
                     size_t in_len,           // Any byte length
                     AES_BYTE out[],              // Ciphertext, same length as plaintext
                     const AES_WORD key[],        // From the key setup
                     int keysize,             // Bit length of the key, 128, 192, or 256
                     const AES_BYTE iv[]);        // IV, must be AES_BLOCK_SIZE bytes long

void aes_decrypt_ctr(const AES_BYTE in[],         // Ciphertext
                     size_t in_len,           // Any byte length
                     AES_BYTE out[],              // Plaintext, same length as ciphertext
                     const AES_WORD key[],        // From the key setup
                     int keysize,             // Bit length of the key, 128, 192, or 256
                     const AES_BYTE iv[]);        // IV, must be AES_BLOCK_SIZE bytes long

///////////////////
// AES - CCM
///////////////////
// Returns True if the input parameters do not violate any constraint.
int aes_encrypt_ccm(const AES_BYTE plaintext[],              // IN  - Plaintext.
                    AES_WORD plaintext_len,                  // IN  - Plaintext length.
                    const AES_BYTE associated_data[],        // IN  - Associated Data included in authentication, but not encryption.
                    unsigned short associated_data_len,  // IN  - Associated Data length in bytes.
                    const AES_BYTE nonce[],                  // IN  - The Nonce to be used for encryption.
                    unsigned short nonce_len,            // IN  - Nonce length in bytes.
                    AES_BYTE ciphertext[],                   // OUT - Ciphertext, a concatination of the plaintext and the MAC.
                    AES_WORD *ciphertext_len,                // OUT - The length of the ciphertext, always plaintext_len + mac_len.
                    AES_WORD mac_len,                        // IN  - The desired length of the MAC, must be 4, 6, 8, 10, 12, 14, or 16.
                    const AES_BYTE key[],                    // IN  - The AES key for encryption.
                    int keysize);                        // IN  - The length of the key in bits. Valid values are 128, 192, 256.

// Returns True if the input parameters do not violate any constraint.
// Use mac_auth to ensure decryption/validation was preformed correctly.
// If authentication does not succeed, the plaintext is zeroed out. To overwride
// this, call with mac_auth = NULL. The proper proceedure is to decrypt with
// authentication enabled (mac_auth != NULL) and make a second call to that
// ignores authentication explicitly if the first call failes.
int aes_decrypt_ccm(const AES_BYTE ciphertext[],             // IN  - Ciphertext, the concatination of encrypted plaintext and MAC.
                    AES_WORD ciphertext_len,                 // IN  - Ciphertext length in bytes.
                    const AES_BYTE assoc[],                  // IN  - The Associated Data, required for authentication.
                    unsigned short assoc_len,            // IN  - Associated Data length in bytes.
                    const AES_BYTE nonce[],                  // IN  - The Nonce to use for decryption, same one as for encryption.
                    unsigned short nonce_len,            // IN  - Nonce length in bytes.
                    AES_BYTE plaintext[],                    // OUT - The plaintext that was decrypted. Will need to be large enough to hold ciphertext_len - mac_len.
                    AES_WORD *plaintext_len,                 // OUT - Length in bytes of the output plaintext, always ciphertext_len - mac_len .
                    AES_WORD mac_len,                        // IN  - The length of the MAC that was calculated.
                    int *mac_auth,                       // OUT - TRUE if authentication succeeded, FALSE if it did not. NULL pointer will ignore the authentication.
                    const AES_BYTE key[],                    // IN  - The AES key for decryption.
                    int keysize);                        // IN  - The length of the key in BITS. Valid values are 128, 192, 256.

///////////////////
// Test functions
///////////////////
int aes_test();
int aes_ecb_test();
int aes_cbc_test();
int aes_ctr_test();
int aes_ccm_test();

#endif   // AES_H
