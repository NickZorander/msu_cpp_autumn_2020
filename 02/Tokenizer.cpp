#include <iostream>
#include <string>
#include <functional>
#include <cctype>


using namespace std;

//typedef void(*tokenHandler) (string);

using tokenHandler = function<void(string)>;

class TokenParser
{
	tokenHandler startHandler, digitHandler, stringHandler, endHandler;
	char* data;
	size_t len;

	enum parserState
	{
		digitToken, stringToken, separator
	};


	public:
		TokenParser(const char* str, size_t size) 
		{
			data = new char[size];
			len = size;
			
			for (size_t i = 0; i < len; ++i)
			{
				data[i] = str[i];
			}
		}
		
		void SetStartCallback(tokenHandler t)
		{
			startHandler = t;
		}

		void SetDigitTokenCallback(tokenHandler t)
		{
			digitHandler = t;
		}

		void SetStringTokenCallback(tokenHandler t)
		{
			stringHandler = t;
		}

		void SetEndCallback(tokenHandler t)
		{
			endHandler = t;
		}

		~TokenParser()
		{
			delete[] data;
		}

		

		void Parse()
		{
			startHandler("startParse ");

			parserState  state = separator;
			string currentToken;

			for (size_t i =0; i< len; ++i)
			{				
				char c = data[i];
				if (state == separator)
				{
					if (isspace(c))
					{
						continue;
					}
					
					if (isdigit(c))
					{
						state = digitToken;
						currentToken += c;
						continue;
					}

					if (isalpha(c))
					{
						state = stringToken;
						currentToken += c;
						continue;
					}
				}

				if (state == digitToken)
				{
					if (isdigit(c))
					{
						currentToken += c;
						continue;
					}
					else if (isspace(c))
					{
						digitHandler(currentToken);
						currentToken = *new string();
						state = separator;
						continue;
					}
				}

				if (state == stringToken)
				{
					if (isalpha(c))
					{
						currentToken += c;
						continue;
					}
					else if (isspace(c))
					{
						stringHandler(currentToken);
						currentToken = *new string();
						state = separator;
						continue;
					}
				}
			}

			if (state != separator) 
			{
				if (state == digitToken)
				{
					digitHandler(currentToken);
				}

				if (state == stringToken)
				{
					stringHandler(currentToken);
				}
			}

			endHandler("endParse");
		}
};



string result = "";

void testHandler(string s)
{
	result += s + "<!>";
}

void strHdlr(string s)
{
	result += "str<!>";
}

void digHdlr(string s)
{
	result += "dig<!>";
}

void UnitTests()
{
	string str1 = "123456789 12 asad aAqw 20 a 2";
	string str2 = "         2             ";
	string str3 = "a";

	TokenParser parser1(str1.c_str(), str1.length());
	parser1.SetStartCallback(testHandler);
	parser1.SetDigitTokenCallback(testHandler);
	parser1.SetStringTokenCallback(testHandler);
	parser1.SetEndCallback(testHandler);
	parser1.Parse();
	if (result == "startParse <!>123456789<!>12<!>asad<!>aAqw<!>20<!>a<!>2<!>endParse<!>")
		cout << "SUCCES" << endl;
	else
		cout << "FAIL" << endl;

	result = "";
	TokenParser parser2(str2.c_str(), str2.length());
	parser2.SetStartCallback(testHandler);
	parser2.SetDigitTokenCallback(testHandler);
	parser2.SetStringTokenCallback(testHandler);
	parser2.SetEndCallback(testHandler);
	parser2.Parse();
	if (result == "startParse <!>2<!>endParse<!>")
		cout << "SUCCES" << endl;
	else
		cout << "FAIL" << endl;

	result = "";
	TokenParser parser3(str3.c_str(), str3.length());
	parser3.SetStartCallback(testHandler);
	parser3.SetDigitTokenCallback(testHandler);
	parser3.SetStringTokenCallback(testHandler);
	parser3.SetEndCallback(testHandler);
	parser3.Parse();
	if (result == "startParse <!>a<!>endParse<!>")
		cout << "SUCCES" << endl;
	else
		cout << "FAIL" << endl;

	result = "";
	string str4 = "aaa 666";
	TokenParser parser4(str4.c_str(), str4.length());
	parser4.SetStartCallback(testHandler);
	parser4.SetDigitTokenCallback(digHdlr);
	parser4.SetStringTokenCallback(strHdlr);
	parser4.SetEndCallback(testHandler);
	parser4.Parse();
	if (result == "startParse <!>str<!>dig<!>endParse<!>")
		cout << "SUCCES" << endl;
	else
		cout << "FAIL" << endl;
}


int main()
{
	UnitTests();
}

