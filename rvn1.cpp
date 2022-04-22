#include <stdio.h>
#include <iostream>

#pragma warning(disable:4996)

#define INPUT_LENGTH 1234

struct Stack
{
    char data;
    Stack* next;
};

void Push(Stack** stack, const char data);
char Pop(Stack** stack);
char Get(Stack* stack);
void Print(Stack* stack);
void Delete(Stack** stack);

bool StandartToRpn(const char* standart, char* rvn);
bool is_operator(const char ch);
int priority(const char ch);
float Calculate(const char* rvn, float* letterValue);

void InitVar(char ch, float* letterValue, bool* letterIsInit);

int main()
{
    setlocale(LC_ALL, "Russian");
    char standart[INPUT_LENGTH];
    char rpn[INPUT_LENGTH]{ '\0' };
    bool varIsInit[UCHAR_MAX] = { false };
    float letterVar[UCHAR_MAX];
    while (true)
    {
        std::cout << "Ввод: ";
        std::cin >> standart;
        {
            char* ch = standart;
            while (*ch != '\0')
            {
                if (isalpha(*ch))
                {
                    InitVar(*ch, letterVar, varIsInit);
                }
                ch++;
            }
        }
        if (StandartToRpn(standart, rpn) == false)
        {
            std::cout << "Ошибка преобразования\n";
            return -1;
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
        std::cin >> answ;
        std::cout << '\n';
        switch (answ)
        {
        case '1':
        default :
            break;
        case '0':
            return 0;
        }
    }
}

void InitVar(char ch, float* letterValue, bool* varIsInit)
{
    if (varIsInit[(int)ch])
        return;
    std::cout << ch << ": ";
    std::cin >> letterValue[(int)ch];
    varIsInit[(int)ch] = true;
}

bool is_operator(const char ch)
{
    if (ch == '*' || ch == '/' || ch == '-' || ch == '+')
        return true;
    else
        return false;
}

int priority(const char ch)
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


bool StandartToRpn(const char* orig, char* rpn)
{
    Stack* stack = NULL;
    for (int i = 0; orig[i] != '\0'; i++)
    {
        if (isalpha(orig[i]))
        {
            strncat(rpn, orig + i, 1);
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
                strncat(rpn, &ch, 1);
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
                    strncat(rpn, &ch, 1);
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
        char ch = Pop(&stack);
        strncat(rpn, &ch, 1);
    }
}

float Calculate(const char* rvn, float* letterValue)
{
    Stack* stack = NULL;
    float result = 0;
    char chr = 0;
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


void Push(Stack** stack, const char val)
{
    Stack* temp = new Stack{ val, *stack };
    *stack = temp;
}

char Pop(Stack** stack)
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

char Get(Stack* stack)
{
    if (stack == NULL)
    {
        printf("%s", "\n");
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
