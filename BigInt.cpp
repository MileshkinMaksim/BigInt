#include "bigint.h"
#ifndef UINT32_MAX
#define UINT32_MAX 4294967295u
#endif

// Auxiliary methods
void check_size_in_bits(BigInt& number) {
    /// <summary>
    ///		Controls amount of bytes in BigInt
    /// </summary>
    /// <param name="number">
    ///		Big number
    /// </param>

    while (number.number.size() > 1 && number.number.back() == 0)
        number.number.pop_back();

    int bytes = number.number.size();

    if (bytes == 0 || (bytes == 1 && number.number[0] == 0)) {
        number.cntBits = 0;
        return;
    }

    long long degOf2 = 1, deg = 1;
    while (true) {
        degOf2 *= 2;
        if (degOf2 > number.number[bytes - 1]) {
            break;
        }
        deg++;
    }

    number.cntBits = (bytes - 1) * 32 + deg;
}

int divideByTwo(std::vector<int>& a) {
    /// <summary>
    ///		Divides decimal interpretation of Big number by two
    /// </summary>
    /// <param name="a">
    ///		decimal interpretation of Big number
    /// </param>
    /// <returns>
    ///		number % 2 - remainder
    /// </returns>

    int carry = 0, b = 2;
    for (int i = (int)a.size() - 1; i >= 0; --i) {
        long long cur =
            a[i] + carry * 1ll * 1000000000;  
        a[i] = int(cur / b);
        carry = int(cur % b);
    }
    while (a.size() > 1 && a.back() == 0) a.pop_back();

    return carry;
}

void multByTwo(std::vector<unsigned int>& num) {
    /// <summary>
    ///		Multiplies decimal interpretation of Big number by two
    /// </summary>
    /// <param name="num">
    ///		decimal interpretation of Big number
    /// </param>

    int limit = 1000000000;

    int carry = 0;
    for (int i = 0; i < num.size(); i++) {
        int remainder = (num[i] * 2 + carry) % limit;
        carry = (num[i] * 2 + carry - remainder) / limit;
        num[i] = remainder;
    }
    if (carry > 0) {
        num.push_back(carry);
    }
}

void reverse_bytes(BigInt& number) {
    /// <summary>
    ///		Makes the representation of Big number in back-code
    /// </summary>
    /// <param name="number">
    ///		Big number
    /// </param>

    for (int i = 0; i < number.number.size(); i++) {
        number.number[i] = ~number.number[i];
    }
}

int fromInputToBigInt(BigInt& number, std::vector<int>& num10) {
    /// <summary>
    ///		translates input from decimal to (UINT_MAX + 1)-notation
    /// </summary>
    /// <param name="number">
    ///		Big number
    /// </param>
    /// <param name="num10">
    ///		Container for decimal interpretation of Big number
    /// </param>
    /// <returns>
    ///		Count of bits in Big number
    /// </returns>

    int cntBits = 0;
    if (number.number.size() == 0) {
        number.number.push_back(0);
    }
    while (num10.size() > 1 || num10[0] >= 1) {
        int carry = divideByTwo(num10);
        if (cntBits / 32 >= number.number.size()) {
            number.number.push_back(carry);
        }
        else {
            number.number[cntBits / 32] =
                number.number[cntBits / 32] ^ (carry << (cntBits % 32));
        }
        cntBits++;
    }
    if (number.number.size() == 1 && number.number[0] == 0) {
        number.number.pop_back();
        number.sign = 1;
        number.cntBits = 0;
    }

    return cntBits;
}


