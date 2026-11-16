#include "BigCalculator.h"
#include <algorithm>
#include <sstream>

BigCalculator::BigCalculator() : normalSize(true) {
    nextTable['a'] = 'b';
    nextTable['b'] = 'h';
    nextTable['h'] = 'g';
    nextTable['g'] = 'd';
    nextTable['d'] = 'f';
    nextTable['f'] = 'c';
    nextTable['c'] = 'k';
    nextTable['k'] = 'i';
    nextTable['i'] = 'e';
    nextTable['e'] = 'j';
    nextTable['j'] = 'a';

    buildSmallTables();
}

char BigCalculator::next(char value) const {
    return nextTable.at(value);
}

char BigCalculator::previous(char value) const {
    for (const auto& entry : nextTable) {
        if (entry.second == value) {
            return entry.first;
        }
    }
    return value;
}

void BigCalculator::buildSmallTables() {
    for (char left : alphabet) {
        for (char right : alphabet) {
            addition[left][right] = addSymbol(left, right);
        }
    }

    for (char left : alphabet) {
        for (char right : alphabet) {
            multiplication[left][right] = multiplySymbol(left, right);
        }
    }
}

char BigCalculator::addSymbol(char left, char right) const {
    char result = left;
    char counter = 'a';

    while (counter != right) {
        result = next(result);
        counter = next(counter);
    }

    return result;
}

char BigCalculator::multiplySymbol(char left, char right) const {
    char result = 'a';
    char counter = 'a';

    while (counter != right) {
        result = addSymbol(result, left);
        counter = next(counter);
    }

    return result;
}

std::pair<char, char> BigCalculator::addDigits(char left, char right, char carry) const {
    char result = left;
    char newCarry = 'a';
    char counter = 'a';

    while (counter != right) {
        if (result == alphabet.back()) {
            result = 'a';
            newCarry = 'b';
        } else {
            result = next(result);
        }

        counter = next(counter);
    }

    if (carry == 'b') {
        if (result == alphabet.back()) {
            result = 'a';
            newCarry = 'b';
        } else {
            result = next(result);
        }
    }

    return {result, newCarry};
}

std::pair<char, char> BigCalculator::subtractDigits(char left, char right, char borrow) const {
    char result = left;
    char newBorrow = 'a';
    char counter = 'a';

    while (counter != right) {
        if (result == 'a') {
            result = alphabet.back();
            newBorrow = 'b';
        } else {
            result = previous(result);
        }

        counter = next(counter);
    }

    if (borrow == 'b') {
        if (result == 'a') {
            result = alphabet.back();
            newBorrow = 'b';
        } else {
            result = previous(result);
        }
    }  

    return {result, newBorrow};
}

