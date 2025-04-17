cmake_minimum_required(VERSION 3.10.0)
project(Cypher VERSION 0.1.0 LANGUAGES C)

set(CMAKE_BUILD_TYPE Release)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/trezor-firmware/crypto
)

add_executable(Cypher 
    "${PROJECT_SOURCE_DIR}/main.c"
    "${PROJECT_SOURCE_DIR}/src/mta/agent.c"
    "${PROJECT_SOURCE_DIR}/src/mta/cot_machine.c"

    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/address.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/base58.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/ecdsa.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/curves.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/bignum.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/memzero.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/hasher.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/hmac_drbg.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/secp256k1.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/sha2.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/sha3.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/ripemd160.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/rand.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/legacy/vendor/trezor-crypto/groestl.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/hmac.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/blake256.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/blake2b.c"
    "${PROJECT_SOURCE_DIR}/trezor-firmware/crypto/rfc6979.c"
)

