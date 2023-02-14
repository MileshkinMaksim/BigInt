#include "pch.h"
#include "BigInt2.h"


int max(int, int);
int min(int, int);
int digits_num(int);

BigInt::BigInt()
{
	this->is_negative = false;

	this->binary_represent = { 0 };
	this->binary_amount = 1;
}

BigInt::BigInt(int number)
{
	this->binary_amount = 0;
	if (number < 0)
		this->is_negative = true;
	this->num_size = digits_num(abs(number));

	if (number == 0)
	{
		this->binary_represent = { 0 };
		this->binary_amount = 1;
		this->is_negative = false;
		return;
	}

	if (number < 0)
		this->is_negative = true;
	else
		this->is_negative = false;

	std::bitset<sizeof(int) * 8> tmp(abs(number));
	int packer = 0;
	int mask_array[8] = {
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b10000000 };
	char cur_char = 0;

	for (int i = 0; i < tmp.size(); i++)
	{
		if (packer == 7)
		{
			packer = 0;
			this->binary_represent.push_back(cur_char);
			this->binary_amount += 1;
			cur_char = 0;
		}

		cur_char += mask_array[packer] * tmp[i];
		packer++;
	}
	if (packer != 0)
	{
		this->binary_represent.push_back(cur_char);
		this->binary_amount += 1;
	}

	fit();
}

BigInt::BigInt(std::string input)
{
	this->binary_amount = 0;
	this->is_negative = false;
	this->num_size = input.length();

	if (input == "0")
	{
		this->binary_amount = 1;
		this->binary_represent = { 0 };
		return;
	}

	this->numerical_digit_amount = 0;
	char switcher = 0;
	numerical_digits.push_back(0);
	int start = 0;


	if (input[0] == '-')
	{
		this->is_negative = true;
		this->num_size -= 1;
		start = 1;
	}

	//naive realization of converting to 2-base
	for (int i = input.size() - 1; i >= start; i--)
	{
		if (switcher == 9)
		{
			switcher = 0;
			this->numerical_digit_amount++;
			this->numerical_digits.push_back(0);
		}
		if (input[i] > 47 && input[i] < 58) {
			numerical_digits[numerical_digit_amount] += (input[i] - 48) * pow(10, switcher);
			++switcher;
		}
		else
		{
			this->numerical_digits.clear();
			throw std::invalid_argument("String must not contain any letters or symbols except digits!");
			return;
		}
	}


	int packer = 0;
	int mask_array[8] = {
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b10000000 };
	char cur_char = 0;

	while (this->numerical_digit_amount > 0 || this->numerical_digits[0] > 1)
	{
		if (packer == 7)
		{
			packer = 0;
			this->binary_represent.push_back(cur_char);
			this->binary_amount += 1;
			cur_char = 0;
			continue;
		}
		cur_char += mask_array[packer] * (this->numerical_digits[0] % 2);
		this->divide_by_two();
		packer++;
	}
	if (packer != 0)
	{
		if (packer == 7)
		{
			this->binary_represent.push_back(cur_char);
			this->binary_amount += 1;
			cur_char = 0;

			cur_char += int(bool(mask_array[packer] * this->numerical_digits[0]));
			this->binary_represent.push_back(cur_char);
			this->binary_amount += 1;
		}
		else
		{
			cur_char += mask_array[packer] * this->numerical_digits[0];
			this->binary_represent.push_back(cur_char);
			this->binary_amount += 1;
		}
	}

	this->numerical_digits.clear();
	this->numerical_digit_amount = 0;

	fit();
}

BigInt::BigInt(const BigInt& base)
{
	this->num_size = base.num_size;
	this->binary_represent = base.binary_represent;
	this->binary_amount = base.binary_amount;
	this->is_negative = base.is_negative;
}

BigInt::~BigInt()
{
	this->binary_represent.clear();
}

