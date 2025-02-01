#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>

void derive_key_pbkdf2(const char *password, const unsigned char *salt, int salt_len, int iterations, int key_len, unsigned char *output_key) {
    if (PKCS5_PBKDF2_HMAC(password, strlen(password), salt, salt_len, iterations, EVP_sha256(), key_len, output_key) == 0) {
        fprintf(stderr, "Error generating key with PBKDF2\n");
    }
}

int main() {
    const char *password = "mypassword";
    unsigned char salt[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}; // 8 bytes
    int iterations = 10000;
    int key_len = 8; // DES necesita 8 bytes
    unsigned char key[8];

    derive_key_pbkdf2(password, salt, sizeof(salt), iterations, key_len, key);
    //derive_key_pbkdf2(password, NULL, 0, iterations, key_len, key);
    printf("Salt: ");
    for (int i = 0; i < sizeof(salt); i++) {
        printf("%02x", salt[i]);
    }
    printf("\n");
    printf("Clave derivada: ");
    for (int i = 0; i < key_len; i++) {
        printf("%02x", key[i]);
    }
    printf("\n");

    

    return 0;
}
