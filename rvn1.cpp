#include <stdio.h>
#include <iostream>
#include <Windows.h>

#pragma warning(disable:4996)

#define INPUT_LENGTH 1234

struct Stack
{
	unsigned char data;
	Stack* next;
};

void Push(Stack** stack, const unsigned char data);
unsigned char Pop(Stack** stack);
unsigned char Get(Stack* stack);
void Print(Stack* stack);
void Delete(Stack** stack);
bool StackIsEmpty(Stack* stack);

bool StandartToRpn(const unsigned char* standart, unsigned char* rpn);
bool isCorrectInput(const unsigned char* str);
bool is_operator(const unsigned char ch);
int priority(const unsigned char ch);
float Calculate(const unsigned char* rpn, float* letterValue);
void InitVar(const unsigned char ch, float* letterValue, bool* letterIsInit);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	unsigned char standart[INPUT_LENGTH];
	unsigned char rpn[INPUT_LENGTH]{ 0 };
	bool varIsInit[UCHAR_MAX] = { false };
	float letterVar[UCHAR_MAX];
	while (true)
	{
		std::cout << "Ввод: ";
		std::cin >> standart;
		if (isCorrectInput(standart))
		{
			unsigned char* ch = standart;
			while (*ch != '\0')
			{
				if (isalpha(*ch))
				{
					InitVar(*ch, letterVar, varIsInit);
				}
				ch++;
			}
		}
		else
		{
			std::cout << "Некрректный ввод. Попробуйте еще раз. У вас все получится :)\n";
			continue;
		}
		if (StandartToRpn(standart, rpn) == false)
		{
			std::cout << "Ошибка преобразования\n";
		}
		std::cout << "ОПЗ: " << rpn << '\n';
		std::cout << "Результат: " << Calculate(rpn, letterVar) << "\n\n";
		rpn[0] = '\0';
		for (int i = 0; i < UCHAR_MAX; i++)
		{
			varIsInit[i] = false;
		}
		char answ;
		std::cout << "1.Продолжить\n";
		std::cout << "0.Выйти\n";
		std::cout << "Выбор: ";
		std::cin >> answ;
		std::cout << '\n';
		switch (answ)
		{
		case '1':
		default:
			break;
		case '0':
			return 0;
		}
	}
}

void InitVar(const unsigned char ch, float* letterValue, bool* varIsInit)
{
	if (varIsInit[(unsigned)ch])
		return;
	std::cout << ch << ": ";
	while (!(std::cin >> letterValue[(unsigned)ch]))
	{
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		std::cout << "НЕ используй НЕ цифры при вводе числа!\n" << ch << ": ";
	}
	varIsInit[(unsigned)ch] = true;
}

bool is_operator(const unsigned char ch)
{
	if (ch == '*' || ch == '/' || ch == '-' || ch == '+')
		return true;
	else
		return false;
}

int priority(const unsigned char ch)
{
	switch (ch)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '(':
	default:
		return 0;
	}
}


bool StandartToRpn(const unsigned char* orig, unsigned char* rpn)
{
	Stack* stack = NULL;
	for (int i = 0; orig[i] != '\0'; i++)
	{
		if (isalpha(orig[i]))
		{
			strncat((char*)rpn, (char*)orig + i, 1);
			continue;
		}

		else if (orig[i] == '(')
		{
			Push(&stack, orig[i]);
			continue;
		}

		else if (orig[i] == ')')
		{
			while (stack != NULL)
			{
				if (Get(stack) == '(')
					break;
				char ch = Pop(&stack);
				strncat((char*)rpn, &ch, 1);
			}

			if (stack == NULL)
			{
				printf("Открывающая скобка не найдена\n");
				return false;
			}

			Pop(&stack);
			continue;
		}

		else if (is_operator(orig[i]))
		{
			if (stack == NULL)
			{
				Push(&stack, orig[i]);
			}

			else if (priority(orig[i]) > priority(Get(stack)))
			{
				Push(&stack, orig[i]);
			}
			else
			{
				while (stack != NULL)
				{
					char ch = Pop(&stack);
					strncat((char*)rpn, &ch, 1);
					if (priority(orig[i]) > priority(Get(stack)))
						break;
				}
				Push(&stack, orig[i]);
			}
		}

		else
		{
			std::cout << "Ошибка. Неподдерживаемый символ\n";
			return false;
		}
	}

	while (stack != NULL)
	{
		unsigned char ch = Pop(&stack);
		strncat((char*)rpn, (char*)&ch, 1);
	}

	return true;
}