BigInt& BigInt::operator=(const BigInt& base)
{
	this->num_size = base.size();
	this->is_negative = base.is_negative;

	this->binary_represent = base.binary_represent;
	this->binary_amount = base.binary_amount;

	return *this;
}

BigInt BigInt::operator~() const
{
	BigInt temp;
	temp.is_negative = this->is_negative;
	temp.binary_represent = this->binary_represent;
	temp.binary_amount = this->binary_amount;

	for (int i = 0; i < this->binary_amount; i++)
	{
		temp.binary_represent[i] = (~temp.binary_represent[i]) & 0b01111111;
	}

	return temp;
}

BigInt& BigInt::operator++()
{
	bool carry = false;
	if (!this->is_negative)
	{
		this->binary_represent[0] += 1;
		carry = bool(this->binary_represent[0] & 0b10000000);
		this->binary_represent[0] &= 0b01111111;

		for (int i = 1; i < this->binary_amount; i++)
		{
			if (!carry)
				break;
			this->binary_represent[i] += 1;
			carry = bool(this->binary_represent[i] & 0b10000000);
			this->binary_represent[i] &= 0b01111111;
		}
	}
	else
	{
		this->is_negative = false;
		--(*this);
		this->is_negative = true;
	}

	fit();

	return *this;
}

const BigInt BigInt::operator++(int) const
{
	BigInt temp(*this);

	BigInt* tis = const_cast<BigInt*>(this);

	++(*tis);

	tis->fit();

	return temp;
}

BigInt& BigInt::operator--()
{
	bool carry = 0;
	if (this->binary_represent[0] == 0 && this->binary_amount == 1)
	{
		this->is_negative = true;
	}

	if (this->is_negative)
	{
		this->is_negative = false;
		++(*this);
		this->is_negative = true;
	}

	else
	{
		this->binary_represent[0] |= 0b10000000;
		this->binary_represent[0] -= 1;
		carry = !bool(this->binary_represent[0] & 0b10000000);
		this->binary_represent[0] &= 0b01111111;

		for (int i = 1; i < this->binary_amount; i++)
		{
			if (!carry) break;
			this->binary_represent[i] |= 0b10000000;
			this->binary_represent[i] -= 1;
			carry = !bool(this->binary_represent[i] & 0b10000000);
			this->binary_represent[i] &= 0b01111111;
		}

	}

	fit();

	return *this;
}

const BigInt BigInt::operator--(int) const
{
	BigInt temp(*this);

	BigInt* tis = const_cast<BigInt*>(this);

	--(*tis);

	tis->fit();

	return temp;

}

BigInt& BigInt::operator+=(const BigInt& arg)
{
	int iter_until = max(this->bin_size(), arg.bin_size());
	if (*this > arg) iter_until = min(this->bin_size(), arg.bin_size());

	bool carry = false;
	if ((!this->is_negative && !arg.is_negative) || (this->is_negative && arg.is_negative))
	{
		for (int i = 0; i < iter_until; i++)
		{
			if (i < this->binary_amount)
			{
				this->binary_represent[i] += arg.binary_represent[i];
				if (carry) this->binary_represent[i] += 1;
				carry = bool(this->binary_represent[i] & 0b10000000);
				this->binary_represent[i] &= 0b01111111;
			}
			else
			{
				this->binary_represent.push_back(arg.binary_represent[i]);
				this->binary_amount += 1;
				if (carry) this->binary_represent[i] += 1;
				carry = 0;
			}
		}
		if (carry)
		{
			if (!(iter_until == this->binary_amount))
			{
				this->binary_represent[iter_until] += 1;
			}
			else
			{
				this->binary_represent.push_back(1);
				this->binary_amount += 1;
			}
		}
	}
	else if (this->is_negative && !arg.is_negative)
	{
		this->is_negative = false;
		*this -= arg;
		this->is_negative = true;
	}
	else
	{
		arg.set_negative(false);
		*this -= arg;
		arg.set_negative(true);
	}

	fit();

	return *this;
}

