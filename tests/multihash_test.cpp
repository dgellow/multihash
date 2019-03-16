#include "multihash/multihash.h"
#include "gtest/gtest.h"

namespace {

void assertErr(std::string expected, multihash::OptError err) {
	ASSERT_TRUE(err) << "Expected an error. expected: '" << expected << "'";
	ASSERT_EQ(expected, err.value());
}

TEST(Binary, DecodeUvarIntFromBinary) {
	multihash::UInt val{};
	int count{};

	// read one byte
	std::tie(val, count) = binary::binUvarint({0b00000001});
	EXPECT_EQ(1, val);
	EXPECT_EQ(1, count);

	std::tie(val, count) = binary::binUvarint({0x11, 0x12, 0x13});
	EXPECT_EQ(0x11, val);
	EXPECT_EQ(1, count);

	// read multiple bytes
	std::tie(val, count) = binary::binUvarint({0b10000000, 0b00000001, 0b00000111});
	EXPECT_EQ(128, val);
	EXPECT_EQ(2, count);

	std::tie(val, count) = binary::binUvarint({0b11111111, 0b00000001, 0b00000111});
	EXPECT_EQ(255, val);
	EXPECT_EQ(2, count);

	std::tie(val, count) = binary::binUvarint({0b10101100, 0b00000010, 0b00000111});
	EXPECT_EQ(300, val);
	EXPECT_EQ(2, count);

	std::tie(val, count) = binary::binUvarint({0b10000000, 0b10000000, 0b00000001, 0b00000111});
	EXPECT_EQ(16384, val);
	EXPECT_EQ(3, count);

	// empty buffer
	std::tie(val, count) = binary::binUvarint({});
	EXPECT_EQ(0, val);
	EXPECT_EQ(0, count);

	// value overflow
	std::tie(val, count) = binary::binUvarint({0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x02});
	EXPECT_EQ(0, val);
	EXPECT_EQ(-10, count);
}

TEST(Binary, DecodeUvarInt) {
	multihash::Bytes buf{};
	multihash::UInt val{};
	multihash::OptError optErr{};

	// read values
	buf = {0x11, 0xFF, 0x01};
	std::tie(val, optErr) = binary::uvarint(buf); // first value
	ASSERT_FALSE(optErr) << "Expected no error. error: '" << optErr.value() << "'";
	EXPECT_EQ(0x11, val);

	std::tie(val, optErr) = binary::uvarint(buf); // second value
	ASSERT_FALSE(optErr) << "Expected no error. error: '" << optErr.value() << "'";
	EXPECT_EQ(0xFF, val);

	// empty buffer
	buf = {};
	std::tie(val, optErr) = binary::uvarint(buf);
	ASSERT_TRUE(optErr) << "Expected an error. expected: '" << multihash::errVarIntBufferTooShort << "'";
	ASSERT_EQ(multihash::errVarIntBufferTooShort, optErr.value());
	EXPECT_EQ(0, val);

	// value overflow
	buf = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x02};
	std::tie(val, optErr) = binary::uvarint(buf);
	ASSERT_TRUE(optErr) << "Expected an error. expected: '" << multihash::errVarIntTooLong << "'";
	ASSERT_EQ(multihash::errVarIntTooLong, optErr.value());
	EXPECT_EQ(0, val);
}

TEST(Binary, DecodeHexString) {
	std::string hex{"11148a173fd3e32c0fa78b90fe42d305f202244e2739"};
	multihash::Bytes expected{
	    0x11, 0x14, 0x8a, 0x17, 0x3f, 0xd3, 0xe3, 0x2c, 0x0f, 0xa7, 0x8b,
	    0x90, 0xfe, 0x42, 0xd3, 0x05, 0xf2, 0x02, 0x24, 0x4e, 0x27, 0x39,
	};
	auto decoded = binary::decodeHex(hex);
	EXPECT_EQ(expected, decoded);
}

} // namespace
