#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_KEY_LEN 256

void xorEncryptDecrypt(char *message, const char *key, int dataLen) {
    int keyLen = strlen(key);
    if (keyLen == 0) {
        return;
    }
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
        message[strcspn(message, "\n")] = '\0';

        printf("Enter the filename to save encrypted data: ");
        scanf("%255s", filename);

        printf("Enter the encryption key: ");
        scanf("%255s", key);

        if (strlen(key) == 0) {
            printf("Error: Key cannot be empty.\n");
            return 1;
        }

        FILE *file = fopen(filename, "wb");
        if (!file) {
            perror("Error opening file for writing");
            return 1;
        }
        int msgLen = strlen(message);
        xorEncryptDecrypt(message, key, msgLen);
        fwrite(message, sizeof(char), msgLen, file);
        fclose(file);
    } else if (choice == 'd') {
        char filename[256];
        char key[MAX_KEY_LEN];

        printf("Enter the filename to read encrypted data: ");
        scanf("%255s", filename);

        printf("Enter the decryption key: ");
        scanf("%255s", key);

        if (strlen(key) == 0) {
            printf("Error: Key cannot be empty.\n");
            return 1;
        }

        FILE *file = fopen(filename, "rb");
        if (!file) {
            perror("Error opening file for reading");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (fileSize < 0) {
            perror("Error getting file size");
            fclose(file);
            return 1;
        }

        char *encryptedData = (char *)malloc(fileSize + 1);
        if (!encryptedData) {
            perror("Error allocating memory");
            fclose(file);
            return 1;
        }

        size_t readBytes = fread(encryptedData, sizeof(char), fileSize, file);
        if (readBytes != (size_t)fileSize) {
            perror("Error reading file content");
            free(encryptedData);
            fclose(file);
            return 1;
        }
        
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