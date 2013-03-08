/**
 * @ingroup adf_os_public
 * @file adf_os_crypto.h
 * This file defines crypto APIs
 */

#ifndef __ADF_OS_CRYPTO_H
#define __ADF_OS_CRYPTO_H

#include <adf_os_crypto_pvt.h>

/**
 * @brief Representation of a cipher context.
 */ 
typedef __adf_os_cipher_t     adf_os_cipher_t;

/**
 * @brief Types of crypto algorithms
 */ 
typedef enum adf_os_crypto_alg{
    ADF_OS_CRYPTO_AES = __ADF_OS_CRYPTO_AES,
    ADF_OS_CRYPTO_OTHER = __ADF_OS_CRYPTO_OTHER,
}adf_os_crypto_alg_t;


/**
 * @brief allocate the cipher context
 * @param[in] type crypto algorithm
 * 
 * @return the new cipher context
 */
static inline adf_os_cipher_t
adf_os_crypto_alloc_cipher(adf_os_crypto_alg_t type)
{
    return __adf_os_crypto_alloc_cipher(type);
}

/**
 * @brief free the cipher context
 * 
 * @param[in] cipher cipher context
 */
static inline void
adf_os_crypto_free_cipher(adf_os_cipher_t cipher)
{
    __adf_os_crypto_free_cipher(cipher);
}

/**
 * @brief set the key for cipher context with length keylen
 * 
 * @param[in] cipher    cipher context
 * @param[in] key       key material
 * @param[in] keylen    length of key material
 * 
 * @return a_uint32_t
 */
static inline a_uint32_t
adf_os_crypto_cipher_setkey(adf_os_cipher_t cipher, const a_uint8_t *key, a_uint8_t keylen)
{
    return __adf_os_crypto_cipher_setkey(cipher, key, keylen);
}

/**
 * @brief encrypt the data with AES
 * 
 * @param[in]   cipher  cipher context
 * @param[in]   src     unencrypted data
 * @param[out]  dst     encrypted data
 */
static inline void
adf_os_crypto_rijndael_encrypt(adf_os_cipher_t cipher, const void *src, void *dst)
{
    __adf_os_crypto_rijndael_encrypt(cipher, src, dst);
}
#endif
