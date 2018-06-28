// gcc -std=c11 -lcrypto -Wextra -Wall -pedantic bitseal.c -o bitseal 
// in Debian install libssl-dev to compile

// B58A; the base58 alphabet.  don't change this
// PSWD; the brainwallet password 
// RNDS; number of rounds of SHA256 to perform on PSWD, use hex or dec
// STAT; 1 turns on the status reporting, 0 for off
// UPDA; updates the status reporting every x sha rounds
// #define PSWD "`~1!@#$%^&*9(0)-_=+[{]}|;:'abABzZ'><,.?/"

#define PSWD "TestAddressXxYvwz"
#define RNDS 3000000ULL
#define STAT 1
#define UPDA 500000

#include<openssl/ec.h> // needed for all the EC functions
#include<openssl/obj_mac.h> // some stuff about NID_secp256k1 in here
#include<openssl/ripemd.h> // needed for..wait for it..RIPEMD!!LOLL
#include<openssl/sha.h> // sha stuff in hurr + BIGNUM
#include<stdio.h> // printf'in
#include<string.h> // memcpy
#include<time.h> // time(NULL)

// testing: on x86_64, x86, and x86 virtual machine or vps 
//        : roughly 10 minutes per billion rounds 
//        : do 24 hr test 
//        : 144 billion rounds for final test 
//        : all alphanumeric + symbol + space (except single quote 
// do repetitions millions of times to make sure same result 
//   occurs every time 

int print_b58(unsigned char binary_input[], int size)
{
  //+----------------------------------------------------------+
  //| print the base 58 version of input given---------------->|
  //+----------------------------------------------------------+
  const char B58A[] =
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
  // pointer counter
  char *pc;
  // array allocation not exact, a bit of extra room given
  char output_string[127];
  // string counter, an integer
  int sc, ai;

  // allocate and initialize new BIGNUM structure 
  BIGNUM *bn = BN_new();
  BIGNUM *bn_remainder = BN_new();
  BIGNUM *bn_divider = BN_new();
  // structure for storing memory or something, needed for division 
  BN_CTX *bn_ctx = BN_CTX_new();
  if(!bn || !bn_remainder || !bn_divider || !bn_ctx) goto fail;

  if(!BN_bin2bn(binary_input, size, bn)) goto fail;
  if(!BN_hex2bn(&bn_divider, "3A")) goto fail;
  
  for(sc = 0; !BN_is_zero(bn); sc++)
  { 
    // int BN_div(BN *dv, BN *rem, BN *a, BN *d, BN_CTX *ctx);
    // BN_div() divides a by d and places the result in dv
    // and the remainder in rem (dv=a/d, rem=a%d). 
    if(!BN_div(bn, bn_remainder, bn, bn_divider, bn_ctx)) goto fail;
    pc = BN_bn2dec(bn_remainder);
    if(!pc) goto fail;
    // change code_string_counter from string to int 
    // the remainder is used to calculate the b58 string
    if(sscanf(pc, "%d", &ai) == EOF) goto fail;
    output_string[sc] = B58A[ai];
  }
  // set the end of output_string to 0 so it prints right
  output_string[sc] = 0;
  // reverse the output_string, it's backwards right now
  int length = strlen(output_string), i, j;
  char cc;
  for(i = 0, j = length - 1; i < j; i++, j--)
  {
    cc = output_string[i];
    output_string[i] = output_string[j];
    output_string[j] = cc;
  }
  if(printf("%s", output_string) < 0) goto fail;
  return 1;

  fail:
    printf("program failure in block print_b58\n");
    return 0;
}

