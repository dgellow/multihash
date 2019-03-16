#include "multihash/multihash.h"
#include "gtest/gtest.h"

namespace {

void assertErr(std::string expected, multihash::OptError err) {
	ASSERT_TRUE(err) << "Expected an error. expected: '" << expected << "'";
	ASSERT_EQ(expected, err.value());
}

TEST(Binary, DecodeUvarIntFromBinary) {
	multihash::Bytes buf;
	multihash::UInt val;
	int count;

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
	FAIL() << "Not implemented yet";
}

TEST(Binary, DecodeHexString) {
	FAIL() << "Not implemented yet";
}

} // namespace
