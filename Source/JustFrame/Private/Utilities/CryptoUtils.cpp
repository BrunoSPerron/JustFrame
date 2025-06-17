// Every Frame is a Choice //
#include "Utilities/CryptoUtils.h"
#include "Misc/Base64.h"

extern "C" bool eri_VerifySignatureECDSA_Internal(const char *publicKey, const char *message,
                                                  const uint8_t *sig, size_t sigLen);

namespace FCryptoUtils {

bool VerifySignatureECDSA(const FString &PublicKeyPEM, const FString &Message,
                          const FString &Base64Signature) {
  TArray<uint8> SigBin;
  if (!FBase64::Decode(Base64Signature, SigBin)) return false;

  FTCHARToUTF8 KeyUTF8(*PublicKeyPEM);
  FTCHARToUTF8 MsgUTF8(*Message);

  return eri_VerifySignatureECDSA_Internal(KeyUTF8.Get(), MsgUTF8.Get(), SigBin.GetData(),
                                           SigBin.Num());
}

} // namespace FCryptoUtils
