#pragma once

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace multihash {

using UInt = unsigned int;
using Bytes = std::vector<char>;

using Error = std::optional<std::string>;

const Error errUnknownHashCode{"unknown hash function code"};
const Error errTooShort{"multihash too short, must be >= 2 bytes"};
const Error errTooLong{"multihash too long, must be < 129 bytes"};
const Error errInconsistantLength{"multihash length inconsistent"};
const Error errInvalidInput{"invalid input"};

enum class hash {
	ID,

	SHA1,
	SHA2_256,
	SHA2_512,
	SHA3_224,
	SHA3_256,
	SHA3_384,
	SHA3_512,
	SHA3,

	KECCAK_224,
	KECCAK_256,
	KECCAK_384,
	KECCAK_512,

	SHAKE_128,
	SHAKE_256,

	BLAKE2B_MIN,
	BLAKE2B_MAX,
	BLAKE2S_MIN,
	BLAKE2S_MAX,

	DBL_SHA2_256,

	MURMUR3,

	X11,
};

const std::map<u_int64_t, hash> hash_code = {
    {0x00, hash::ID},
    {0x11, hash::SHA1},
    {0x12, hash::SHA2_256},
    {0x13, hash::SHA2_512},
    {0x17, hash::SHA3_224},
    {0x16, hash::SHA3_256},
    {0x15, hash::SHA3_384},
    {0x14, hash::SHA3_512},
    {0x14, hash::SHA3},

    {0x1A, hash::KECCAK_224},
    {0x1B, hash::KECCAK_256},
    {0x1C, hash::KECCAK_384},
    {0x1D, hash::KECCAK_512},

    {0x18, hash::SHAKE_128},
    {0x19, hash::SHAKE_256},

    {0xb201, hash::BLAKE2B_MIN},
    {0xb240, hash::BLAKE2B_MAX},
    {0xb241, hash::BLAKE2S_MIN},
    {0xb260, hash::BLAKE2S_MAX},

    {0x56, hash::DBL_SHA2_256},

    {0x22, hash::MURMUR3},

    {0x1100, hash::X11},
};

const std::map<std::string, hash> hash_name = {
    {"id", hash::ID},
    {"sha1", hash::SHA1},
    {"sha2-256", hash::SHA2_256},
    {"sha2-512", hash::SHA2_512},
    {"sha3-224", hash::SHA3_224},
    {"sha3-256", hash::SHA3_256},
    {"sha3-384", hash::SHA3_384},
    {"sha3-512", hash::SHA3_512},
    {"dbl-sha2-256", hash::DBL_SHA2_256},
    {"murmur3", hash::MURMUR3},
    {"keccak-224", hash::KECCAK_224},
    {"keccak-256", hash::KECCAK_256},
    {"keccak-384", hash::KECCAK_384},
    {"keccak-512", hash::KECCAK_512},
    {"shake-128", hash::SHAKE_128},
    {"shake-256", hash::SHAKE_256},
    {"x11", hash::X11},
};


class Multihash {
public:
	Multihash(hash code, size_t length, Bytes digest) : code(code), length(length), digest(digest) {}
	Multihash(hash code, std::string s);

	const UInt &size() { return length; }
	const hash &getCode() { return code; }
	const Bytes &getDigest() { return digest; }

private:
	hash code;
	UInt length;
	Bytes digest;
};

using OptioMultihashRef = std::optional<std::reference_wrapper<Multihash>>;

void encode(Bytes &buf, hash code);

std::tuple<OptioMultihashRef, Error> decode(std::vector<char> buf);

bool validate(u_int64_t code) {
	auto search = hash_code.find(code);
	return search == hash_code.end();
}

} // namespace multihash
