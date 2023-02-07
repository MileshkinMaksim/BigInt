// BigInt.h : включаемый файл для стандартных системных включаемых файлов

#pragma once
#include <iostream>
#include <math.h>
#include <exception>
#include <sstream>
#include <vector>

class BigInt {
public:
	std::vector<unsigned int> number;
	char sign;
	int cntBits;

	BigInt();
	BigInt(int);
	BigInt(std::string);  // std::invalid_argument
	BigInt(const BigInt&);
	~BigInt();
	// BigInt(BigInt&&);

	// BigInt& operator=(BigInt&&);
	BigInt& operator=(const BigInt&);  //
	BigInt operator~() const;

	BigInt& operator++();
	const BigInt operator++(int) const;
	BigInt& operator--();
	const BigInt operator--(int) const;

	BigInt& operator+=(const BigInt&);  //
	BigInt& operator*=(const BigInt&);
	BigInt& operator-=(const BigInt&);
	BigInt& operator/=(const BigInt&);
	BigInt& operator^=(const BigInt&);
	BigInt& operator%=(const BigInt&);
	BigInt& operator&=(const BigInt&);
	BigInt& operator|=(const BigInt&);

	BigInt operator+() const;  // unary +
	BigInt operator-() const;  // unary -

	bool operator==(const BigInt&) const;
	bool operator!=(const BigInt&) const;
	bool operator<(const BigInt&) const;
	bool operator>(const BigInt&) const;
	bool operator<=(const BigInt&) const;
	bool operator>=(const BigInt&) const;

	operator int() const;
	operator std::string() const;

	size_t size() const;  // size in bytes
};

BigInt operator<<(const BigInt&, const int);
BigInt operator>>(const BigInt&, const int);
BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);

std::istream& operator>>(std::istream& is, BigInt& number);
std::ostream& operator<<(std::ostream& o, const BigInt& number);

//
void check_size_in_bits(BigInt& number);
int divideByTwo(std::vector<int>& a);
void multByTwo(std::vector<unsigned int>& num);
void reverse_bytes(BigInt& number);
int fromInputToBigInt(BigInt& number, std::vector<int>& num10);