bool BigCalculator::validNumber(const std::vector<char>& number) const {
    if (number.empty()) {
        return false;
    }

    if (number.size() > 8) {
        return false;
    }

    for (char value : number) {
        bool found = false;

        for (char symbol : alphabet) {
            if (symbol == value) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

std::vector<char> BigCalculator::normalize(std::vector<char> number) const {
    while (number.size() > 1 and number.front() == 'a') {
        number.erase(number.begin());
    }

    return number;
}

bool BigCalculator::lessOrEqual(const std::vector<char>& left, const std::vector<char>& right) const {
    std::vector<char> normalizedLeft = normalize(left);
    std::vector<char> normalizeRight = normalize(right);

    if (normalizedLeft.size() != normalizeRight.size()) {
        return normalizedLeft.size() < normalizeRight.size();
    }

    auto leftIt = normalizedLeft.begin();
    auto rightIt = normalizeRight.begin();

    while (leftIt != normalizedLeft.end()) {
        auto leftPosition = std::find(alphabet.begin(), alphabet.end(), *leftIt);
        auto rightPosition = std::find(alphabet.begin(), alphabet.end(), *rightIt);

        if (leftPosition != rightPosition) {
            return leftPosition < rightPosition;
        }

        ++leftIt;
        ++rightIt;
    }

    return true;
}

std::vector<char> BigCalculator::addNumbers(const std::vector<char>& left, const std::vector<char>& right) {
    std::vector<char> result;

    auto leftIt = left.rbegin();
    auto rightIt = right.rbegin();

    char carry = 'a';

    while (leftIt != left.rend() or rightIt != right.rend()) {
        char leftDigit = 'a';
        char rightDigit = 'a';

        if (leftIt != left.rend()) {
            leftDigit = *leftIt;
        }

        if (rightIt != right.rend()) {
            rightDigit = *rightIt;
        }

        std::pair<char, char> step = addDigits(leftDigit, rightDigit, carry);

        result.push_back(step.first);
        carry = step.second;

        if (leftIt != left.rend()) {
            ++leftIt;
        }

        if (rightIt != right.rend()) {
            ++rightIt;
        }
    }

    if (carry != 'a') {
        result.push_back(carry);
    }

    std::reverse(result.begin(), result.end());

    if (result.size() > 8) {
        normalSize = false;
        return {};
    }

    return normalize(result);
}

std::vector<char> BigCalculator::subtractNumbers(const std::vector<char>& left, const std::vector<char>& right) {
    std::vector<char> result;

    auto leftIt = left.rbegin();
    auto rightIt = right.rbegin();

    char borrow = 'a';

    while (leftIt != left.rend()) {
        char rightDigit = 'a';

        if (rightIt != right.rend()) {
            rightDigit = *rightIt;
        }

        std::pair<char, char> step = subtractDigits(*leftIt, rightDigit, borrow);

        result.push_back(step.first);
        borrow = step.second;

        ++leftIt;

        if (rightIt != right.rend()) {
            ++rightIt;
        }
    }

    std::reverse(result.begin(), result.end());

    return normalize(result);
}

std::vector<char> BigCalculator::multiplyBySymbol(const std::vector<char>& number, char multiplier) {
    std::vector<char> result(1, 'a');
    char counter = 'a';

    while (counter != multiplier) {
        result = addNumbers(result, number);

        if (!normalSize) {
            return {};
        }

        counter = next(counter);
    }

    return normalize(result);
}

std::vector<char> BigCalculator::shiftLeft(std::vector<char> number, const std::vector<char>& shift) const {
    for (char ignored : shift) {
        (void)ignored;
        number.push_back('a');
    }

    return normalize(number);
}

std::vector<char> BigCalculator::appendDigit(std::vector<char> number, char digit) const {
    number.push_back(digit);
    return normalize(number);
}

std::vector<char> BigCalculator::multiplyNumbers(const std::vector<char>& left, const std::vector<char>& right) {
    std::vector<char> result(1, 'a');
    std::vector<char> shift;

    for (auto digit = right.rbegin(); digit != right.rend(); digit++) {
        std::vector<char> partial = multiplyBySymbol(left, *digit);

        if (!normalSize) {
            return {};
        }

        partial = shiftLeft(partial, shift);
        result = addNumbers(result, partial);

        if (!normalSize) {
            return {};
        }

        shift.push_back('a');
    }

    return normalize(result);
}

std::pair<std::vector<char>, std::vector<char>> BigCalculator::divideNumbers(const std::vector<char>& dividend, const std::vector<char>& divisor) {
    std::vector<char> quotient;
    std::vector<char> remainder(1, 'a');

    for (char digit : dividend) {
        remainder = appendDigit(remainder, digit);
        char selected = 'a';
        std::vector<char> selectedProduct(1, 'a');

        for (char candidate : alphabet) {
            std::vector<char> product = multiplyBySymbol(divisor, candidate);

            if (!normalSize) {
                return {{}, {}};
            }

            if (lessOrEqual(product, remainder)) {
                selected = candidate;
                selectedProduct = product;
            }
        }

        quotient.push_back(selected);

        remainder = subtractNumbers(remainder, selectedProduct);
    }

    return {normalize(quotient), normalize(remainder)};
}

std::vector<char> BigCalculator::add(const std::vector<char>& num1, const std::vector<char>& num2) {
    normalSize = true;

    if (!validNumber(num1) or !validNumber(num2)) {
        return {};
    }

    return addNumbers(num1, num2);
}

std::vector<char> BigCalculator::subtract(const std::vector<char>& num1, const std::vector<char>& num2) {
    normalSize = true;

    if (!validNumber(num1) or !validNumber(num2)) {
        return {};
    }

    if (!lessOrEqual(num2, num1)) {
        return {};
    }

    return subtractNumbers(num1, num2);
}

std::vector<char> BigCalculator::multiply(const std::vector<char>& num1, const std::vector<char>& num2){
    normalSize = true;

    if (!validNumber(num1) or !validNumber(num2)) {
        return {};
    }

    return multiplyNumbers(num1, num2);
}

std::string BigCalculator::division(const std::vector<char>& num1, const std::vector<char>& num2){
    normalSize = true;

    if (!validNumber(num1) or !validNumber(num2)) {
        return "error";
    }

    std::vector<char> divisor = normalize(num2);

    if (divisor.size() == 1 and divisor.front() == 'a') {
        return "error";
    }

    std::pair<std::vector<char>, std::vector<char>> result = divideNumbers(normalize(num1), divisor);

    if (!normalSize) {
        return "overflow";
    }

    std::ostringstream output;

    output << "Целое = ";

    for (char value : result.first) {
        output << value;
    }

    output << ", остаток = ";

    for (char value : result.second) {
        output << value;
    }

    return output.str();
}

//std::string BigCalculator::pow(const std::vector<char>& num1, const std::vector<char>& num2){
//    return std::string();
//}

//std::vector<char> BigCalculator::gcd(const std::vector<char>& num1, const std::vector<char>& num2){
//    return std::vector<char>();
//}

//std::vector<char> BigCalculator::lcm(const std::vector<char>& num1, const std::vector<char>& num2){
//    return std::vector<char>();
//}