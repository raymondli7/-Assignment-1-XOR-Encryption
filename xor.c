#include <stdio.h>
#include <string.h>

#define MAX_KEY_LEN 256

void xorEncryptDecrypt(char *message, const char *key, int dataLen) {
    int keyLen = strlen(key);
    for (int i = 0; i < dataLen; ++i) {
        message[i] ^= key[i % keyLen];
    }
}

int main() {
    char choice;
    printf("Enter 'e' to encrypt or 'd' to decrypt: ");
    scanf(" %c", &choice);
    while (getchar() != '\n');

    if (choice == 'e') {
        char message[1024];
        char filename[256];
        char key[MAX_KEY_LEN];

        printf("Enter the message to encrypt: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // Remove newline

        printf("Enter the filename to save encrypted data: ");
        scanf("%s", filename);

        printf("Enter the encryption key: ");
        scanf("%s", key);

        FILE *file = fopen(filename, "wb");
        if (!file) {
            perror("Error opening file for writing");
            return 1;
        }
        int msgLen = strlen(message)
        xorEncryptDecrypt(message, key, msgLen);
        fwrite(message, sizeof(char), msgLen, file);
        fclose(file);
    } else if (choice == 'd') {
        char filename[256];
        char key[MAX_KEY_LEN];

        printf("Enter the filename to read encrypted data: ");
        scanf("%s", filename);

        printf("Enter the decryption key: ");
        scanf("%s", key);

        FILE *file = fopen(filename, "rb");
        if (!file) {
            perror("Error opening file for reading");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *encryptedData = (char *)malloc(fileSize + 1);
        if (!encryptedData) {
            perror("Error allocating memory");
            fclose(file);
            return 1;
        }

        fread(encryptedData, sizeof(char), fileSize, file);
        encryptedData[fileSize] = '\0';
        fclose(file);

        xorEncryptDecrypt(encryptedData, key, fileSize);
        printf("Decrypted message: %s\n", encryptedData);

        free(encryptedData);
    } else {
        printf("Invalid choice. Please enter 'e' or 'd'.\n");
    }

    return 0;
}