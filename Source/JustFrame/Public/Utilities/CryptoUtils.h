// Every Frame is a Choice //
#pragma once

#include "Containers/UnrealString.h"

namespace FCryptoUtils {
// Verifies a base64 ECDSA-SHA256 signature against a given message and PEM-encoded public key.
bool VerifySignatureECDSA(const FString &PublicKeyPEM, const FString &Message,
                          const FString &Base64Signature);
} // namespace FCryptoUtils