BigInt& BigInt::operator*=(const BigInt& arg)
{
	bool this_is_negative = this->is_negative;
	bool arg_is_negative = arg.is_negative;

	BigInt temp(*this);
	temp.is_negative = false;

	this->is_negative = false;
	this->binary_amount = 1;
	this->binary_represent.clear();
	this->binary_represent.push_back(0);

	bool zero_or_one;
	unsigned char mask_array[7] = {
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000
	};


	for (int i = arg.bin_size() - 1; i >= 0; i--)
	{
		for (int a = 6; a >= 0; a--)
		{
			zero_or_one = bool(arg.binary_represent[i] & mask_array[a]);
			this->shla();

			if (zero_or_one) *this += temp;
		}
	}

	this->is_negative = this_is_negative ^ arg_is_negative;

	return *this;
}

BigInt& BigInt::operator-=(const BigInt& arg)
{
	bool carry = false;

	if ((!arg.is_negative && !this->is_negative) || (arg.is_negative && this->is_negative))
	{
		if (*this > arg)
		{
			int iter_until = arg.bin_size();
			for (int i = 0; i < iter_until; i++)
			{
				this->binary_represent[i] |= 0b10000000;
				this->binary_represent[i] -= arg.binary_represent[i];
				if (carry) this->binary_represent[i] -= 1;
				carry = !bool(this->binary_represent[i] & 0b10000000);
				this->binary_represent[i] &= 0b01111111;
			}
		}
		else
		{
			this->is_negative = true;

			BigInt temp(arg);

			int iter_until = this->bin_size();
			for (int i = 0; i < iter_until; i++)
			{
				temp.binary_represent[i] |= 0b10000000;
				temp.binary_represent[i] -= this->binary_represent[i];
				if (carry) temp.binary_represent[i] -= 1;
				carry = !bool(temp.binary_represent[i] & 0b10000000);
				temp.binary_represent[i] &= 0b01111111;
			}

			this->binary_represent = temp.binary_represent;
			this->binary_amount = temp.binary_amount;
		}
	}
	else if (this->is_negative && !arg.is_negative)
	{
		this->is_negative = false;
		*this += arg;
		this->is_negative = true;
	}
	else if (!this->is_negative && arg.is_negative)
	{
		arg.set_negative(false);
		*this += arg;
		arg.set_negative(true);
	}

	fit();

	return *this;
}

BigInt& BigInt::operator/=(const BigInt& arg)
{
	bool mem_arg_is_negative = arg.is_negative;
	arg.set_negative(false);

	BigInt result(0);
	if (arg == BigInt())
	{
		throw std::invalid_argument("String must not contain any letters or symbols except digits!");
		return *this;
	}

	unsigned char mask_array[7] = {
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000
	};

	int bit_iterator = 0;

	for (int temp_iter = 6; temp_iter >= 0; temp_iter--)
	{
		if (this->binary_represent[this->binary_amount - 1] & mask_array[temp_iter])
		{
			bit_iterator = temp_iter + (this->binary_amount - 1) * 7;
			break;
		}
	}



	BigInt dividend(1);
	bit_iterator--;

	while (bit_iterator >= 0)
	{
		if (dividend >= arg)
		{
			result.shla();
			result++;
			dividend -= arg;

			dividend.shla();
			dividend.binary_represent[dividend.binary_amount - 1] += int(bool(this->binary_represent[bit_iterator / 7] & mask_array[bit_iterator % 7]));
		}
		else
		{
			result.shla();

			dividend.shla();
			dividend.binary_represent[dividend.binary_amount - 1] += int(bool(this->binary_represent[bit_iterator / 7] & mask_array[bit_iterator % 7]));
		}

		bit_iterator--;
	}
	if (dividend >= arg)
	{
		result.shla();
		result++;
		dividend -= arg;
	}
	else
	{
		result.shla();
	}


	arg.set_negative(mem_arg_is_negative);
	result.set_negative(mem_arg_is_negative ^ this->is_negative);

	this->binary_amount = result.bin_size();
	this->binary_represent = result.binary_represent;
	this->is_negative = result.is_negative;


	return *this;
}

