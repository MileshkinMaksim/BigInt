#include "pch.h"
#include "BigInt2.h"

using std::cout; using std::endl;

TEST(TestCaseName, TestName) {
	BigInt a("123456789123456789123456789");
	BigInt add;
	BigInt sub;
	BigInt mul;
	BigInt div;
	BigInt mod;
	BigInt And;
	BigInt Or;
	BigInt Xor;
	BigInt Not;
	BigInt equality_tester;

	EXPECT_EQ(string(a), "123456789123456789123456789");
	EXPECT_EQ(string(-a), "-123456789123456789123456789");

	equality_tester = a;
	equality_tester = equality_tester;
	EXPECT_EQ(a, equality_tester);

	add = a + equality_tester;
	EXPECT_EQ(string(add), "246913578246913578246913578");

	EXPECT_EQ(string(add + BigInt("-987654321")), "246913578246913577259259257");

	sub = add;
	sub -= BigInt("987654321123456789");
	EXPECT_EQ(string(sub), "246913577259259257123456789");

	sub -= BigInt("-987654321123456789");
	EXPECT_EQ(string(sub), "246913578246913578246913578");

	sub -= BigInt("123456789123456789123456789123456789");
	EXPECT_EQ(string(sub), "-123456791352423289447303760674791659");

	mul = add * BigInt("246913578246913578246913578");
	EXPECT_EQ(string(mul), "60966315122694714184423113124218882062490483000762084");

	mul *= -123352654;
	EXPECT_EQ(string(mul), "-7520356774984728626420036462814630779321194927885887083970936");

	div = (-mul) / BigInt("123456789");
	EXPECT_EQ(string(div), "351866239356472436757127172397862877979521724702523391");

	div = mul / BigInt("123456789");
	EXPECT_EQ(string(div), "-351866239356472436757127172397862877979521724702523391");

	mod = (-mul) % BigInt("10");
	EXPECT_EQ(string(mod), "6");

	equality_tester = a - BigInt(123456789);
	EXPECT_TRUE(a > equality_tester);
	EXPECT_FALSE(a < equality_tester);
	EXPECT_TRUE(a != equality_tester);
	EXPECT_FALSE(a == equality_tester);

	equality_tester += BigInt(123456789);

	EXPECT_TRUE(a >= equality_tester++);
	EXPECT_TRUE(a <= equality_tester);

	EXPECT_TRUE(a, +a);

	And = a & BigInt("123456789");
	Or = a | BigInt("123456789");
	Xor = a ^ BigInt("123456789");
	Not = ~a;

	EXPECT_EQ(string(And), "123456789123456788988972309");
	EXPECT_EQ(string(Or), "123456789123456789179784981");
	EXPECT_EQ(string(Xor), "123456789123456789112656384");
	EXPECT_EQ(string(Not), "2352423289447303760674791658");
}

