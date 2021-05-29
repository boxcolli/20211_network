#include "argon2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define A_DEFAULT_HASH_LEN 32
#define A_DEFAULT_SALT_LEN 16

#define A_DEFAULT_PWD "omok1q2w#E$R"
#define A_DEFAULT_T_COST 2
#define A_DEFAULT_M_COST (1<<16)
#define A_DEFAULT_PARALLELISM 1

typedef uint8_t u8_hash;

int argon2_hash_default(const void *salt, const size_t saltlen, void *hash, const size_t hashlen)
{
    // give salt, saltlen, hash, hashlen
    // everything else will use predefined
    //
    //
    //
    return argon2i_hash_raw(A_DEFAULT_T_COST, A_DEFAULT_M_COST, A_DEFAULT_PARALLELISM,
        A_DEFAULT_PWD, strlen((char*)A_DEFAULT_PWD), salt, saltlen, hash, hashlen);
}

void argon2_print(u8_hash *hash, const size_t hashlen)
{
    // in case you are curious about the result
    //
    //
    //
    //
    for( int i=0; i<hashlen; ++i ) printf( "%02x", hash[i] ); printf( "\n" );
}