BigInt& BigInt::operator^=(const BigInt& arg)
{
	int iter_until = min(this->bin_size(), arg.bin_size());

	for (int i = 0; i < iter_until; i++)
	{
		this->binary_represent[i] ^= arg.binary_represent[i];
	}

	return *this;
}

BigInt& BigInt::operator%=(const BigInt& arg)
{
	bool mem_arg_is_negative = arg.is_negative;
	arg.set_negative(false);

	if (arg == BigInt())
	{
		std::cerr << "Invalid argument!" << std::endl;
		return *this;
	}

	unsigned char mask_array[7] = {
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000
	};

	int bit_iterator = 0;

	for (int temp_iter = 6; temp_iter >= 0; temp_iter--)
	{
		if (this->binary_represent[this->binary_amount - 1] & mask_array[temp_iter])
		{
			bit_iterator = temp_iter + (this->binary_amount - 1) * 7;
			break;
		}
	}



	BigInt dividend(1);
	bit_iterator--;

	while (bit_iterator >= 0)
	{
		if (dividend >= arg)
		{
			dividend -= arg;

			dividend.shla();
			dividend.binary_represent[dividend.binary_amount - 1] += int(bool(this->binary_represent[bit_iterator / 7] & mask_array[bit_iterator % 7]));
		}
		else
		{
			dividend.shla();
			dividend.binary_represent[dividend.binary_amount - 1] += int(bool(this->binary_represent[bit_iterator / 7] & mask_array[bit_iterator % 7]));
		}

		bit_iterator--;
	}
	if (dividend >= arg)
	{
		dividend -= arg;
	}


	arg.set_negative(mem_arg_is_negative);

	this->binary_amount = dividend.bin_size();
	this->binary_represent = dividend.binary_represent;


	return *this;
}

BigInt& BigInt::operator&=(const BigInt& arg)
{
	int iter_until = min(this->bin_size(), arg.bin_size());

	for (int i = 0; i < iter_until; i++)
	{
		this->binary_represent[i] &= arg.binary_represent[i];
	}

	return *this;
}

BigInt& BigInt::operator|=(const BigInt& arg)
{
	int iter_until = min(this->bin_size(), arg.bin_size());

	for (int i = 0; i < iter_until; i++)
	{
		this->binary_represent[i] |= arg.binary_represent[i];
	}

	return *this;
}

BigInt BigInt::operator+() const
{
	return *this;
}

BigInt BigInt::operator-() const
{
	BigInt temp;
	temp.is_negative = !this->is_negative;
	temp.binary_represent = this->binary_represent;
	temp.binary_amount = this->binary_amount;
	temp.num_size = this->num_size;

	return temp;
}

bool BigInt::operator==(const BigInt& comparable) const
{
	if (this->is_negative != comparable.is_negative) return false;
	if (this->binary_amount == comparable.bin_size())
	{
		for (int i = 0; i < this->binary_amount; i++)
		{
			if (this->binary_represent[i] != comparable.binary_represent[i]) return false;
		}
		return true;
	}
	else return false;
}

bool BigInt::operator!=(const BigInt& comparable) const
{
	if (this->is_negative != comparable.is_negative) return true;
	if (this->binary_amount == comparable.bin_size())
	{
		for (int i = 0; i < this->binary_amount; i++)
		{
			if (this->binary_represent[i] != comparable.binary_represent[i]) return true;
		}
		return false;
	}
	else return true;

}

bool BigInt::operator<(const BigInt& comparable) const
{
	if (this->is_negative != comparable.is_negative)
	{
		if (this->is_negative && !comparable.is_negative) return true;
		else false;
	}
	if (this->binary_amount == comparable.bin_size())
	{
		for (int i = this->binary_amount - 1; i >= 0; i--)
		{
			if (this->binary_represent[i] < comparable.binary_represent[i]) return true;
			if (this->binary_represent[i] > comparable.binary_represent[i]) return false;
		}
		return false;
	}
	else if (this->binary_amount < comparable.bin_size()) return true;
	else return false;
}

