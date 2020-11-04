#include <iostream>
#include <random>
#include <time.h>

using digit = int16_t; 
using namespace std;
#define assert(condition){if(!(condition)){std::cout << ">>fail"<<std::endl;}else{std::cout << ">>success"<<std::endl<<std::endl;}}

#define max(a,b) ((a)>(b)?(a):(b))
class BigInt;
ostream& operator<< (ostream &out, const BigInt& num);
class BigInt
{
	digit *digits;
	bool is_negative;
	int size;
public:
	BigInt(int dig = 0)
	{		
		if (is_negative = dig < 0)
		{			
			dig = -dig;
		}

		size = 1;
		int tmp = dig;
		while (tmp = tmp / 10)
		{
			++size;
		}

		digits = new digit[size];
		for (int i = size - 1; i >= 0; --i)
		{
			digits[i] = dig % 10;
			dig /= 10;
		}
		this->deleteZeros();
	}

	BigInt(const string& s)
	{
		if (s[0] == '-')
		{
			size = s.size() - 1;
			digits = new digit[size];
			is_negative = true;
			for (int i = 1; i < s.size(); ++i)
			{
				digits[i-1] = s[i] - '0';
			}
		}
		else
		{
			size = s.size();
			digits = new digit[size];
			is_negative = false;
			for (int i = 0; i < size; ++i)
			{
				digits[i] = s[i] - '0';
			}
		}

		this -> deleteZeros();
	}

	BigInt(int s, bool lenght)
	{
		digits = new digit[s];
		size = s;
		for (int i = 0; i < size; ++i)
		{
			digits[i] = 0;
		}
		is_negative = false;
	}

	BigInt(const BigInt& num)
	{
		size = num.size;
		digits = new digit[size];
		is_negative = num.is_negative;
		for (int i = 0; i < size; ++i)
		{
			digits[i] = num.digits[i];
		}
	}
	
	BigInt(BigInt&& num)
	{
		digits = num.digits;
		size = num.size;
		is_negative = num.is_negative;
		num.digits = nullptr;
	}

	~BigInt()
	{
		if (digits!=nullptr)
			delete[] digits;
	}

	int getsize() const
	{
		return size;
	}

	bool get_is_negative() const
	{
		return is_negative;
	}

	const digit operator[](int i) const
	{
		return digits[i];
	}

	BigInt operator-() const
	{	
		BigInt res = BigInt(*this);
		res.is_negative = !res.is_negative;
		return res;
	}

	void deleteZeros()
	{
		int j = 0;
		int count = 0;
		while (digits[j] == 0 && j!=size-1)
		{
			j++;
			count++;
		}

		if (count != 0)
		{
			digit *tmp = new digit[size - count];
			for (int i = 0; i < size - count; ++i)
			{
				tmp[i] = digits[i + count];
			}
			delete[] digits;
			digits = tmp;
			size = size - count;
		}			
	}

	BigInt &operator=(const BigInt& num)
	{
		if (size != num.size)
		{
			size = num.size;
			delete[] digits;
			digits = new digit[size];
		}
		for (int i = 0; i < size; i++)
			digits[i] = num.digits[i];
		is_negative = num.is_negative;

		return *this;
	}

	bool operator>(const BigInt& num) const
	{
		if (!is_negative && num.is_negative)
			return true;
		if (is_negative && !num.is_negative)
			return false;
		if (!is_negative && !num.is_negative)
		{
			if (size > num.size)
				return true;
			if (size < num.size)
				return false;
			for (int i = 0; i < size; ++i)
			{
				if (digits[i] != num.digits[i])
					return digits[i] > num.digits[i];
			}
		}
		
		if (size > num.size)
			return false;
		if (size < num.size)
			return true;
		for (int i = 0; i < size; ++i)
		{
			if (digits[i] != num.digits[i])
				return digits[i] < num.digits[i];
		}
				
	}

	bool operator<(const BigInt& num) const
	{
		return !(*this > num);
	}

