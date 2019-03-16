#include "multihash/multihash.h"

namespace binary {
using multihash::OptError, multihash::errVarIntTooLong, multihash::errVarIntBufferTooShort;
using multihash::UInt, multihash::Bytes;

// Decodes unsigned variable integer from buffer.
// Returns the value and the count of bytes read.
//
// Count of bytes gives indication of errors:
//    count == 0: buffer was too small
//    count < 0: value larger than 64 bits, thus overflow,
//               -count is the number of bytes read.
std::tuple<UInt, int> binUvarint(const Bytes &buf) {
	UInt x{};
	UInt s{};

	for (int i = 0; i < buf.size(); i++) {
		unsigned char b = buf.at(i);
		if (b < 0x80) {
			if (i >= 9 && b > 1) {
				return {0, -(i + 1)}; // overflow
			}
			return {x | UInt(b) << s, i + 1};
		}
		x |= UInt(b & 0x7f) << s;
		s += 7;
	}
	return {0, 0};
}

// Decodes a uint64 from buffer and returns that value.
// May return errors errVarIntBufferTooShort and errVarintTooLong.
std::tuple<UInt, OptError> uvarint(Bytes &buf) {
	auto [n, c] = binUvarint(buf);
	if (c == 0) {
		return {0, errVarIntBufferTooShort};
	}
	if (c < 0) {
		return {0, errVarIntTooLong};
	}
	buf = Bytes{buf.begin() + c, buf.end()};
	return {n, std::nullopt};
}

// Decodes a string representation of an hexadecimal value into a multihash::Bytes.
// String length should be even.
Bytes decodeHex(const std::string &hex) {
	Bytes bytes;
	for (UInt i = 0; i < hex.size(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		unsigned char b = std::strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(b);
	}

	return bytes;
}

} // namespace binary

namespace multihash {

// Decodes a buffer and constructs a Multihash.
// May return same errors as decode(Bytes &buf, Multihash &m).
ResMultihash decode(Bytes &buf) {
	Multihash m;
	auto err = decode(buf, m);
	return {m, err};
}

// Constructs a Multihash from a string.
// May return same errors as decode(Bytes &buf, Multihash &m).
ResMultihash decode(std::string str) {
	Bytes buf = binary::decodeHex(str);
	return decode(buf);
}

// Decodes a buffer and initialize the multihash parameter with the result.
// May return errors errTooShort, errInconsistantLength, errUnknownHashCode.
OptError decode(Bytes &buf, Multihash &m) {
	if (buf.size() < 2) {
		return errTooShort;
	}

	UInt code{};
	UInt length{};
	OptError err{};

	std::tie(code, err) = binary::uvarint(buf);
	if (err) {
		return err;
	}

	std::tie(length, err) = binary::uvarint(buf);
	if (err) {
		return err;
	}

	if (buf.size() != length) {
		return errInconsistantLength;
	}

	auto hashSearch = hash_by_code.find(code);
	if (hashSearch == hash_by_code.end()) {
		return errUnknownHashCode;
	}

	m = Multihash{code, length, buf};
	return std::nullopt;
}

// Initialize the multihash parameter from a string.
// May return same errors as decode(Bytes &buf, Multihash &m).
OptError decode(std::string str, Multihash &m) {
	Bytes buf = binary::decodeHex(str);
	return decode(buf, m);
}

} // namespace multihash
