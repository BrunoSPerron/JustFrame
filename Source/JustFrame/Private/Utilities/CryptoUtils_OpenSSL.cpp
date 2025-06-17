// Every Frame is a Choice //

#define UI OpenSSL_UI
#include <openssl/evp.h>
#undef UI
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h>
#include <openssl/bio.h>
#include <cstring>
#include <string>
#include <vector>

extern "C" bool eri_VerifySignatureECDSA_Internal(const char *publicKey, const char *message,
                                                  const uint8_t *sig, size_t sigLen) {
  BIO *bio = BIO_new_mem_buf(publicKey, -1);
  if (!bio) return false;

  EVP_PKEY *pubKey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
  BIO_free(bio);
  if (!pubKey) return false;

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  bool result = false;

  if (ctx) {
    EVP_PKEY_CTX *pctx = nullptr;

    if (EVP_DigestVerifyInit(ctx, &pctx, EVP_sha256(), nullptr, pubKey) == 1 &&
        EVP_DigestVerifyUpdate(ctx, message, strlen(message)) == 1 &&
        EVP_DigestVerifyFinal(ctx, sig, sigLen) == 1) {
      result = true;
    }

    EVP_MD_CTX_free(ctx);
  }

  EVP_PKEY_free(pubKey);
  return result;
}
