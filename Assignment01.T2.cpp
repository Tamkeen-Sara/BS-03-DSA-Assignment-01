#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <climits>
#include <cstdint>
#include <string>
using namespace std;

const int BITS_PER_NODE = 64;
const int NODES_REQUIRED = 1024 / BITS_PER_NODE;
const uint64_t MAX_UINT64 = UINT64_MAX;

struct Node {
    uint64_t value;
    Node* next;
    Node(uint64_t val = 0) : value(val), next(nullptr) {}
};

class LargeNumber {
private:
    Node* head;

    void deepCopy(const LargeNumber& other) {
        Node* otherCurrent = other.head;
        Node** current = &head;
        while (otherCurrent) {
            *current = new Node(otherCurrent->value);
            current = &((*current)->next);
            otherCurrent = otherCurrent->next;
        }
    }

public:
    LargeNumber() : head(nullptr) {}

    LargeNumber(uint64_t val) : head(nullptr) {
        head = new Node(val);
    }

    LargeNumber(const LargeNumber& other) : head(nullptr) {
        deepCopy(other);
    }

    LargeNumber& operator=(const LargeNumber& other) {
        if (this != &other) {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            deepCopy(other);
        }
        return *this;
    }

    void generateRandom() {
        srand(time(nullptr));
        for (int i = 0; i < NODES_REQUIRED; ++i) {
            uint64_t randomValue = ((uint64_t)rand() << 32) | rand();
            if (i == NODES_REQUIRED - 1) {
                randomValue |= (1ULL << 63); 
            }
            Node* newNode = new Node(randomValue);
            newNode->next = head;
            head = newNode;
        }
    }

    bool isEven() const {
        return head && !(head->value & 1);
    }

    bool isZero() const {
        Node* current = head;
        while (current) {
            if (current->value != 0) return false;
            current = current->next;
        }
        return true;
    }

    bool isOne() const {
        if (!head || head->value != 1) return false;
        Node* current = head->next;
        while (current) {
            if (current->value != 0) return false;
            current = current->next;
        }
        return true;
    }

    void divideByTwo() {
        uint64_t carry = 0;
        Node* current = head;
        while (current) {
            uint64_t newValue = (current->value >> 1) | (carry << 63);
            carry = current->value & 1;
            current->value = newValue;
            current = current->next;
        }
        if (head && head->value == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void subtract(const LargeNumber& other) {
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;
        uint64_t borrow = 0;

        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value >= otherCurrent->value + borrow) {
                thisCurrent->value -= otherCurrent->value + borrow;
                borrow = 0;
            } else {
                thisCurrent->value = (MAX_UINT64 - otherCurrent->value - borrow + 1 + thisCurrent->value);
                borrow = 1;
            }
            thisCurrent = thisCurrent->next;
            otherCurrent = otherCurrent->next;
        }

        while (thisCurrent && borrow) {
            if (thisCurrent->value >= borrow) {
                thisCurrent->value -= borrow;
                borrow = 0;
            } else {
                thisCurrent->value = MAX_UINT64 - borrow + 1;
                borrow = 1;
            }
            thisCurrent = thisCurrent->next;
        }

        while (head && head->next && head->value == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    LargeNumber mod(const LargeNumber& m) const {
        LargeNumber result = *this;
        while (result >= m) {
            result.subtract(m);
        }
        return result;
    }

    LargeNumber modMul(const LargeNumber& other, const LargeNumber& m) const {
        LargeNumber result;
        LargeNumber temp = *this;

        Node* otherCurrent = other.head;
        while (otherCurrent) {
            for (int i = 0; i < 64; ++i) {
                if (otherCurrent->value & (1ULL << i)) {
                    result = (result + temp).mod(m);
                }
                temp = (temp + temp).mod(m);
            }
            otherCurrent = otherCurrent->next;
        }

        return result;
    }

    LargeNumber modPow(const LargeNumber& exponent, const LargeNumber& m) const {
        if (m.isOne()) return LargeNumber(0);

        LargeNumber result(1);
        LargeNumber base = this->mod(m);
        LargeNumber exp = exponent;

        while (!exp.isZero()) {
            if (!exp.isEven()) {
                result = result.modMul(base, m);
            }
            base = base.modMul(base, m);
            exp.divideByTwo();
        }

        return result;
    }

    bool operator==(const LargeNumber& other) const {
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;

        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value != otherCurrent->value) return false;
            thisCurrent = thisCurrent->next;
            otherCurrent = otherCurrent->next;
        }

        return !thisCurrent && !otherCurrent;
    }

