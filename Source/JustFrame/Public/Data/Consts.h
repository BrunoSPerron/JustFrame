// Every Frame is a Choice //
#pragma once

constexpr const TCHAR *MOVE_DATA_PUBLIC_KEY_PEM = TEXT(R"(-----BEGIN PUBLIC KEY-----
...your ECDSA public key here...
-----END PUBLIC KEY-----)");

constexpr const float FIXED_DELTA_TIME = 1.0f / 60.0f;
constexpr const int MAX_HEALTH = 200;