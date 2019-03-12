#include "multihash/multihash.h"
#include <tuple>

namespace multihash {

// Decodes a uint64 from buffer and returns that value and an optional error
std::tuple<UInt, OptError> uvarint(Bytes &buf) {
	// TODO: not implemented yet
	return {0, std::nullopt};
}

// Decodes a buffer and constructs a Multihash.
// May return errors errTooShort, errInconsistantLength, errUnknownHashCode.
ResMultihash decode(Bytes buf) {
	if (buf.size() < 2)
		return {std::nullopt, errTooShort};

	UInt code{};
	UInt length{};
	OptError err{};

	std::tie(code, err) = uvarint(buf);
	if (err)
		return {std::nullopt, err};

	std::tie(length, err) = uvarint(buf);
	if (err)
		return {std::nullopt, err};

	if (buf.size() != length)
		return {std::nullopt, errInconsistantLength};

	auto hashSearch = hash_code.find(code);
	if (hashSearch == hash_code.end())
		return {std::nullopt, errUnknownHashCode};

	Multihash m{hashSearch->second, length, buf};
	return {m, std::nullopt};
}

// Constructs a Multihash from a string.
// May return same errors as decode(Bytes &buf).
ResMultihash decode(std::string str) {
	Bytes buf{str.begin(), str.end()};
	return decode(buf);
}

} // namespace multihash