    bool operator>=(const LargeNumber& other) const {
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;

        while (thisCurrent && thisCurrent->value == 0) thisCurrent = thisCurrent->next;
        while (otherCurrent && otherCurrent->value == 0) otherCurrent = otherCurrent->next;

        int thisNodes = 0, otherNodes = 0;
        Node* temp = thisCurrent;
        while (temp) {
            thisNodes++;
            temp = temp->next;
        }
        temp = otherCurrent;
        while (temp) {
            otherNodes++;
            temp = temp->next;
        }

        if (thisNodes > otherNodes) return true;
        if (thisNodes < otherNodes) return false;
        
        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value > otherCurrent->value) return true;
            if (thisCurrent->value < otherCurrent->value) return false;
            thisCurrent = thisCurrent->next;
            otherCurrent = otherCurrent->next;
        }

        return true;
    }

    LargeNumber operator+(const LargeNumber& other) const {
        LargeNumber result;
        Node** resultCurrent = &result.head;
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;
        bool carry = false;

        while (thisCurrent || otherCurrent || carry) {
            uint64_t sum = carry ? 1 : 0;
            if (thisCurrent) {
                sum += thisCurrent->value;
                thisCurrent = thisCurrent->next;
            }
            if (otherCurrent) {
                sum += otherCurrent->value;
                otherCurrent = otherCurrent->next;
            }

            *resultCurrent = new Node(sum);
            resultCurrent = &((*resultCurrent)->next);
            
            carry = (sum < (carry ? 1 : 0)) || 
                    (thisCurrent && sum < thisCurrent->value) || 
                    (otherCurrent && sum < otherCurrent->value);
        }

        return result;
    }

    LargeNumber operator-(const LargeNumber& other) const {
        LargeNumber result = *this;
        result.subtract(other);
        return result;
    }

    bool isPrime() {
        if (isEven()) return false;
        
        LargeNumber two(2);
        LargeNumber one(1);

        LargeNumber d = *this - one;
        int s = 0;
        while (d.isEven()) {
            d.divideByTwo();
            s++;
        }

        for (int i = 0; i < 5; i++) {
            LargeNumber a;
            a.generateRandom();
            a = a.mod(*this);

            if (a.isZero() || a.isOne()) continue;

            LargeNumber x = a.modPow(d, *this);

            if (x.isOne() || x == *this - one) continue;

            bool composite = true;
            for (int j = 0; j < s - 1; j++) {
                x = x.modMul(x, *this);
                if (x == *this - one) {
                    composite = false;
                    break;
                }
            }

            if (composite) return false;
        }

        return true;
    }

    void display() const {
        Node* current = head;
        cout << "\" ";
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
         cout << "\"" << endl;
    }

    ~LargeNumber() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    string consent;
    cout << "\n\t\t\tWelcome To Primality Checker For 1024-Bit Numbers!" << endl;
    do{
    LargeNumber largeNum;
    cout << "\n\tA Random Prime Number is Being Generated!" << endl;
    largeNum.generateRandom();
    cout << "\nA 1024-bit number represented as: ";
    largeNum.display();
    bool isPrime = largeNum.isPrime();
    string Result = (isPrime ? "True" : "False");
    if(Result == "True"){
         cout << "\n\tThe generated Number is A Prime Number!" << endl;
         cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    }
    else {
        cout << "\n\tThe generated Number is not a Prime Number!" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    }
    cout << "\n\tDo You Want To Check The Primality Of Another Number?(Yes/No)" << endl;
    cin >> consent;
    } while(consent == "Yes"|| consent =="yes"); 
    cout << "\n\t\t\tThank You For Using This System!Exiting..." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    return 0;
}