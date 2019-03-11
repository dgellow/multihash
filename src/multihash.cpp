#include "multihash/multihash.h"
#include <tuple>

namespace multihash {

Multihash::Multihash(hash code, std::string s) : code(code) {
	// TODO: not implemented yet
	this->digest = {};
	this->length = this->digest.size();
}

std::tuple<UInt, Error> uvarint(Bytes &buf) {
	// TODO: not implemented yet
	return {0, std::nullopt};
}

std::tuple<OptioMultihashRef, Error> decode(Bytes &buf) {
	if (buf.size() < 2) {
		return {std::nullopt, errTooShort};
	}

	UInt code{};
	UInt length{};
	Error err{};

	std::tie(code, err) = uvarint(buf);
	if (err) {
		return {std::nullopt, err};
	}

	std::tie(length, err) = uvarint(buf);
	if (err) {
		return {std::nullopt, err};
	}

	if (buf.size() != length) {
		return {std::nullopt, errInconsistantLength};
	}

	auto hashSearch = hash_code.find(code);
	if (hashSearch == hash_code.end()) {
		return {std::nullopt, errUnknownHashCode};
	}

	auto m = Multihash{hashSearch->second, length, buf};
	return {std::ref(m), std::nullopt};
}

} // namespace multihash