BigInt& BigInt::operator++() {
    /// <summary>
    ///		Prefix increment
    /// </summary>
    /// <returns>
    ///		Copy of (this)-incremented number
    /// </returns>

    if (this->sign == -1) {
        int i = 0;
        int carry = -1;
        while (i < this->number.size()) {
            unsigned int res = (unsigned int)this->number[i] + carry;
            if (res == UINT32_MAX) {
                if (this->number.size() == 1) {
                    this->number[i] = 0;
                    this->sign = 1;
                    this->cntBits = 0;
                    break;
                }
                this->number[i] = UINT32_MAX;
                carry = -1;
                i++;
            }
            else {
                this->number[i] = res;
                break;
            }
        }
        if (this->number[this->number.size() - 1] == 0) {
            this->number.pop_back();
        }

        return *this;
    }

    int i = 0;
    int carry = 1;
    while (true) {
        if (this->number.size() == 0) {
            this->number.push_back(1);
            this->cntBits = 1;
            this->sign = 1;
            break;
        }
        long long res = (long long)this->number[i] + carry;
        if (res > UINT32_MAX) {
            this->number[i] = 0;
            carry = 1;
            i++;
            if (i >= this->number.size()) {
                this->number.push_back(carry);
                break;
            }
        }
        else {
            this->number[i] = res;
            break;
        }
    }

    check_size_in_bits(*this);
    if (this->cntBits == 0) {
        this->number.clear();
        this->sign = 1;
    }

    return *this;
}

const BigInt BigInt::operator++(int value) const {
    /// <summary>
    ///		Postfix increment
    /// </summary>
    /// <param name="value">flag for compiler</param>
    /// <returns>
    ///		Incremented copy of (this)-number
    /// </returns>

    BigInt result(*const_cast<BigInt*>(this));

    BigInt* number = const_cast<BigInt*>(this);

    ++(*number);

    return result;
}

BigInt& BigInt::operator--() {
    /// <summary>
    ///		Prefix decrement
    /// </summary>
    /// <returns>
    ///		Copy of (this)-decremented number
    /// </returns>

    if (this->sign == 1) {
        int i = 0;
        int carry = -1;
        while (i < this->number.size()) {
            unsigned int res = (unsigned int)this->number[i] + carry;
            if (res == UINT32_MAX) {
                if (this->number.size() == 1) {
                    this->number[i] = 1;
                    this->sign = -1;
                    this->cntBits = 1;
                    break;
                }
                this->number[i] = UINT32_MAX;
                carry = -1;
                i++;
            }
            else {
                this->number[i] = res;
                break;
            }
        }
        if (this->number[this->number.size() - 1] == 0) {
            this->number.pop_back();
        }

        return *this;
    }

    int i = 0;
    int carry = 1;
    while (true) {
        if (this->number.size() == 0) {
            this->number.push_back(1);
            this->cntBits = 1;
            this->sign = -1;
            break;
        }
        long long res = (long long)this->number[i] + carry;
        if (res > UINT32_MAX) {
            this->number[i] = 0;
            carry = 1;
            i++;
            if (i >= this->number.size()) {
                this->number.push_back(carry);
                break;
            }
        }
        else {
            this->number[i] = res;
            break;
        }
    }

    check_size_in_bits(*this);
    if (this->cntBits == 0) {
        this->number.clear();
        this->sign = 1;
    }

    return *this;
}

const BigInt BigInt::operator--(int value) const {
    /// <summary>
    ///		Postfix decrement
    /// </summary>
    /// <param name="value">flag for compiler</param>
    /// <returns>
    ///		Decremented copy of (this)-number
    /// </returns>

    BigInt result(*const_cast<BigInt*>(this));

    BigInt* number = const_cast<BigInt*>(this);

    --(*number);

    return result;
}

