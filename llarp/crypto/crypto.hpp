#ifndef LLARP_CRYPTO_HPP
#define LLARP_CRYPTO_HPP

#include <crypto/constants.hpp>
#include <crypto/types.hpp>

#include <util/buffer.hpp>

#include <functional>
#include <stdbool.h>
#include <stdint.h>

/**
 * crypto.hpp
 *
 * libsodium abstraction layer
 * potentially allow libssl support in the future
 */

namespace llarp
{
  /// PKE(result, publickey, secretkey, nonce)
  using path_dh_func = std::function< bool(
      SharedSecret &, const PubKey &, const SecretKey &, const TunnelNonce &) >;

  /// TKE(result, publickey, secretkey, nonce)
  using transport_dh_func = std::function< bool(
      SharedSecret &, const PubKey &, const SecretKey &, const TunnelNonce &) >;

  /// SH(result, body)
  using shorthash_func =
      std::function< bool(ShortHash &, const llarp_buffer_t &) >;

  /// library crypto configuration
  struct Crypto
  {
    virtual ~Crypto() = 0;

    /// xchacha symmetric cipher
    virtual bool
    xchacha20(const llarp_buffer_t &, const SharedSecret &,
              const TunnelNonce &) = 0;

    /// xchacha symmetric cipher (multibuffer)
    virtual bool
    xchacha20_alt(const llarp_buffer_t &, const llarp_buffer_t &,
                  const SharedSecret &, const byte_t *) = 0;

    /// path dh creator's side
    virtual bool
    dh_client(SharedSecret &, const PubKey &, const SecretKey &,
              const TunnelNonce &) = 0;
    /// path dh relay side
    virtual bool
    dh_server(SharedSecret &, const PubKey &, const SecretKey &,
              const TunnelNonce &) = 0;
    /// transport dh client side
    virtual bool
    transport_dh_client(SharedSecret &, const PubKey &, const SecretKey &,
                        const TunnelNonce &) = 0;
    /// transport dh server side
    virtual bool
    transport_dh_server(SharedSecret &, const PubKey &, const SecretKey &,
                        const TunnelNonce &) = 0;
    /// blake2b 512 bit
    virtual bool
    hash(byte_t *, const llarp_buffer_t &) = 0;
    /// blake2b 256 bit
    virtual bool
    shorthash(ShortHash &, const llarp_buffer_t &) = 0;
    /// blake2s 256 bit hmac
    virtual bool
    hmac(byte_t *, const llarp_buffer_t &, const SharedSecret &) = 0;
    /// ed25519 sign
    virtual bool
    sign(Signature &, const SecretKey &, const llarp_buffer_t &) = 0;
    /// ed25519 verify
    virtual bool
    verify(const PubKey &, const llarp_buffer_t &, const Signature &) = 0;
    /// seed to secretkey
    virtual bool
    seed_to_secretkey(llarp::SecretKey &, const llarp::IdentitySecret &) = 0;
    /// randomize buffer
    virtual void
    randomize(const llarp_buffer_t &) = 0;
    /// randomizer memory
    virtual void
    randbytes(void *, size_t) = 0;
    /// generate signing keypair
    virtual void
    identity_keygen(SecretKey &) = 0;
    /// generate encryption keypair
    virtual void
    encryption_keygen(SecretKey &) = 0;
    /// generate post quantum encrytion key
    virtual void
    pqe_keygen(PQKeyPair &) = 0;
    /// post quantum decrypt (buffer, sharedkey_dst, sec)
    virtual bool
    pqe_decrypt(const PQCipherBlock &, SharedSecret &, const byte_t *) = 0;
    /// post quantum encrypt (buffer, sharedkey_dst,  pub)
    virtual bool
    pqe_encrypt(PQCipherBlock &, SharedSecret &, const PQPubKey &) = 0;
  };

  inline Crypto::~Crypto()
  {
  }

  /// return random 64bit unsigned interger
  uint64_t
  randint();

  const byte_t *
  seckey_topublic(const SecretKey &secret);

  const byte_t *
  pq_keypair_to_public(const PQKeyPair &keypair);

  const byte_t *
  pq_keypair_to_secret(const PQKeyPair &keypair);

}  // namespace llarp

#endif