bool BigInt::operator>(const BigInt& comparable) const
{
	if (this->is_negative != comparable.is_negative)
	{
		if (this->is_negative && !comparable.is_negative) return false;
		else true;
	}
	if (this->binary_amount == comparable.bin_size())
	{
		for (int i = this->binary_amount - 1; i >= 0; i--)
		{
			if (this->binary_represent[i] > comparable.binary_represent[i]) return true;
			if (this->binary_represent[i] < comparable.binary_represent[i]) return false;
		}
		return false;
	}
	else if (this->binary_amount > comparable.bin_size()) return true;
	else return false;
}

bool BigInt::operator<=(const BigInt& comparable) const
{
	if (this->is_negative != comparable.is_negative)
	{
		if (this->is_negative && !comparable.is_negative) return true;
		else false;
	}
	if (this->binary_amount == comparable.bin_size())
	{
		for (int i = this->binary_amount - 1; i >= 0; i--)
		{
			if (this->binary_represent[i] < comparable.binary_represent[i]) return true;
			if (this->binary_represent[i] > comparable.binary_represent[i]) return false;
		}
		return true;
	}
	else if (this->binary_amount < comparable.bin_size()) return true;
	else return false;
}

bool BigInt::operator>=(const BigInt& comparable) const
{
	if (this->is_negative != comparable.is_negative)
	{
		if (this->is_negative && !comparable.is_negative) return false;
		else true;
	}
	if (this->binary_amount == comparable.bin_size())
	{
		for (int i = this->binary_amount - 1; i >= 0; i--)
		{
			if (this->binary_represent[i] > comparable.binary_represent[i]) return true;
			if (this->binary_represent[i] < comparable.binary_represent[i]) return false;
		}
		return true;
	}
	else if (this->binary_amount > comparable.bin_size()) return true;
	else return false;
}

BigInt::operator int() const
{
	return (this->get_safe(3) << 21) + (this->get_safe(2) << 14) + (this->get_safe(1) << 7) + (0xFF & this->get_safe(0));
}

BigInt::operator std::string() const
{
	std::string result = "";

	vector <int> output;
	output.push_back(this->binary_represent[this->bin_size() - 1]);
	int k = this->bin_size() - 2;
	int carry = 0, carry_t;

	//back to 10 base form
	while (k >= 0)
	{
		for (int a = 0; a < 7; a++)
		{
			//reform
			for (int b = 0; b < output.size(); b++)
			{
				carry_t = output[b] / 100000000;
				output[b] %= 100000000;
				output[b] += carry;
				carry = carry_t;

				carry_t = output[b] / 100000000;
				output[b] %= 100000000;
				if (carry_t != 0) carry = carry * 10 + carry_t;
			}
			if (carry != 0) output.push_back(carry);
			carry = 0;

			//multiply
			for (int j = 0; j < output.size(); j++)
			{
				output[j] *= 2;
			}
		} //shift left 7 bits

		output[0] += this->binary_represent[k];

		//reform one more time
		for (int b = 0; b < output.size(); b++)
		{
			carry_t = output[b] / 100000000;
			output[b] %= 100000000;
			output[b] += carry;
			carry = carry_t;

			carry_t = output[b] / 100000000;
			output[b] %= 100000000;
			if (carry_t != 0) carry = carry * 10 + carry_t;
		}
		if (carry != 0) output.push_back(carry);
		carry = 0;

		k--;
	}

	if (this->is_negative) result += "-";
	for (int k = output.size() - 1; k >= 0; k--)
	{
		if (k < (output.size() - 1) && output[k] < 100000000)
		{
			int add = 8 - digits_num(output[k]);
			for (int i = 0; i < add; i++)
			{
				result += "0";
			}
			result += std::to_string(output[k]);
		}
		else result += std::to_string(output[k]);
	}
	return result;
}