bool BigInt::operator==(const BigInt& number) const {
    /// <summary>
    ///		Compares one Big number with another
    /// </summary>
    /// <param name="number">second member of comparison</param>
    /// <returns>Equal or not</returns>

    if (this->sign == number.sign &&
        this->number.size() == number.number.size()) {
        int size = this->number.size();
        for (int i = 0; i < size && size != 0; i++) {
            if (this->number[i] != number.number[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool BigInt::operator!=(const BigInt& number) const {
    /// <summary>
    ///
    /// </summary>
    /// <param name="number"></param>
    /// <returns></returns>

    return !(*this == number);
}

bool BigInt::operator<(const BigInt& number) const {
    /// <summary>
    ///
    /// </summary>
    /// <param name="number"></param>
    /// <returns></returns>

    if (this->sign == 1 && number.sign == -1) {
        return false;
    }
    else if (this->sign == -1 && number.sign == 1) {
        return true;
    }
    else if (this->sign == -1) {
        if (this->number.size() > number.number.size()) {
            return true;
        }
        else if (this->number.size() < number.number.size()) {
            return false;
        }
        else {
            for (int i = this->number.size() - 1; i >= 0; i--) {
                if (this->number[i] > number.number[i]) {
                    return true;
                }
                else if (this->number[i] < number.number[i]) {
                    return false;
                }
            }
        }
    }
    else {
        if (this->number.size() > number.number.size()) {
            return false;
        }
        else if (this->number.size() < number.number.size()) {
            return true;
        }
        else {
            for (int i = this->number.size() - 1; i >= 0; i--) {
                if (this->number[i] < number.number[i]) {
                    return true;
                }
                else if (this->number[i] > number.number[i]) {
                    return false;
                }
            }
        }
    }

    return false;
}

bool BigInt::operator>(const BigInt& number) const {
    /// <summary>
    ///
    /// </summary>
    /// <param name="number"></param>
    /// <returns></returns>

    if (this->sign == 1 && number.sign == -1) {
        return true;
    }
    else if (this->sign == -1 && number.sign == 1) {
        return false;
    }
    else if (this->sign == -1) {
        if (this->number.size() > number.number.size()) {
            return false;
        }
        else if (this->number.size() < number.number.size()) {
            return true;
        }
        else {
            for (int i = this->number.size() - 1; i >= 0; i--) {
                if (this->number[i] > number.number[i]) {
                    return false;
                }
                else if (this->number[i] < number.number[i]) {
                    return true;
                }
            }
        }
    }
    else {
        if (this->number.size() > number.number.size()) {
            return true;
        }
        else if (this->number.size() < number.number.size()) {
            return false;
        }
        else {
            for (int i = this->number.size() - 1; i >= 0; i--) {
                if (this->number[i] < number.number[i]) {
                    return false;
                }
                else if (this->number[i] > number.number[i]) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool BigInt::operator<=(const BigInt& number) const {
    /// <summary>
    ///
    /// </summary>
    /// <param name="number"></param>
    /// <returns></returns>

    return !((*this) > number);
}

bool BigInt::operator>=(const BigInt& number) const {
    /// <summary>
    ///
    /// </summary>
    /// <param name="number"></param>
    /// <returns></returns>

    return !((*this) < number);
}


BigInt operator>>(const BigInt& number1, const int number2);

BigInt operator<<(const BigInt& number1, const int number2) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>//

    BigInt num1(number1);
    int num2 = number2;

    if (num2 < 0) {
        num2 = ~num2 + 1;
        num1 = num1 >> num2;

        return num1;
    }

    while (num2 > 0) {
        int carry = 0, tempCarry = 0;
        for (int i = 0; i < num1.number.size(); i++) {
            carry = (num1.number[i] >> 31) & 1;
            num1.number[i] = tempCarry | (num1.number[i] << 1);
            tempCarry = carry;
        }
        if (carry > 0) {
            num1.number.push_back(carry);
        }
        --num2;
    }

    check_size_in_bits(num1);

    return num1;
}

BigInt operator>>(const BigInt& number1, const int number2) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>

    BigInt num1(number1);
    int num2 = number2;

    if (num2 < 0) {
        num2 = ~num2 + 1;
        num1 = num1 << num2;

        return num1;
    }

    while (num2 > 0) {
        int carry = 0, tempCarry = 0;
        for (int i = num1.number.size() - 1; i >= 0; i--) {
            carry = num1.number[i] & 1;
            num1.number[i] = (tempCarry << 31) | (num1.number[i] >> 1);
            tempCarry = carry;
        }
        while (num1.number.size() > 0 && num1.number.back() == 0) {
            num1.number.pop_back();
        }
        --num2;
    }

    check_size_in_bits(num1);

    return num1;
}

BigInt operator^(const BigInt& number1, const BigInt& number2) {
    /// <summary>
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>

    BigInt result;
    BigInt num1(number1);
    BigInt num2(number2);

    int num1Size = num1.number.size(), num2Size = num2.number.size();
    int minSize = num1Size > num2Size ? num2Size : num1Size;

    if (num1.sign == -1) {
        reverse_bytes(num1);
        --num1;
    }
    if (num2.sign == -1) {
        reverse_bytes(num2);
        --num2;
    }

    for (int i = 0; i < minSize; i++) {
        result.number.push_back((unsigned int)num1.number[i] ^ num2.number[i]);
    }

    if (num1Size > num2Size) {
        for (int i = minSize; i < num1Size; i++) {
            result.number.push_back(num1.number[i]);
        }
    }
    else if (num1Size < num2Size) {
        for (int i = minSize; i < num2Size; i++) {
            result.number.push_back(num2.number[i]);
        }
    }

    if (num1.sign == -1 && num2.sign == -1) {
        result.sign = 1;
    }
    else if (num1.sign == -1 || num2.sign == -1) {
        result.sign = -1;
        reverse_bytes(result);
        --result;
    }
    else {
        result.sign = 1;
    }

    check_size_in_bits(result);
    if (result.cntBits == 0) {
        result.number.clear();
        result.sign = 1;
    }

    return result;
}

BigInt operator&(const BigInt& number1, const BigInt& number2) {
    /// <summary>
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>

    BigInt result;
    BigInt num1(number1);
    BigInt num2(number2);

    int num1Size = num1.number.size(), num2Size = num2.number.size();
    int minSize = num1Size > num2Size ? num2Size : num1Size;

    if (num1.sign == -1) {
        reverse_bytes(num1);
        --num1;
    }
    if (num2.sign == -1) {
        reverse_bytes(num2);
        --num2;
    }

    for (int i = 0; i < minSize; i++) {
        result.number.push_back((unsigned int)num1.number[i] & num2.number[i]);
    }

    if (num1.sign == -1 && num2.sign == -1) {
        result.sign = -1;
        reverse_bytes(result);
        --result;
    }
    else {
        result.sign = 1;
    }

    check_size_in_bits(result);
    if (result.cntBits == 0) {
        result.number.clear();
        result.sign = 1;
    }

    return result;
}

BigInt operator|(const BigInt& number1, const BigInt& number2) {
    /// <summary>
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>

    BigInt result;
    BigInt num1(number1);
    BigInt num2(number2);

    int num1Size = num1.number.size(), num2Size = num2.number.size();
    int minSize = num1Size > num2Size ? num2Size : num1Size;

    if (num1.sign == -1) {
        reverse_bytes(num1);
        --num1;
    }

    if (num2.sign == -1) {
        reverse_bytes(num2);
        --num2;
    }

    for (int i = 0; i < minSize; i++) {
        result.number.push_back((unsigned int)(num1.number[i] | num2.number[i]));
    }

    if (num1Size > num2Size) {
        for (int i = minSize; i < num1Size; i++) {
            result.number.push_back(num1.number[i]);
        }
    }
    else if (num1Size < num2Size) {
        for (int i = minSize; i < num2Size; i++) {
            result.number.push_back(num2.number[i]);
        }
    }

    if (num1.sign == -1 || num2.sign == -1) {
        result.sign = -1;
        reverse_bytes(result);
        --result;
    }
    else {
        result.sign = 1;
    }

    check_size_in_bits(result);
    if (result.cntBits == 0) {
        result.number.clear();
        result.sign = 1;
    }

    return result;
}

BigInt operator-(
    const BigInt& num1,
    const BigInt& num2);

BigInt operator+(const BigInt& number1, const BigInt& number2) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>

    BigInt result;
    BigInt num1(number1);
    BigInt num2(number2);

    int maxSize = num1.number.size() < num2.number.size() ? num2.number.size()
        : num1.number.size();

    if (num1.number.size() < num2.number.size()) {
        for (int i = num1.number.size(); i < num2.number.size(); i++) {
            num1.number.push_back(0);
        }
    }
    if (num1.number.size() > num2.number.size()) {
        for (int i = num2.number.size(); i < num1.number.size(); i++) {
            num2.number.push_back(0);
        }
    }

    if (num1.sign != num2.sign) {
        if (num1.sign == -1) {
            reverse_bytes(num1);
            --num1;
        }
        else {
            reverse_bytes(num2);
            --num2;
        }
    }

    int carry = 0;
    for (int i = 0; i < maxSize; i++) {
        long long res = (long long)num1.number[i] + num2.number[i] + carry;
        carry = (res >> 32) & 1;
        result.number.push_back(res & UINT32_MAX);
    }

    if (num1.sign == num2.sign) {
        result.number.push_back(carry);
        result.sign = num1.sign;
    }
    else {
        if (num1.sign == -1) {
            num1.sign = 1;
            reverse_bytes(num1);
            ++num1;
            if (num1 > num2) {
                result.sign = -1;
                reverse_bytes(result);
                --result;
            }
            else {
                result.sign = 1;
            }
        }
        else if (num2.sign == -1) {
            num2.sign = 1;
            reverse_bytes(num2);
            ++num2;
            if (num1 < num2) {
                result.sign = -1;
                reverse_bytes(result);
                --result;
            }
            else {
                result.sign = 1;
            }
        }
    }

    check_size_in_bits(result);
    if (result.cntBits == 0) {
        result.number.clear();
        result.sign = 1;
    }

    return result;
}

BigInt operator-(const BigInt& num1, const BigInt& num2) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>

    BigInt result;

    if (num1.sign == -1 && num2.sign == 1) {
        BigInt number2(num2);
        number2.sign == -1;
        result = number2 + num1;
    }
    else if (num1.sign == -1 && num2.sign == -1) {
        BigInt number1(num1);
        BigInt number2(num2);
        number1.sign = 1;
        number2.sign = 1;
        result = number2 - number1;
    }
    else if (num1.sign == 1 && num2.sign == -1) {
        BigInt number2(num2);
        number2.sign = 1;
        result = num1 + number2;
    }
    else {
        BigInt number2(num2);
        if (num2.number.size() != 0) {
            number2.sign = -1;
        }
        result = num1 + number2;
    }

    if (result.cntBits == 0) {
        result.number.clear();
        result.sign = 1;
    }

    return result;
}

BigInt operator*(const BigInt& num1, const BigInt& number2) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="number2"></param>
    /// <returns></returns>

    BigInt result;

    if (num1.number.size() == 0 || number2.number.size() == 0) {
        return result;
    }

    BigInt num2(number2);

    int cntBit = 0;
    for (int i = 0; i < num1.cntBits; i++) {
        int bit = (num1.number[i / 32] >> i % 32) & 1;
        if (bit == 1) {
            result = result + num2;
        }
        num2 = num2 << 1;
    }

    if (num1.sign == num2.sign) {
        result.sign = 1;
    }
    else {
        result.sign = -1;
    }

    if (result.cntBits == 0) {
        result.number.clear();
        result.sign = 1;
    }

    return result;
}

BigInt operator/(const BigInt& number1, const BigInt& number2) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="number1"></param>
    /// <param name="number2"></param>
    /// <returns></returns>

    if (number2.number.size() == 0) {
        throw std::exception();
    }

    BigInt result;  // result of division

    BigInt num1(number1);  // remainder
    BigInt num2(number2);  // divider

    BigInt num2falsh(number2);
    num1.sign = 1;
    num2.sign = 1;
    num2falsh.sign = 1;

    if (number1.sign == number2.sign) {
        result.sign = 1;
    }
    else {
        result.sign = -1;
    }

    while (num1 > num2) {
        num2 = num2 << 1;
    }

    result.number.push_back(0);

    int cntSteps = 0;
    while (num2 >= num2falsh) {
        result = result << 1;
        if (num1 >= num2) {
            num1 = num1 - num2;
            result.number[0] = result.number[0] | 1;
            check_size_in_bits(result);
        }
        num2 = num2 >> 1;
        cntSteps++;
    }

    if (result.cntBits == 0) {
        result.sign = 1;
        result.number.clear();
    }

    return result;
}

BigInt operator%(const BigInt& number1, const BigInt& number2) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="num1"></param>
    /// <param name="num2"></param>
    /// <returns></returns>
    ///

    if (number2.number.size() == 0) {
        throw std::exception();
    }

    BigInt result;         // result of division
    BigInt num1(number1);  // remainder
    BigInt num2(number2);  // divider
    BigInt num2falsh(number2);
    num1.sign = 1;
    num2.sign = 1;
    num2falsh.sign = 1;

    if (number1.sign == number2.sign) {
        result.sign = 1;
    }
    else {
        result.sign = -1;
    }

    while (num1 > num2) {
        num2 = num2 << 1;
    }

    result.number.push_back(0);

    int cntSteps = 0;
    while (num2 >= num2falsh) {
        result = result << 1;
        if (num1 >= num2) {
            num1 = num1 - num2;
            result.number[0] = result.number[0] | 1;
            check_size_in_bits(result);
        }
        num2 = num2 >> 1;
        cntSteps++;
    }

    if (number1.sign == -1 || number2.sign == -1) {
        num1.sign = -1;
    }

    if (result.cntBits == 0) {
        num1.sign = 1;
        num1.number.clear();
    }

    return num1;
}

BigInt::operator std::string() const {
    /// <summary>
    ///
    /// </summary>

    std::ostringstream str;

    if (this->sign == -1) {
        str << '-';
    }

    int limit = 1000000000, carry = 0;
    long long temp = 0;
    std::vector<unsigned int> output = { 0 };

    int cntBits = this->cntBits;

    while (cntBits > 0) {
        multByTwo(output);
        int bit = (this->number[(cntBits - 1) / 32] >> ((cntBits - 1) % 32)) & 1;
        output[0] = output[0] ^ bit;
        cntBits--;
    }

    for (int i = output.size() - 1; i >= 0; i--) {
        if (output[i] < limit / 10 && i != output.size() - 1) {
            int ten = 1, degree = 0;
            while (ten * 10 < output[i]) {
                ten *= 10;
                degree++;
            }
            int cntZeroes = 8 - degree;
            for (int i = 0; i < cntZeroes; i++) {
                str << 0;
            }
        }
        str << output[i];
    }

    return str.str();
}

BigInt::operator int() const {
    /// <summary>
    ///
    /// </summary>

    return (int)this->number[0] * this->sign;
}

size_t BigInt::size() const {
    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>

    return this->number.size() * 4 + 5;
}


BigInt::BigInt() {
    /// <summary>
    /// </summary>

    this->sign = 1;
    this->cntBits = 0;
}

BigInt::BigInt(int value) {
    /// <summary>
    ///     Constructor which converts INT input to Big number
    /// </summary>
    /// <param name="value"></param>

    if (value == 0) {
        this->sign = 1;
        this->cntBits = 0;
        return;
    }

    int toNum = value;
    if (value < 0) {
        this->sign = -1;
        toNum = ~value + 1;
    }
    else {
        this->sign = 1;
    }
    this->number = { (unsigned int)toNum };

    int cntBits = 0, val = 1;
    while (val <= toNum) {
        val *= 2;
        cntBits++;
    }
    this->cntBits = cntBits;
}

BigInt::BigInt(std::string input) {
    /// <summary>
    ///		Constructor which converts string input to Big number
    /// </summary>
    /// <param name="input">
    ///		Input from console
    /// </param>

    int i = input.size() - 1;
    if (input[0] == '-') {
        this->sign = -1;
    }
    else {
        this->sign = 1;
    }

    std::vector<int> number = { 0 };
    long long temp = 0;
    int degreeOf10 = 0, currIdx = 0, limit = 1000000000;

    while (i >= 0 && input[i] != '-') {
        if (input[i] < 48 || input[i] > 57) {
            throw std::invalid_argument("Invalid argument exception");
        }
        temp += (input[i] - 48) * (long long)pow(10, degreeOf10);
        if (temp >= limit || degreeOf10 >= 9) {
            number[currIdx] = temp % limit;
            temp = (temp - number[currIdx]) / limit;
            number.push_back(0);
            degreeOf10 = 0;
            currIdx++;
        }
        i--;
        degreeOf10++;
    }
    if (temp > 0) {
        number[currIdx] = temp;
    }
    else if (input.size() > 1) {
        number.pop_back();
    }

    this->cntBits = fromInputToBigInt(*this, number);
}

BigInt::BigInt(const BigInt& number) {
    /// <summary>
    /// Similar to (operator=) but it is constructor
    /// </summary>
    /// <param name="number">
    ///		Input data for copying
    /// </param>

    if (this->number.size() != 0) {
        this->number.clear();
    }
    this->cntBits = number.cntBits;
    this->sign = number.sign;

    for (int i = 0; i < number.number.size(); i++) {
        this->number.push_back(number.number[i]);
    }
}


BigInt::~BigInt() {
    /// <summary>
    /// Clear Big number data
    /// </summary>

    this->number.clear();
}


BigInt& BigInt::operator=(const BigInt& number) {
    /// <summary>
    ///		Copies data from one BigInt to another
    /// </summary>
    /// <param name="number">
    ///		Input data
    /// </param>
    /// <returns>
    ///		reference to BigInt with input data
    /// </returns>

    if (this != &number) {
        if (this->number.size() != 0) {
            this->number.clear();
        }
        this->cntBits = number.cntBits;
        this->sign = number.sign;

        for (int i = 0; i < number.number.size(); i++) {
            this->number.push_back(number.number[i]);
        }
    }

    return *this;
}

BigInt BigInt::operator~() const {
    /// <summary>
    ///		Changees every bit of Big number
    /// </summary>
    /// <returns>
    ///		Big number with reversed bits
    /// </returns>

    BigInt number(*const_cast<BigInt*>(this));

    if (number.number.size() == 0) {
        number.number.push_back(0);
    }

    number.sign *= -1;

    --number;

    return number;
}

BigInt& BigInt::operator+=(const BigInt& num) {
    *this = (*this) + num;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& num) {
    *this = (*this) * num;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& num) {
    *this = (*this) - num;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& num) {
    *this = (*this) / num;
    return *this;
}

BigInt& BigInt::operator^=(const BigInt& num) {
    *this = (*this) ^ num;
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& num) {
    *this = (*this) % num;
    return *this;
}

BigInt& BigInt::operator&=(const BigInt& num) {
    *this = (*this) & num;
    return *this;
}

BigInt& BigInt::operator|=(const BigInt& num) {
    *this = (*this) | num;
    return *this;
}

BigInt BigInt::operator+() const {
    /// <summary>
    /// </summary>
    /// <returns>
    ///		Positive interpretation of (this)-number
    /// </returns>

    BigInt result(*this);
    result.sign *= 1;

    return result;
}  // unary +

BigInt BigInt::operator-() const {
    /// <summary>
    /// </summary>
    /// <returns>
    ///		Negative interpretation of (this)-number
    /// </returns>

    BigInt result(*this);
    result.sign *= -1;

    return result;
}

std::ostream& operator<<(std::ostream& o, const BigInt& bigNum) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="o"></param>
    /// <param name="i"></param>
    /// <returns></returns>

    if (bigNum.number.size() == 0) {
        o << 0;
    }
    else {
        o << (std::string)bigNum;
    }
    return o;
}

std::istream& operator>>(std::istream& is, BigInt& number) {
    /// <summary>
    ///
    /// </summary>
    /// <param name="is"></param>
    /// <param name="number"></param>
    /// <returns></returns>

    std::string input;
    is >> input;
    number = BigInt(input);
    return is;
}