	bool operator==(const BigInt& num) const
	{
		if (size != num.size || is_negative != num.is_negative)
			return false;
		for (int i = 0; i < size; ++i)
		{
			if (digits[i] != num.digits[i])
				return false;
		}
		return true;
	}

	bool operator>=(const BigInt& num) const
	{
		return (*this > num) || (*this == num);
	}

	bool operator<=(const BigInt& num) const
	{
		return (*this < num) || (*this == num);
	}

	bool operator!=(const BigInt& num) const
	{
		return !(*this ==  num);
	}
	
	
	BigInt operator-(const BigInt& num) const
	{
		if (num.is_negative)
			return *this + (-num);

		if (is_negative && !num.is_negative)
			return -(-*this + num);

		if (*this < num)
			return -(num -*this);

		int carry = 0;
		BigInt res(size, true);
		for (int i = size - 1, j = num.size - 1; i >= 0 ; --i, --j)
		{
			res.digits[i] = digits[i] - (carry + (j >= 0 ? num.digits[j] : 0));
			carry = res.digits[i] < 0;
			if (carry)
				res.digits[i] += 10;
			//cout << res;
		}
		res.is_negative = false;
		res.deleteZeros();
		return res;
	}

	BigInt operator+(const BigInt& num) const
	{	
		if (!is_negative && !num.is_negative)
		{
			BigInt res(max(size, num.size)+1, true);
			for (int i = size-1, j=num.size-1, k =res.size-1; k > 0; --k, --i, --j)
			{
				int sum = (i>=0? digits[i] : 0) + (j>=0? num.digits[j]:0) + res.digits[k];
				res.digits[k] = sum % 10;
				res.digits[k - 1] = sum / 10;
				//cout << "sum: " << sum << " i: "<< i<< " j: "<<j<<" k: "<< k<< " res: "<< res;
			}
			res.is_negative = false;
			res.deleteZeros();
			//cout << res << res.size;
			return res;
		}

		if (is_negative && !num.is_negative)
			return num - (-*this);

		if (!is_negative && num.is_negative)
			return *this - (-num);

		if (is_negative && num.is_negative)
			return -(-*this + (-num));
	}

	BigInt operator*(const BigInt& num) const
	{		
		BigInt res(size + num.size, true);
		for (int i = 0; i < size; i++)
			for (int j = 0; j < num.size; j++)
				res.digits[i + j + 1] += digits[i] * num.digits[j];

		for (int i = size + num.size - 1; i > 0; i--)
		{
			res.digits[i - 1] += res.digits[i] / 10;
			res.digits[i] %= 10;
		}
		res.is_negative = (is_negative != num.is_negative);
		res.deleteZeros();
		return res;
	}
};

ostream& operator<< (ostream &out, const BigInt& num)
{
	if (num.get_is_negative())
		out << '-';
	for (int i = 0; i < num.getsize(); ++i)
	{
		out << (num[i]);
	}
	out << endl;
	return out;
}

void calc_test(int reps)
{
	srand(time(0));
	for (int operation = 0; operation <= 2; operation++)
	{
		switch (operation)
		{
		case 0:
			cout << "________ section '+' __________" << endl << endl;
			break;
		case 1:
			cout << "________ section '-' __________" << endl << endl;
			break;
		case 2:
			cout << "________ section '*' __________" << endl << endl;
			break;
		}

		for (int sign_a = -1; sign_a <= 1; sign_a += 2)
			for (int sign_b = -1; sign_b <= 1; sign_b += 2)
			{
				switch (operation)
				{
				case 0:
				{
					for (int i = 0; i < reps; ++i)
					{
						int num_a = rand()%30000 * sign_a, num_b = rand() % 30000 * sign_b;
						BigInt a(num_a), b(num_b);
						cout << num_a << " + (" << num_b << ")"<<  endl;
						bool res = (a + b == num_a + num_b);
						assert(res);
						if (!res)
						{
							cout << "a + b = "<<  num_a + num_b<<endl;
							cout << "computed value: " << a + b <<endl;
						}
					}					
					break;
				}
				case 1:
				{
					for (int i = 0; i < reps; ++i)
					{
						int num_a = rand() % 30000 * sign_a, num_b = rand() % 30000 * sign_b;
						BigInt a(num_a), b(num_b);
						cout << num_a << " - (" << num_b << ")" << endl;
						bool res = (a - b == num_a - num_b);
						assert(res);
						if (!res)
						{
							cout << "a - b = " << num_a - num_b << endl;
							cout << "computed value: " << a - b << endl;
						}
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < reps; ++i)
					{
						int num_a = rand() % 30000 * sign_a, num_b = rand() % 30000 * sign_b;
						BigInt a(num_a), b(num_b);
						cout << num_a << " * (" << num_b << ")" << endl;
						bool res = (a * b == num_a * num_b);
						assert(res);
						if (!res)
						{
							cout << "a * b = " << num_a * num_b << endl;
							cout << "computed value: " << a * b << endl;
						}
					}
					break;
				}
				}
			}
		cout << "_______________________________" << endl << endl;
	}
		
}

