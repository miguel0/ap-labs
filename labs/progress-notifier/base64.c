#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "logger.h"

void progressSig(int sig);
void quitSig(int isg);
void encode(char *fileName);
void decode(char *fileName);
char* base64_encode(char* plain);
char* base64_decode(char* cipher);

char base46_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

int fileSize;
int bytesProcessed;

int main(int argc, char *argv[])
{
    if(argc < 3) {
        errorf("Wrong number of arguments.\n");
        exit(1);
    }

    fileSize = 1;
    bytesProcessed = 0;

    if(signal(SIGUSR1, progressSig) == SIG_ERR) {
        errorf("Error with signals.\n");
        exit(1);
    }

    if(signal(SIGINT, quitSig) == SIG_ERR) {
        errorf("Error with signals.\n");
        exit(1);
    }

    if(!strcmp("--encode", argv[1])) {
        encode(argv[2]);
    } else if(!strcmp("--decode", argv[1])) {
        decode(argv[2]);
    } else {
        errorf("Flag not recognized.\n");
        exit(1);
    }

    return 0;
}

void progressSig(int sig)
{
    infof("Current progress: %.2f\%\n", (bytesProcessed * 100.0) / fileSize);
    signal(sig, progressSig);
}

void quitSig(int isg)
{
    infof("Exiting program.\n");
    exit(1);
}

void encode(char *fileName)
{
    // Opening files.
    int source = open(fileName, O_RDONLY);
    if(source < 0) {
        errorf("Error openning source file.\n");
        exit(1);
    }

    int dest = open("encoded.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dest < 0) {
        errorf("Error openning destination file.\n");
        exit(1);
    }

    // Getting file length and returning to the beginning of the file.
    fileSize = lseek(source, 0, SEEK_END);
    if(fileSize == -1) {
        errorf("Error getting file length.\n");
        exit(1);
    }

    if(lseek(source, -fileSize, SEEK_END) == -1) {
        errorf("Error returning to the beginning of the file.\n");
        exit(1);
    }

    // Encoding.
    char data[129];
    char *encodedString;
    int bytesRead = -1;

    while((bytesRead = read(source, data, 128)) > 0) {
        data[bytesRead] = '\0';
        encodedString = base64_encode(data);

        if(write(dest, encodedString, strlen(encodedString)) < 0) {
            errorf("Error writing to file.\n");
            exit(1);
        }

        bytesProcessed += bytesRead;
    }

    if(bytesRead < 0) {
            errorf("Error reading file.\n");
            exit(1);
    }

    // Closing files.
    if(close(source) < 0) {
        errorf("Unable to close source file.\n");
        exit(1);
    }

    if(close(dest) < 0) {
        errorf("Unable to close destination file.\n");
        exit(1);
    }
}

void decode(char *fileName)
{
    // Opening files.
    int source = open(fileName, O_RDONLY);
    if(source < 0) {
        errorf("Error openning source file.\n");
        exit(1);
    }

    int dest = open("decoded.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dest < 0) {
        errorf("Error openning destination file.\n");
        exit(1);
    }

    // Getting file length and returning to the beginning of the file.
    fileSize = lseek(source, 0, SEEK_END);
    if(fileSize == -1) {
        errorf("Error getting file length.\n");
        exit(1);
    }

    if(lseek(source, -fileSize, SEEK_END) == -1) {
        errorf("Error returning to the beginning of the file.\n");
        exit(1);
    }

    // Decoding.
    char data[129];
    char *decodedString;
    int bytesRead = -1;

    while((bytesRead = read(source, data, 128)) > 0) {
        data[bytesRead] = '\0';
        decodedString = base64_decode(data);

        if(write(dest, decodedString, strlen(decodedString)) < 0) {
            errorf("Error writing to file.\n");
            exit(1);
        }

        bytesProcessed += bytesRead;
    }
    
    if(bytesRead < 0) {
            errorf("Error reading file.\n");
            exit(1);
    }

    // Closing files.
    if(close(source) < 0) {
        errorf("Unable to close source file.\n");
        exit(1);
    }

    if(close(dest) < 0) {
        errorf("Unable to close destination file.\n");
        exit(1);
    }
}


    /***********************************************************
    * Base64 library implementation                            *
    * @author Ahmed Elzoughby                                  *
    * @date July 23, 2017                                      *
    ***********************************************************/

char* base64_encode(char* plain) {

    char counts = 0;
    char buffer[3];
    char* cipher = malloc(strlen(plain) * 4 / 3 + 4);
    int i = 0, c = 0;

    for(i = 0; plain[i] != '\0'; i++) {
        buffer[counts++] = plain[i];
        if(counts == 3) {
            cipher[c++] = base46_map[buffer[0] >> 2];
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            cipher[c++] = base46_map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if(counts > 0) {
        cipher[c++] = base46_map[buffer[0] >> 2];
        if(counts == 1) {
            cipher[c++] = base46_map[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        } else {                      // if counts == 2
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0';   /* string padding character */
    return cipher;
}


char* base64_decode(char* cipher) {

    char counts = 0;
    char buffer[4];
    char* plain = malloc(strlen(cipher) * 3 / 4);
    int i = 0, p = 0;

    for(i = 0; cipher[i] != '\0'; i++) {
        char k;
        for(k = 0 ; k < 64 && base46_map[k] != cipher[i]; k++);
        buffer[counts++] = k;
        if(counts == 4) {
            plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
                plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if(buffer[3] != 64)
                plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    plain[p] = '\0';    /* string padding character */
    return plain;
}
