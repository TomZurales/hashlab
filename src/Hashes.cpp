#include "Hashes.h"

#include "Random.h"


#include <stdlib.h>
//#include <stdint.h>
#include <assert.h>
//#include <emmintrin.h>
//#include <xmmintrin.h>
#include <inttypes.h>


//best hash in the whole world
void StudentHash(const void * key, int len, uint32_t seed, void * out) {
    //some initialization stuff (some is usually done in preprocessor, but this is for smhasher)
    const uint32_t* data = (const uint32_t*) key;
    int CHUNK_SIZE = 4;
    uint32_t dig;
    uint32_t xor_tot = 0x2ba320c4;
    uint32_t sum_tot = 0xd1d729a9;
    uint64_t large_dig = (uint64_t) len;

    for(int i = 0; i < len / CHUNK_SIZE; i++){
      xor_tot ^= data[i] * 0xC6A499BF;
      sum_tot += data[i] * 0x9DD5886F;

      for(int j = 0; j <= 32; j++){
        large_dig ^= ((uint64_t) xor_tot << j) + j;
        large_dig += ((uint64_t) sum_tot << j) + j;
        large_dig += ((uint64_t) xor_tot << j) + j;
        large_dig ^= ((uint64_t) sum_tot << j) + j;
      }
    }

    dig = large_dig >> (32 - (large_dig % 32));


    uint8_t byte1 = dig;
    uint8_t byte2 = dig >> 8;
    uint8_t byte3 = dig >> 16;
    uint8_t byte4 = dig >> 24;

    switch (large_dig % 4)
    {
    case 0:
      dig = (byte1 << 24) + (byte2 << 8) + (byte3) + (byte4 << 16);
      break;
    case 1:
      dig = (byte1 << 8) + (byte2 << 16) + (byte3) + (byte4 << 24);
      break;
    case 2:
      dig = (byte1) + (byte2 << 8) + (byte3 << 24) + (byte4 << 16);
      break;
    case 3:
      dig = (byte1 << 16) + (byte2 << 24) + (byte3 << 8) + (byte4);
      break;
    default:
      break;
    }

    *(uint32_t*)out = dig ^ xor_tot ^ sum_tot;
}

//ef1dbf9e  zero/mod/13

//----------------------------------------------------------------------------
// fake / bad hashes

void BadHash ( const void * key, int len, uint32_t seed, void * out )
{
  uint32_t h = seed;

  const uint8_t * data = (const uint8_t*)key;

  for(int i = 0; i < len; i++)
  {
    h ^= h >> 3;
    h ^= h << 5;
    h ^= data[i];
  }

  *(uint32_t*)out = h;
}

void sumhash ( const void * key, int len, uint32_t seed, void * out )
{
  uint32_t h = seed;

  const uint8_t * data = (const uint8_t*)key;

  for(int i = 0; i < len; i++)
  {
    h += data[i];
  }

  *(uint32_t*)out = h;
}

void sumhash32 ( const void * key, int len, uint32_t seed, void * out )
{
  uint32_t h = seed;

  const uint32_t * data = (const uint32_t*)key;

  for(int i = 0; i < len/4; i++)
  {
    h += data[i];
  }

  *(uint32_t*)out = h;
}

void DoNothingHash ( const void *, int, uint32_t, void * )
{
}

//-----------------------------------------------------------------------------
// One-byte-at-a-time hash based on Murmur's mix

uint32_t MurmurOAAT ( const void * key, int len, uint32_t seed )
{
  const uint8_t * data = (const uint8_t*)key;

  uint32_t h = seed;

  for(int i = 0; i < len; i++)
  {
    h ^= data[i];
    h *= 0x5bd1e995;
    h ^= h >> 15;
  }

  return h;
}

void MurmurOAAT_test ( const void * key, int len, uint32_t seed, void * out )
{
	*(uint32_t*)out = MurmurOAAT(key,len,seed);
}

//----------------------------------------------------------------------------

void FNV ( const void * key, int len, uint32_t seed, void * out )
{
  unsigned int h = seed;

  const uint8_t * data = (const uint8_t*)key;

  h ^= BIG_CONSTANT(2166136261);

  for(int i = 0; i < len; i++)
  {
    h ^= data[i];
    h *= 16777619;
  }

  *(uint32_t*)out = h;
}

//-----------------------------------------------------------------------------

uint32_t x17 ( const void * key, int len, uint32_t h ) 
{
  const uint8_t * data = (const uint8_t*)key;
    
  for(int i = 0; i < len; ++i) 
  {
        h = 17 * h + (data[i] - ' ');
    }

    return h ^ (h >> 16);
}

//-----------------------------------------------------------------------------

void Bernstein ( const void * key, int len, uint32_t seed, void * out ) 
{
  const uint8_t * data = (const uint8_t*)key;
    
  for(int i = 0; i < len; ++i) 
  {
        seed = 33 * seed + data[i];
    }

  *(uint32_t*)out = seed;
}

//-----------------------------------------------------------------------------
// Crap8 hash from http://www.team5150.com/~andrew/noncryptohashzoo/Crap8.html

uint32_t Crap8( const uint8_t *key, uint32_t len, uint32_t seed ) {
  #define c8fold( a, b, y, z ) { p = (uint32_t)(a) * (uint64_t)(b); y ^= (uint32_t)p; z ^= (uint32_t)(p >> 32); }
  #define c8mix( in ) { h *= m; c8fold( in, m, k, h ); }

  const uint32_t m = 0x83d2e73b, n = 0x97e1cc59, *key4 = (const uint32_t *)key;
  uint32_t h = len + seed, k = n + len;
  uint64_t p;

  while ( len >= 8 ) { c8mix(key4[0]) c8mix(key4[1]) key4 += 2; len -= 8; }
  if ( len >= 4 ) { c8mix(key4[0]) key4 += 1; len -= 4; }
  if ( len ) { c8mix( key4[0] & ( ( 1 << ( len * 8 ) ) - 1 ) ) }
  c8fold( h ^ k, n, k, k )
  return k;
}

void Crap8_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint32_t*)out = Crap8((const uint8_t*)key,len,seed);
}