void compr_test(int reps)
{
	for (int operation = 0; operation <= 3; operation++)
	{
		switch (operation)
		{
		case 0:
			cout << "________ section '<' __________" << endl << endl;
			break;
		case 1:
			cout << "________ section '<=' __________" << endl << endl;
			break;
		case 2:
			cout << "________ section '>' __________" << endl << endl;
			break;
		case 3:
			cout << "________ section '>=' __________" << endl << endl;
			break;
		}

		for (int sign_a = -1; sign_a <= 1; sign_a += 2)
			for (int sign_b = -1; sign_b <= 1; sign_b += 2)
			{
				switch (operation)
				{
				case 0:
				{
					for (int i = 0; i < reps; ++i)
					{
						int num_a = rand() % 30000 * sign_a, num_b = rand() % 30000 * sign_b;
						BigInt a(num_a), b(num_b);
						cout << num_a << " < (" << num_b << ")" << endl;
						bool res = (a < b == num_a < num_b);
						assert(res);
						if (!res)
						{
							cout << "a < b = " << (num_a < num_b) << endl;
							cout << "computed value: " << (a < b) << endl;
						}
					}
					break;
				}
				case 1:
				{
					for (int i = 0; i < reps; ++i)
					{
						int num_a = rand() % 30000 * sign_a, num_b = rand() % 30000 * sign_b;
						BigInt a(num_a), b(num_b);
						cout << num_a << " <= (" << num_b << ")" << endl;
						bool res = (a <= b == num_a <= num_b);
						assert(res);
						if (!res)
						{
							cout << "a <= b = " << (num_a <= num_b) << endl;
							cout << "computed value: " << (a <= b) << endl;
						}
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < reps; ++i)
					{
						int num_a = rand() % 30000 * sign_a, num_b = rand() % 30000 * sign_b;
						BigInt a(num_a), b(num_b);
						cout << num_a << " > (" << num_b << ")" << endl;
						bool res = (a > b == num_a > num_b);
						assert(res);
						if (!res)
						{
							cout << "a > b = " << (num_a > num_b) << endl;
							cout << "computed value: " << (a > b) << endl;
						}
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < reps; ++i)
					{
						int num_a = rand() % 30000 * sign_a, num_b = rand() % 30000 * sign_b;
						BigInt a(num_a), b(num_b);
						cout << num_a << " >= (" << num_b << ")" << endl;
						bool res = (a >= b == num_a >= num_b);
						assert(res);
						if (!res)
						{
							cout << "a >= b = " << (num_a >= num_b) << endl;
							cout << "computed value: " << (a >= b) << endl;
						}
					}
					break;
				}
				}
			}
		cout << "_______________________________" << endl << endl;
	}
}

int main()
{
	calc_test(3);
	compr_test(1);
	BigInt a = 1;
	BigInt b("123456789012345678901234567890");
	BigInt c = a * b + 2;
	assert(c == BigInt("123456789012345678901234567892"));
	BigInt k(5);
	BigInt d;
	d = std::move(k);
	a = d + b;
	assert(a == BigInt("123456789012345678901234567895"));
	cout << a;
}


