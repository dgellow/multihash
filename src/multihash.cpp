#include "multihash/multihash.h"
#include <tuple>

namespace multihash {

// Decodes unsigned variable integer from buffer.
// Returns the value and the count of bytes read.
//
// Count of bytes gives indication of errors:
//    count == 0: buffer was too small
//    count < 0: value larger than 64 bits, thus overflow,
//                -count is the number of bytes read.
std::tuple<UInt, int> binUvarint(const Bytes &buf) {
	UInt x;
	UInt s;

	for (int i = 0; i < buf.size(); i++) {
		short b = buf.at(i);
		if (b < 0x80) {
			if ((i > 9 || i == 9) && (b > 1)) {
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
		return {0, errVarintTooLong};
	}
	buf = Bytes{buf.begin() + c, buf.end()};
	return {n, std::nullopt};
}

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
	Bytes buf{str.begin(), str.end()};
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

	std::tie(code, err) = uvarint(buf);
	if (err) {
		return err;
	}

	std::tie(length, err) = uvarint(buf);
	if (err) {
		return err;
	}

	if (buf.size() != length) {
		return errInconsistantLength;
	}

	auto hashSearch = hash_code.find(code);
	if (hashSearch == hash_code.end()) {
		return errUnknownHashCode;
	}

	m = {hashSearch->second, length, buf};
	return std::nullopt;
}

// Initialize the multihash parameter from a string.
// May return same errors as decode(Bytes &buf, Multihash &m).
OptError decode(std::string str, Multihash &m) {
	Bytes buf{str.begin(), str.end()};
	return decode(buf, m);
}

} // namespace multihash
