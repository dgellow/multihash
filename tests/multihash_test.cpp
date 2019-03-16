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
	FAIL() << "Not implemented yet";
}

} // namespace