int main()
{
  //+----------------------------------------------------------+
  //| generate private key ----------------------------------->|
  //+----------------------------------------------------------+
  unsigned char privatekey[SHA256_DIGEST_LENGTH];
  unsigned char privatecopy[SHA256_DIGEST_LENGTH];
  unsigned long long c;
  // struct to keep some sha data in
  // (used by all SHA functions for the rest of program)
  SHA256_CTX sha_ctx_struct;
 
  time_t before = time(NULL), after;
  double seconds;
  for(c = 0; c < RNDS; c++)
  {
    // initialize sha struct 
    if(!SHA256_Init(&sha_ctx_struct)) goto fail;
    // load sha struct with passphrase if this is first round
    if(!c)
    {
      if(!SHA256_Update(&sha_ctx_struct, PSWD, strlen(PSWD))) goto fail;
    }
    // if not first round, feed in hash from last round
    else
    {
      if(!SHA256_Update(&sha_ctx_struct, privatekey, SHA256_DIGEST_LENGTH))
        goto fail;
    }
    // perform hash function and store in privatekey (void func)
    if(!SHA256_Final(privatekey, &sha_ctx_struct)) goto fail;
    // check for collisions, if the last 2 generated hashes are
    // the same, report the error
    // I don't know if this code works, not sure how to test
    // looks good though.  if it breaks at least it wont hurt much
    if(RNDS > 1 && c == RNDS - 2)
    {
      if(!memcpy(privatecopy, privatekey, SHA256_DIGEST_LENGTH)) goto fail;
    }
    if(RNDS > 1 && c == RNDS - 1)
    {
      if(!memcmp(privatecopy, privatekey, SHA256_DIGEST_LENGTH))
      {
        printf("a collision happened\n");
        goto fail;
      }
    }
    // do a little percentage left counter thingy
    // status indicator begin
    if(STAT && !(c%UPDA))
    {
      printf("  %.4lf%% ", ((double)c/(double)RNDS)*100);
      after = time(NULL);
      seconds = difftime(after, before);
      if(seconds)
      { // 60 secs in min, 60 min in hr, 24 hr in day
        // 3600, 86400
        if((seconds/((double)c/(double)RNDS))-seconds > 86400)
        { 
          printf("(%.0lf days) ",
          ((seconds/((double)c/(double)RNDS))-seconds)/86400);
        }
        else if((seconds/((double)c/(double)RNDS))-seconds > 3600)
        {
          printf("(%.0lf hrs) ",
          ((seconds/((double)c/(double)RNDS))-seconds)/3600);
        }
        else if((seconds/((double)c/(double)RNDS))-seconds > 60)
        {
          printf("(%.0lf mins) ",
          ((seconds/((double)c/(double)RNDS))-seconds)/60);
        }
        else
        {
          printf("(%.0lf secs) ",
          ((seconds/((double)c/(double)RNDS))-seconds));
        }
      }
      else
      {
        printf("(calculating...)\r");
        fflush(stdout);
        continue;
      }
      for(int f = 0; f < SHA256_DIGEST_LENGTH/2; f++)
      {
        if(printf("%02x", privatekey[f]) < 0) goto fail;
      }
      printf("...\r");
      fflush(stdout);
    }
    // status indicator end 
  }
  // clear out any status remnants
  printf(
    "          "
    "          "
    "          "
    "          "
    "          "
    "          "
    "          "
    "    \r"
  );
  //+----------------------------------------------------------+
  //| generate extended private key, it's important ---------->|
  //+----------------------------------------------------------+
  // pardon the hardcoding, that's just how it is homie
  // extended private key is 37 bytes 
  unsigned char extended_privatekey[37], hash_result[33];
  if(!SHA256_Init(&sha_ctx_struct)) goto fail;
  // zero out the memory in extended_privatekey
  if(!memset(extended_privatekey, 0, 37)) goto fail;
  // the standard says to set the first byte to 0x80
  extended_privatekey[0] = 0x80;
  // basically add 0x80 to the front of privatekey
  if(!memcpy(extended_privatekey + 1, privatekey, SHA256_DIGEST_LENGTH))
    goto fail;
  // yeah do it, need some backup memory for stuff
  if(!memcpy(hash_result, extended_privatekey, 33)) goto fail;
  // prepare to do sha256 on 33 bytes of hashresult (extendedprivkey)
  if(!SHA256_Update(&sha_ctx_struct, hash_result, SHA256_DIGEST_LENGTH+1))
    goto fail;
  // perform the hash
  if(!SHA256_Final(hash_result, &sha_ctx_struct)) goto fail;
  if(!SHA256_Init(&sha_ctx_struct)) goto fail;
  if(!SHA256_Update(&sha_ctx_struct, hash_result, SHA256_DIGEST_LENGTH))
    goto fail;
  // and den, ya hash it again
  if(!SHA256_Final(hash_result, &sha_ctx_struct)) goto fail;
  // tack on the first 4 bytes of this double hash onto the end
  // of the extended_privatekey
  // extended_privatekey will contain the 37 byte extended private key 
  // it is ready to be converted to WIF nicka (after the memcpy)
  if(!memcpy(extended_privatekey + 33, hash_result, 4)) goto fail;

  //+----------------------------------------------------------+
  //| generate the public key -------------------------------->|
  //+----------------------------------------------------------+
  unsigned char publickey[65];
 
  // ECDSA block of stuff .... copied mostly from /u/runeks
  BIGNUM *n = BN_new();
  BIGNUM *order = BN_new();
  BIGNUM *nmodorder = BN_new();
  BN_CTX *bnctx = BN_CTX_new();
  if(!n || !order || !nmodorder || !bnctx) goto fail;
  EC_POINT *ecpoint;
  unsigned int bufsize;
  EC_GROUP *pgroup = NULL; 
  pgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
  if(!pgroup) goto fail;
  n = BN_bin2bn(privatekey, SHA256_DIGEST_LENGTH, n);
  if(!n) goto fail;
  if(!EC_GROUP_get_order(pgroup, order, NULL)) goto fail;
  if(!BN_mod(nmodorder, n, order, bnctx)) goto fail;
  ecpoint = EC_POINT_new(pgroup);
  if(!ecpoint) goto fail;
  if(!EC_POINT_mul(pgroup, ecpoint, nmodorder, NULL, NULL, NULL))goto fail;
  bufsize = EC_POINT_point2oct(
    pgroup, ecpoint, POINT_CONVERSION_UNCOMPRESSED, 
    NULL, 0, NULL);
  if(!bufsize) goto fail;
  if(!EC_POINT_point2oct(
    pgroup, ecpoint, POINT_CONVERSION_UNCOMPRESSED, publickey, bufsize, 
    NULL)) goto fail;

  unsigned char hash_of_publickey[SHA256_DIGEST_LENGTH];
  if(!SHA256_Init(&sha_ctx_struct)) goto fail;
  if(!SHA256_Update(&sha_ctx_struct, publickey, 65)) goto fail;
  if(!SHA256_Final(hash_of_publickey, &sha_ctx_struct)) goto fail;
  
  // Perform RIPEMD-160 hashing on the result of SHA-256 
  // unsigned char *RIPEMD160(const unsigned char *d, unsigned long n,
  //   unsigned char *md);

  // RIPEMD160() computes the RIPEMD-160 message digest of the n 
  // bytes at d and places it in md (which must have space for 
  // RIPEMD160_DIGEST_LENGTH == 20 bytes of output). If md is NULL,
  // the digest is placed in a static array.

  unsigned char md[20];
  if(!RIPEMD160(hash_of_publickey, SHA256_DIGEST_LENGTH, md)) goto fail;
  // Add version byte in front of RIPEMD-160 hash (0x00 for Main Network)
  unsigned char md_plus_versionbyte[21];
  md_plus_versionbyte[0] = 0x00;
  if(!memcpy(md_plus_versionbyte + 1, md, 20)) goto fail;
  // Perform SHA-256 hash on the extended RIPEMD-160 result
  // hash_of md plus version byte
  unsigned char hash_of_mdpv[SHA256_DIGEST_LENGTH];
  if(!SHA256_Init(&sha_ctx_struct)) goto fail;
  if(!SHA256_Update(&sha_ctx_struct, md_plus_versionbyte, 21)) goto fail;
  if(!SHA256_Final(hash_of_mdpv, &sha_ctx_struct)) goto fail;

  // Perform SHA-256 hash on the result of the previous SHA-256 hash 
  if(!SHA256_Init(&sha_ctx_struct)) goto fail;
  if(!SHA256_Update(&sha_ctx_struct, hash_of_mdpv, SHA256_DIGEST_LENGTH))
    goto fail;
  if(!SHA256_Final(hash_of_mdpv, &sha_ctx_struct)) goto fail;

  // Take the first 4 bytes of the second SHA-256 hash.
  // This is the address checksum
  // Add the 4 checksum bytes at the end of extended RIPEMD-160
  // This the 25-byte binary Bitcoin Address
  unsigned char final[25];
  if(!memcpy(final, md_plus_versionbyte, 21)) goto fail;
  if(!memcpy(final + 21, hash_of_mdpv, 4)) goto fail;

  //+----------------------------------------------------------+
  //| print out the whole reason we're here ------------------>|
  //+----------------------------------------------------------+
  after = time(NULL);
  seconds = difftime(after, before);
  printf("passwd: '%s'\n", PSWD);
  printf("sharnd: '%llu','0x%llx'\n", RNDS, RNDS);
  printf("calctm: '%.0lf secs'\n", seconds);
  printf("prvkey: '");
  // privatekey contains raw binary, so give printf a cookie
  for(int c = 0; c < SHA256_DIGEST_LENGTH; c++)
  {
    if(printf("%02x", privatekey[c]) < 0) goto fail;
  }
  printf("'\n");
  printf("prvb58: '");
  if(!print_b58(extended_privatekey, 37)) goto fail;
  printf("'\n");
  printf("pubb58: '1");
  if(!print_b58(final, 25)) goto fail;
  printf("'\n");
  return 1;

  fail:
    printf("program failure in block main\n");
    return 0;
}
