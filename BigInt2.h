#pragma once
#include <string>
#include <vector>
#include <bitset>
#include <stdexcept>
#include <iostream>

using std::vector; using std::string;

class BigInt {
private:
	int numerical_digit_amount;
	int binary_amount;
	int num_size;

	void divide_by_two();
	void shla();
	void fit();
public:
	vector <int> numerical_digits;
	vector <unsigned char> binary_represent;
	bool is_negative;

	BigInt();
	BigInt(int);
	BigInt(std::string);
	BigInt(const BigInt&);
	~BigInt();

	BigInt& operator=(const BigInt&);

	BigInt operator~() const;

	BigInt& operator++();
	const BigInt operator++(int) const;
	BigInt& operator--();
	const BigInt operator--(int) const;

	BigInt& operator+=(const BigInt&);
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

	size_t size() const;
	size_t bin_size() const;

	void set_negative(bool) const;

	int get_safe(int) const;
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);


std::ostream& operator<<(std::ostream& o, const BigInt& i);
