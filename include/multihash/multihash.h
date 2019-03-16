#pragma once

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace multihash {

using UInt = unsigned int;
using Bytes = std::vector<unsigned char>;

using Error = std::string;
using OptError = std::optional<Error>;

const Error errUnknownHashCode{"unknown hash function code"};
const Error errTooShort{"multihash too short, must be >= 2 bytes"};
const Error errTooLong{"multihash too long, must be < 129 bytes"};
const Error errVarIntBufferTooShort{"uvarint: buffer too small"};
const Error errVarIntTooLong{"uvarint: varint too big (max 64bit)"};
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

const std::map<UInt, hash> hash_by_code = {
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
	Multihash() : hash(hash::ID), code(0), length(0), digest({}) {}
	Multihash(UInt code, size_t length, Bytes digest) : code(code), length(length), digest(digest) {
		this->hash = hash_by_code.at(code);
	}

	UInt size() const { return length; }
	UInt getCode() const { return code; }
	hash getHash() const { return hash; }
	Bytes getDigest() const { return digest; }

private:
	hash hash;
	UInt code;
	UInt length;
	Bytes digest;
};

inline std::ostream &operator<<(std::ostream &stream, const Multihash &m) {
	stream << "Multihash{"
	       << " hash: " << int(m.getHash()) << ", code: " << m.getCode() << ", size: " << m.size();
	return stream;
}

using OptMultihash = std::optional<Multihash>;
using ResMultihash = std::tuple<OptMultihash, OptError>;

void encode(Bytes &buf, hash code);

ResMultihash decode(Bytes &buf);
ResMultihash decode(std::string str);
OptError decode(Bytes &buf, Multihash &m);
OptError decode(std::string str, Multihash &m);

inline bool validate(UInt code) {
	auto search = hash_by_code.find(code);
	return search == hash_by_code.end();
}

} // namespace multihash


namespace binary {
using multihash::UInt, multihash::Bytes, multihash::OptError;

std::tuple<UInt, int> binUvarint(const Bytes &buf);
std::tuple<UInt, OptError> uvarint(Bytes &buf);
Bytes decodeHex(const std::string &hex);

} // namespace binary
