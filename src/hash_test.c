//
// Created by cp723 on 3/14/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "crypto/hash.h"

void print_hex(const unsigned char *array, size_t count) {
    for(size_t i = 0; i < count; i++) {
        printf("%02x", array[i]);
    }
}

int generic_test(const char *name, const EVP_MD *md, const unsigned char *msg,
                 size_t msg_size, const unsigned char *expected_digest) {
    int status;
    unsigned char *digest = malloc(EVP_MD_size(md));
    if(digest == NULL) {
        return -1;
    }

    if(evp_hash(digest, NULL, md, msg, msg_size, NULL, 0)) {
        fprintf(stderr, "ERROR: evp_hash failed\n");
        free(digest);

        return -1;
    }

    printf("%s Hash: Test ", name);
    if(!memcmp(digest, expected_digest, EVP_MD_size(md))) {
        printf("Passed\n");
        status = 0;
    }
    else {
        printf("Failed\n");
        status = 1;
    }

    printf("Expected: ");
    print_hex(expected_digest, EVP_MD_size(md));
    printf("\n");

    printf("Actual:   ");
    print_hex(digest, EVP_MD_size(md));
    printf("\n");

    free(digest);

    return status;
}

#define TEST_SIZE 10

int main() {
    const unsigned char seed[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };

    const EVP_MD *mds[TEST_SIZE] = {
            EVP_md5(),
            EVP_sha1(),
            EVP_sha224(), EVP_sha256(), EVP_sha384(), EVP_sha512(),
            EVP_sha3_224(), EVP_sha3_256(), EVP_sha3_384(), EVP_sha3_512()
    };

    const char *names[TEST_SIZE] = {
            "MD5",
            "SHA1",
            "SHA224", "SHA256", "SHA384", "SHA512",
            "SHA3-224", "SHA3-256", "SHA3-384", "SHA3-512"
    };

    const unsigned char expected_digests[TEST_SIZE][100] = {
            // MD5
            {
                    0xb4, 0xff, 0xcb, 0x23, 0x73, 0x7c, 0xec, 0x31,
                    0x5a, 0x4a, 0x4d, 0x1a, 0xa2, 0xa6, 0x20, 0xce
            },
            // SHA1
            {
                    0xae, 0x5b, 0xd8, 0xef, 0xea, 0x53, 0x22, 0xc4,
                    0xd9, 0x98, 0x6d, 0x06, 0x68, 0x0a, 0x78, 0x13,
                    0x92, 0xf9, 0xa6, 0x42
            },
            // SHA224
            {
                    0x71, 0x44, 0x6e, 0xa9, 0x33, 0x81, 0xba, 0x09,
                    0x1f, 0x94, 0xaf, 0xcd, 0xc5, 0xb9, 0x38, 0x32,
                    0x32, 0x90, 0xa1, 0xa0, 0x27, 0xc2, 0x2a, 0x75,
                    0xe8, 0x8a, 0x04, 0xd0
            },
            // SHA256
            {
                    0x63, 0x0d, 0xcd, 0x29, 0x66, 0xc4, 0x33, 0x66,
                    0x91, 0x12, 0x54, 0x48, 0xbb, 0xb2, 0x5b, 0x4f,
                    0xf4, 0x12, 0xa4, 0x9c, 0x73, 0x2d, 0xb2, 0xc8,
                    0xab, 0xc1, 0xb8, 0x58, 0x1b, 0xd7, 0x10, 0xdd
            },
            // SHA384
            {
                    0xe7, 0x11, 0x24, 0x91, 0xfa, 0xee, 0xfd, 0x57,
                    0x78, 0x6d, 0xa7, 0x3f, 0x36, 0x7b, 0x25, 0xa6,
                    0xf5, 0x76, 0x9f, 0x5c, 0x98, 0xfa, 0x7b, 0x70,
                    0x4d, 0x8d, 0x37, 0x74, 0x77, 0x24, 0xa6, 0x47,
                    0x37, 0x19, 0x89, 0xe8, 0xb0, 0xfe, 0x8d, 0x3c,
                    0xb2, 0x3f, 0x9e, 0xed, 0xd5, 0x28, 0x45, 0x6b
            },
            // SHA512
            {
                    0x3d, 0x94, 0xee, 0xa4, 0x9c, 0x58, 0x0a, 0xef,
                    0x81, 0x69, 0x35, 0x76, 0x2b, 0xe0, 0x49, 0x55,
                    0x9d, 0x6d, 0x14, 0x40, 0xde, 0xde, 0x12, 0xe6,
                    0xa1, 0x25, 0xf1, 0x84, 0x1f, 0xff, 0x8e, 0x6f,
                    0xa9, 0xd7, 0x18, 0x62, 0xa3, 0xe5, 0x74, 0x6b,
                    0x57, 0x1b, 0xe3, 0xd1, 0x87, 0xb0, 0x04, 0x10,
                    0x46, 0xf5, 0x2e, 0xbd, 0x85, 0x0c, 0x7c, 0xbd,
                    0x5f, 0xde, 0x8e, 0xe3, 0x84, 0x73, 0xb6, 0x49
            },
            // SHA3-224
            {
                    0xbf, 0xc9, 0xc1, 0xe8, 0x93, 0x9a, 0xee, 0x95,
                    0x3c, 0xa0, 0xd4, 0x25, 0xa2, 0xf0, 0xcb, 0xdd,
                    0x2d, 0x18, 0x02, 0x5d, 0x5d, 0x6b, 0x79, 0x8f,
                    0x1c, 0x81, 0x50, 0xb9
            },
            // SHA3-256
            {
                    0x05, 0x0a, 0x48, 0x73, 0x3b, 0xd5, 0xc2, 0x75,
                    0x6b, 0xa9, 0x5c, 0x58, 0x28, 0xcc, 0x83, 0xee,
                    0x16, 0xfa, 0xbc, 0xd3, 0xc0, 0x86, 0x88, 0x5b,
                    0x77, 0x44, 0xf8, 0x4a, 0x0f, 0x9e, 0x0d, 0x94
            },
            // SHA3-384
            {
                    0xe0, 0x86, 0xa2, 0xb6, 0xa6, 0x9b, 0xb6, 0xfa,
                    0xe3, 0x7c, 0xaa, 0x70, 0x73, 0x57, 0x23, 0xe7,
                    0xcc, 0x8a, 0xe2, 0x18, 0x37, 0x88, 0xfb, 0xb4,
                    0xa5, 0xf1, 0xcc, 0xac, 0xd8, 0x32, 0x26, 0x85,
                    0x2c, 0xa6, 0xfa, 0xff, 0x50, 0x3e, 0x12, 0xff,
                    0x95, 0x42, 0x3f, 0x94, 0xf8, 0x72, 0xdd, 0xa3
            },
            // SHA3-512
            {
                    0xcb, 0xd3, 0xf6, 0xee, 0xba, 0x67, 0x6b, 0x21,
                    0xe0, 0xf2, 0xc4, 0x75, 0x22, 0x29, 0x24, 0x82,
                    0xfd, 0x83, 0x0f, 0x33, 0x0c, 0x1d, 0x84, 0xa7,
                    0x94, 0xbb, 0x94, 0x72, 0x8b, 0x2d, 0x93, 0xfe,
                    0xbe, 0x4c, 0x18, 0xea, 0xe5, 0xa7, 0xe0, 0x17,
                    0xe3, 0x5f, 0xa0, 0x90, 0xde, 0x24, 0x26, 0x2e,
                    0x70, 0x95, 0x1a, 0xd1, 0xd7, 0xdf, 0xb3, 0xa8,
                    0xc9, 0x6d, 0x11, 0x34, 0xfb, 0x18, 0x79, 0xf2
            }
    };

    int status = 0;

    for(size_t i = 0; i < TEST_SIZE; i++) {
        int sub_status = generic_test(names[i], mds[i], seed, sizeof(seed), expected_digests[i]);
        if(sub_status < 0) {
            return EXIT_FAILURE;
        }
        status |= sub_status;
        if(i < TEST_SIZE - 1) {
            printf("\n");
        }
    }

    return status ? EXIT_FAILURE : EXIT_SUCCESS;
}