BigInt operator+(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp += arg2;

	return temp;
}

BigInt operator-(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp -= arg2;

	return temp;
}

BigInt operator*(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp *= arg2;

	return temp;
}

BigInt operator/(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp /= arg2;

	return temp;
}

BigInt operator^(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp ^= arg2;

	return temp;
}

BigInt operator%(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp %= arg2;

	return temp;
}

//Bit and
BigInt operator&(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp &= arg2;

	return temp;
}

//Bit or
BigInt operator|(const BigInt& arg1, const BigInt& arg2)
{
	BigInt temp(arg1);
	temp |= arg2;

	return temp;
}

std::ostream& operator<<(std::ostream& o, const BigInt& i)
{
	o << string(i);
	return o;
}

size_t BigInt::size() const
{
	return this->num_size;
}

size_t BigInt::bin_size() const
{
	return this->binary_amount;
}

void BigInt::divide_by_two()
{
	int temp_res = 0, carry = 0;
	int sav = this->numerical_digits[this->numerical_digit_amount] / pow(10, digits_num(this->numerical_digits[this->numerical_digit_amount]) - 1);
	int am_dig_sav = digits_num(this->numerical_digits[this->numerical_digit_amount]);

	for (int i = this->numerical_digit_amount; i >= 0; i--)
	{
		temp_res = (carry + this->numerical_digits[i]) / 2;
		carry = this->numerical_digits[i] % 2 * 1000000000;
		this->numerical_digits[i] = temp_res;
	}

	if (sav == 1 && am_dig_sav == 1) //if first digit of number was 1, recalc number of digits
	{
		if (this->numerical_digit_amount > 0)
		{
			this->numerical_digits[this->numerical_digit_amount - 1] += (this->numerical_digits[this->numerical_digit_amount] % 10) * 100000000;
			this->numerical_digits[this->numerical_digit_amount] /= 10;
			if (this->numerical_digits[this->numerical_digit_amount] == 0)
			{
				this->numerical_digits.pop_back();
				this->numerical_digit_amount -= 1;
			}
		}
	}
}

//left bitshift
void BigInt::shla()
{
	bool carry = false;
	for (int i = 0; i < this->binary_amount; i++)
	{
		this->binary_represent[i] = this->binary_represent[i] << 1;
		if (carry) this->binary_represent[i] += 1;
		carry = bool(this->binary_represent[i] & 0b10000000);
		this->binary_represent[i] &= 0b01111111;
	}
	if (carry)
	{
		this->binary_represent.push_back(1);
		this->binary_amount += 1;
	}

	fit();
}

//Removes unneeded bytes
void BigInt::fit()
{
	for (int i = this->binary_amount - 1; i >= 0; i--)
	{
		if (this->binary_represent[i] == 0 && this->binary_amount > 1)
		{
			this->binary_represent.pop_back();
			this->binary_amount -= 1;
		}
		else
			break;
	}
	if (this->binary_represent[0] == 0 && this->binary_amount <= 1)
		this->is_negative = false;
}

//Simulates negative state
void BigInt::set_negative(bool state) const
{
	BigInt* temp = const_cast<BigInt*>(this);
	temp->is_negative = state;
}

//Returns number of digits
int digits_num(int arg)
{
	if (arg == 0)
		return 1;
	return floor(log10(arg) + 1);
}

//Minimum fuc
int min(int arg1, int arg2)
{
	if (arg1 < arg2)
		return arg1;
	return arg2;
}

//Maximum func
int max(int arg1, int arg2)
{
	if (arg1 > arg2)
		return arg1;
	return arg2;
}

//Safe getting byte if an array is accessed
int BigInt::get_safe(int period_number) const
{
	if (period_number >= this->binary_amount)
		return 0;
	else
		return int(this->binary_represent[period_number]);
}