bool isCorrectInput(const unsigned char* input)
{
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int operandCount = 0;
	int variableCount = 0;
	typedef std::basic_string<unsigned char> unsignedString;
	unsignedString str = input;
	if (isalpha(str[0]))
	{
		variableCount++;
	}
	else if (str[0] == '(')
		leftBracketCount++;
	else return false;
	int i = 1;
	while (str[i] != '\0')
	{
		if (isalpha(str[i]))
		{
			if (!is_operator(str[i - 1]) && str[i - 1] != '(')
				return false;
			variableCount++;
		}
		else if (is_operator(str[i]))
		{
			if (!isalpha(str[i - 1]))
				if (str[i - 1] != '(' && str[i - 1] != ')')
					return false;
			operandCount++;
		}
		else if (str[i] == '(')
		{
			if (is_operator(str[i - 1]) || str[i - 1] == '(')
				leftBracketCount++;
			else return false;
		}
		else if (str[i] == ')')
		{
			if (isalpha(str[i - 1]) || str[i - 1] == ')')
				rightBracketCount++;
			else return false;
		}

		i++;
	}
	if (str[i - 1] == ')' || isalpha(str[i - 1]))
	{
		if (leftBracketCount == rightBracketCount && operandCount + 1 == variableCount)
			return true;
		else return false;
	}
	else return false;
}

float Calculate(const unsigned char* rvn, float* letterValue)
{
	Stack* stack = NULL;
	float result = 0;
	char chr = 0;
	if (strlen((char*)rvn) == 1)
		return letterValue[rvn[0]];


	while (*rvn != '\0')
	{
		if (isalpha(*rvn))
		{
			Push(&stack, *rvn);
		}
		else
		{
			float arg2 = letterValue[Pop(&stack)];
			float arg1 = letterValue[Pop(&stack)];
			switch (*rvn)
			{
			case '+':
				result = arg1 + arg2;
				break;
			case '-':
				result = arg1 - arg2;
				break;
			case '*':
				result = arg1 * arg2;
				break;
			case '/':
				result = arg1 / arg2;
				break;

			default:
			{
				std::cout << "Ошибка при расчете\n";
				return 0;
			}
			}
			letterValue[(int)chr] = result;
			Push(&stack, chr);
			chr++;
		}
		rvn++;
	}
	Delete(&stack);
	return result;
}

void Push(Stack** stack, const unsigned char val)
{
	Stack* temp = new Stack{ val, *stack };
	*stack = temp;
}

unsigned char Pop(Stack** stack)
{

	if (*stack == NULL)
	{
		printf("%s", "В стэке пусто. Возвращаю нуль\n");
		return 0;
	}

	Stack* temp = *stack;
	char data = (*stack)->data;
	*stack = (*stack)->next;
	delete temp;
	return data;
}

unsigned char Get(Stack* stack)
{
	if (stack == NULL)
	{
		return 0;
	}
	return stack->data;
}

void Print(Stack* stack)
{
	if (stack == NULL)
	{
		printf("Тут ничего нет\n");
		return;
	}

	while (stack != NULL)
	{
		printf("%d ", stack->data);
		stack = stack->next;
	}
	printf("\n");
}

void Delete(Stack** stack)
{
	while ((*stack) != NULL)
	{
		Stack* temp = (*stack);
		(*stack) = (*stack)->next;
		delete temp;
		temp = *stack;
	}
}

bool StackIsEmpty(Stack* stack)
{
	if (stack == NULL) return true;
	else return false;
}
