#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define BIGINT_MAX_LEN (20647 + 2)
#define ERROR_MSG_SIZE 256
#define bool int
#define true 1
#define false 0
#define POSITIVO 0
#define NEGATIVO 1
#define ERROR(format, ...) report_error(__FILE__, __LINE__, format, __VA_ARGS__)
#define ctoi(x) ((x) - '0') // char to int
#define itoc(x) ((x) + '0') // int to char
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define abs(x) ((x) < 0 ? -(x) : (x))

// Estrutura que armazena um número inteiro grande
typedef struct numero_inteiro
{
    bool sinal;
    char valor[BIGINT_MAX_LEN];
} bigint;

// // Estrutura que armazena o numerador e o denominador de uma fração (não usada por enquanto)
// typedef struct fracao
// {
//     bool sinal;
//     char numerador[BIGINT_MAX_LEN];
//     char denominador[BIGINT_MAX_LEN];
// } fracao;

// Estrutura que guarda um inteiro e um bigint
typedef struct pair_int_bigint
{
    int valor_int;
    bigint valor_bigint;
} pair_int_bigint;

void report_error(const char *, int, const char *, ...);
void print_bigint(const char *, bigint *, const char *);
void imprimir_menu();
void swap(void *, void *, size_t);
int compare_bigint(bigint *, bigint *, bool);
bool is_valid_integer(char *);
bool is_zero(bigint *);
bigint new_bigint(char *);
bigint soma(bigint *, bigint *);
bigint subtracao(bigint *, bigint *);
bigint produto(bigint *, bigint *);
bigint divisao(bigint *, bigint *);
bigint produto_por_definicao(bigint *, bigint *);
bigint divisao_por_definicao(bigint *, bigint *);
bigint potenciacao(bigint *, bigint *);
pair_int_bigint quociente_e_resto(bigint *, bigint *);

// ---------------------------------------------------------------------

int main()
{
    int STR_SIZE = 1000;
    char val1[STR_SIZE];
    char val2[STR_SIZE];

    while (1) {
        // imprimir_menu();

        printf("Entre com dois valores: ");
        scanf(" %s %s", val1, val2);

        bigint m = new_bigint(val1);
        bigint n = new_bigint(val2);

        // int check = compare_bigint(&m, &n, true);
        // print_bigint("", &m, "");
        // printf(" %s ", check == -1 ? "<" : check == 0 ? "==" : ">");
        // print_bigint("", &n, "\n");

        // check = compare_bigint(&m, &n, false);
        // print_bigint("|", &m, "|");
        // printf(" %s ", check == -1 ? "<" : check == 0 ? "==" : ">");
        // print_bigint("|", &n, "|\n");

        print_bigint("Valor 1: ", &m, "\n");
        print_bigint("Valor 2: ", &n, "\n");


        bigint sum = soma(&m, &n);
        print_bigint("Soma = ", &sum, "\n");
        bigint sub = subtracao(&m, &n);
        print_bigint("Diferenca = ", &sub, "\n");
        bigint prod = produto(&m, &n);
        print_bigint("Produto = ", &prod, "\n");
        bigint quoc = divisao(&m, &n);
        print_bigint("Divisao = ", &quoc, "\n");
        bigint pote = potenciacao(&m, &n);
        print_bigint("Potencia = ", &pote, "\n");


    }

    return 0;
}

// ---------------------------------------------------------------------

// Imprime uma mensagem de erro personalizada
void report_error(const char *file, int line, const char *format, ...)
{
    char error_message[ERROR_MSG_SIZE];

    // Inicializa a lista de argumentos
    va_list args;
    va_start(args, format);

    // Formata a mensagem de erro
    vsnprintf(error_message, ERROR_MSG_SIZE, format, args);

    // Finaliza a lista de argumentos
    va_end(args);

    // Exibe a mensagem de erro com detalhes
    fprintf(stderr, "Erro: %s\n", error_message);
    fprintf(stderr, "Arquivo: %s\n", file);
    fprintf(stderr, "Linha: %d\n", line);

    // Finaliza o programa
    exit(EXIT_FAILURE);
}

// ---------------------------------------------------------------------

void imprimir_menu()
{
    // char *linha = "---------------------------------------";
    char *txt_menu_principal = 
"\
+-------------------------------------+\n\
|             CALCULADORA             |\n\
+-------------------------------------+\n\
    [ 1 ] - Soma                       \n\
    [ 2 ] - Subtracao                  \n\
    [ 3 ] - Produto                    \n\
    [ 4 ] - Divisao                    \n\
    [ 5 ] - Potenciacao                \n\
    [ 6 ] - Configuracoes              \n\
    [ 0 ] - Sair                       \n";

    printf("%s", txt_menu_principal);
    int opcao;
    printf("Escolha: ");
    scanf("%d", &opcao);

}

// ---------------------------------------------------------------------

// Checa se a string fornecida corresponde a um número inteiro válido
bool is_valid_integer(char *num)
{
    if (*num == '+' || *num == '-') num ++; // ignora o sinal, se houver
    if (!*num) return false; // confere se a string não está vazia
    num --;

    while (*(++ num))                 // percorre a string
        if (*num < '0' || *num > '9') // checa se os caracteres são todos dígitos (0-9)
            return false;

    return true;
}

// ---------------------------------------------------------------------

// Retorna se (n) é igual a 0
bool is_zero(bigint *n)
{
    return n->valor[0] == '0';
}

// ---------------------------------------------------------------------

// Retorna se (n) é par
bool is_even(bigint *n)
{
    return ctoi(n->valor[strlen(n->valor) - 1]) % 2 == 0;
}

// ---------------------------------------------------------------------

// Converte a string fornecida para o tipo bigint
bigint new_bigint(char *valor)
{
    bool is_signed = (valor[0] == '+' || valor[0] == '-'), is_zero = true;

    // Checa se o valor é um número inteiro válido
    if (!is_valid_integer(valor))
    {
        ERROR("ValueError: \"%s\" (str) for bigint", valor);
    }

    // Pegar o índice de onde os zeros à esquerda acabam
    int zero_count = is_signed;
    while (valor[zero_count])
    {
        if (valor[zero_count] != '0')
        {
            is_zero = false;
            break;
        }
        zero_count ++;
    }

    // Checa se o valor está dentro do limite permitido
    if (strlen(valor) - is_signed - zero_count >= BIGINT_MAX_LEN)
    {
        ERROR("O valor %s esta fora dos limites para o tipo bigint (o valor maximo permitido eh 10^%d - 1)", valor, BIGINT_MAX_LEN - 1);
    }

    bigint new_int;

    // Se o valor for zero, deixa um caractere zero na string
    if (is_zero)
        zero_count --;

    // Copia a string a partir da posição sem sinal e sem zeros à esquerda
    strcpy(new_int.valor, &valor[zero_count]);

    new_int.sinal = (valor[0] == '-') && !is_zero; // Se for zero, não põe o sinal

    return new_int;
}

// ---------------------------------------------------------------------

// Imprime o bigint fornecido
void print_bigint(const char *label, bigint *n, const char *end)
{
    printf("%s%s%s%s", label, n->sinal ? "-" : "", n->valor, end);
}

// ---------------------------------------------------------------------

// Troca o conteúdo entre duas variáveis do tipo char
void swap(void *a, void *b, size_t size)
{
    unsigned char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

// ---------------------------------------------------------------------

// Retorna -1 se a < b, 0 se a == b e 1 se a > b
int compare_bigint(bigint *a, bigint *b, bool considerar_sinal)
{
    if (considerar_sinal)
    {
        // Um é positivo e o outro é negativo
        if (a->sinal != b->sinal)
            return a->sinal == NEGATIVO ? -1 : 1;

        int len_a = strlen(a->valor), len_b = strlen(b->valor);

        // A quantidade de algarismos é igual, então compara as strings com ajuste do sinal
        if (len_a == len_b)
            return strcmp(a->valor, b->valor) * (a->sinal == POSITIVO ? 1 : -1);

        if (len_a < len_b)
            return (a->sinal == POSITIVO) ? -1 : 1;
        
        // len_b < len_a
        return (a->sinal == POSITIVO) ? 1 : -1;
    }

    // -----------------------------------
    // Sem considerar o sinal

    int len_a = strlen(a->valor), len_b = strlen(b->valor);

    if (len_a == len_b)
        return strcmp(a->valor, b->valor);
    
    return (len_a < len_b) ? -1 : 1;
}

// ---------------------------------------------------------------------

// Inverte os caracteres da string fornecida
void inverter_string(char *str)
{
    int i, len = strlen(str);
    for (i = 0; i < (len >> 1) ; i ++)
        swap(&str[i], &str[len - 1 - i], sizeof(char));
}

// ---------------------------------------------------------------------

// Retorna a soma entre (a) e (b)
bigint soma(bigint *a, bigint *b)
{
    char aux_sum[BIGINT_MAX_LEN]; // variável auxiliar para guardar a soma (com os caracteres na ordem inversa)
    int len_a = strlen(a->valor), len_b = strlen(b->valor), len_s = 0, carry = 0;

    // Se os números possuem sinais diferentes, retorna a diferença entre eles
    // (obs.: quando possível, mudar essa gambiarra)
    if (a->sinal != b->sinal)
    {
        int comp = compare_bigint(a, b, true);

        bigint resultado;

        if (comp == 1) // (b) é o negativo, então retorna a - (-b)
        {
            b->sinal = !b->sinal;
            resultado = subtracao(a, b);
            b->sinal = !b->sinal;
        }
        else // (a) é o negativo, então retorna b - (-a)
        {
            a->sinal = !a->sinal;
            resultado = subtracao(b, a);
            a->sinal = !a->sinal;
        }

        return resultado;
    }

    // Soma cada casa até algum dos valores "acabar"
    while (len_a --, len_b --, len_a >= 0 && len_b >= 0)
    {
        int s = ctoi(a->valor[len_a]) + ctoi(b->valor[len_b]) + carry; // soma os valores e a carry
        aux_sum[len_s ++] = itoc(s % 10);
        carry = s / 10;
    }

    // Se ainda resta algo em a, some-o
    while (len_a >= 0)
    {
        int s = ctoi(a->valor[len_a --]) + carry;
        aux_sum[len_s ++] = itoc(s % 10);
        carry = s / 10;
    }

    // Se ainda resta algo em b, some-o
    while (len_b >= 0)
    {
        int s = ctoi(b->valor[len_b --]) + carry;
        aux_sum[len_s ++] = itoc(s % 10);
        carry = s / 10;
    }

    // Soma a carry, se houver
    if (carry) aux_sum[len_s ++] = itoc(carry);

    // Adicione o sinal da soma
    if (a->sinal)
        aux_sum[len_s ++] = '-';
    
    // Adiciona o caracetere terminador à string
    aux_sum[len_s] = '\0';

    // Inverte a soma guardada em aux_sum
    inverter_string(aux_sum);

    // Retorna o resultado da soma
    return new_bigint(aux_sum);
}

// ---------------------------------------------------------------------

// Retorna a diferença entre (a) e (b)
bigint subtracao(bigint *a, bigint *b)
{
    // Sinais diferentes é equivalente à soma de (a) por (-b)
    // (obs.: quando possível, mudar essa gambiarra)
    if (a->sinal != b->sinal)
    {
        b->sinal = !b->sinal; // inverte o sinal de (b)
        bigint resultado = soma(a, b);
        b->sinal = !b->sinal; // desfaz a ação anterior

        return resultado;
    }

    int comp = compare_bigint(a, b, true);

    if (comp == 0) // Subtração do número por ele mesmo
        return new_bigint("0");
    
    if ((comp == -1 && a->sinal == POSITIVO) || (comp == 1 && a->sinal == NEGATIVO))         // (a) é menor que (b)
        swap(&a, &b, sizeof(a)); // faz (a) e (b) trocarem de valor    

    // -----------------------------------
    //     Subtração com sinais iguais
    // print_bigint("Subtraindo ", a, "");
    // print_bigint(" e ", b, "\n");

    char aux_sub[BIGINT_MAX_LEN] = {}; // variável auxiliar para guardar o resultado (com os caracteres na ordem inversa)
    int len_a = strlen(a->valor), len_b = strlen(b->valor), len_s = 0, carry = 0; // variáveis para realizar a subtração
    int sinal = (comp == -1); // Se |(a)| < |(b)|, então (a) - (b) < 0

    len_a --, len_b --;
    
    while (len_a >= 0 && len_b >= 0)
    {
        int s = 10 + ctoi(a->valor[len_a --]) - carry - ctoi(b->valor[len_b --]);
        carry = s < 10;
        aux_sub[len_s ++] = itoc(s % 10);
    }

    // Carrega a carry pelos dígitos restantes em (a)
    while (len_a >= 0)
    {
        int s = 10 + ctoi(a->valor[len_a --]) - carry;
        carry = s < 10;
        aux_sub[len_s ++] = itoc(s % 10);
    }

    // Se o resultado é negativo, põe o sinal
    if (sinal)
        aux_sub[len_s ++] = '-';

    // Adiciona o caracetere terminador à string
    aux_sub[len_s] = '\0';

    inverter_string(aux_sub);

    return new_bigint(aux_sub);
}

// ---------------------------------------------------------------------

// Retorna o produto entre (a) e (b)
bigint produto(bigint *a, bigint *b)
{
    int len_a = strlen(a->valor), len_b = strlen(b->valor), len_s = 0;
    int carry[len_a + len_b]; // carry para todas as casas dos algarismos
    int index, i, j;
    char aux_prod[BIGINT_MAX_LEN] = {}; // variável auxiliar para guardar o produto (com os caracteres na ordem inversa)

    memset(carry, 0, sizeof(carry));

    for (i = 0; i < len_a; i ++)
    {
        for (j = 0; j < len_b; j ++)
        {
            index = i + j;
            int p = ctoi(a->valor[len_a - 1 - i]) * ctoi(b->valor[len_b - 1 - j]);

            carry[index] += p % 10;
            carry[index + 1] += p / 10;
        }
    }

    for (i = 0; i < len_a + len_b; i ++)
    {
        aux_prod[len_s ++] = itoc(carry[i] % 10);
        carry[i + 1] += carry[i] / 10;
    }

    // jogo dos sinais
    aux_prod[len_s ++] = (a->sinal != b->sinal ? '-' : '+');

    inverter_string(aux_prod);

    return new_bigint(aux_prod);
}

// ---------------------------------------------------------------------

// Retorna o produto entre (a) e (b)
// (Tempo de execução demorado para números grandes)
bigint produto_por_definicao(bigint *a, bigint *b)
{
    // Otimização para o caso de algum dos valores ser 0
    if (is_zero(a) || is_zero(b))
        return new_bigint("0");

    bigint um = new_bigint("1"); // referência para o número 1
    bigint resultado = new_bigint("0"); // resultado = 0
    bigint i = new_bigint("0"); // i = 0

    // Otimização para quando a < b
    if (compare_bigint(a, b, false) == -1)
        swap(&a, &b, sizeof(a));

    while (compare_bigint(&i, b, false) != 0) // enquanto i != b
    {
        resultado = soma(&resultado, a); // resutaldo += a
        i = soma(&i, &um); // i ++
    }

    // jogo dos sinais
    resultado.sinal = (a->sinal != b->sinal);

    // Se for 0, tira o sinal
    if (is_zero(&resultado))
        resultado.sinal = POSITIVO;

    return resultado;
}

// ---------------------------------------------------------------------

// Retorna o quociente (bigint) entre (a) e (b)
// (Tempo de execução demorado quando b é significativamente menor que a)
bigint divisao_por_definicao(bigint *a, bigint *b)
{
    if (is_zero(b))
    {
        ERROR("ZeroDivisionError: %s\n", "divison by zero");
    }

    bigint resultado = new_bigint("0"); // resultado = 0
    bigint um = new_bigint("1"); // referência para o número 1
    bigint aux_a = new_bigint(a->valor); // auxiliar de |a|
    bigint aux_b = new_bigint(b->valor); // auxiliar de |b|

    while (compare_bigint(&aux_a, &aux_b, false) >= 0) // enquanto a >= b
    {
        resultado = soma(&resultado, &um); // resutaldo += 1
        aux_a = subtracao(&aux_a, &aux_b); // aux_a -= b
    }

    // jogo dos sinais
    resultado.sinal = (a->sinal != b->sinal);

    // Se for 0, tira o sinal
    if (is_zero(&resultado))
        resultado.sinal = POSITIVO;

    return resultado;
}

// ---------------------------------------------------------------------

// Retorna o quociente (int) e resto (bigint) da divisão entre (a) e (b)
// (Obs. 1: Garanta que o resultado caiba no tipo de dado _int_ antes de usar)
// (Obs. 2: Ineficiente para números grandes, pois aplica divisão por definição)
pair_int_bigint quociente_e_resto(bigint *a, bigint *b)
{
    if (is_zero(b))
    {
        ERROR("ZeroDivisionError: %s\n", "divison by zero");
    }

    int quociente = 0;
    bigint aux_a = new_bigint(a->valor); // auxiliar de |a|
    bigint aux_b = new_bigint(b->valor); // auxiliar de |b|

    while (compare_bigint(&aux_a, &aux_b, false) >= 0) // enquanto |a| >= |b|
    {
        aux_a = subtracao(&aux_a, &aux_b); // aux_a -= b
        quociente ++;
    }

    // jogo dos sinais
    if (a->sinal != b->sinal)
        quociente *= -1;

    // sinal do resto da divisão (a) por (b)
    aux_a.sinal = a->sinal;

    // retorna o par {quociente, resto}
    return (pair_int_bigint){.valor_int = quociente, .valor_bigint = aux_a};
}

// ---------------------------------------------------------------------

// Retorna o quociente da divisão inteira entre (a) e (b)
bigint divisao(bigint *a, bigint *b)
{
    if (is_zero(b))
    {
        ERROR("ZeroDivisionError: %s\n", "divison by zero");
    }

    int len_a = strlen(a->valor), len_b = strlen(b->valor), len_s = 0, len_q = 0, index_a = 0;
    char quociente[BIGINT_MAX_LEN] = {};
    bigint numerador = new_bigint("0"); // auxiliar para as subdivisões
    // bigint resto;

    // jogo dos sinais
    quociente[len_q ++] = (a->sinal != b->sinal ? '-' : '+');
    
    while (index_a < len_a) // enquanto (a) ainda possui dígitos para realizar a divisão
    {
        // Pega os primeiros dígitos de (a) para dividir por (b)
        while (index_a < len_a && len_s < len_b)
        {
            numerador.valor[len_s ++] = a->valor[index_a ++];
        }
        numerador.valor[len_s] = '\0';

        // Se o numerador ainda é menor que o denominador, tenta adicionar mais um dígito de (a)
        if (strcmp(numerador.valor, b->valor) < 0 && !is_zero(&numerador))
        {
            // Se ainda não cabe e houver dígitos em (a), abaixa mais um dígito
            if (index_a < len_a)
            {
                numerador.valor[len_s ++] = a->valor[index_a ++];
                numerador.valor[len_s] = '\0';
            }
            // Senão a divisão acabou
            else
            {
                numerador.valor[len_s] = '\0';
                // resto = numerador;
            }
        }
        pair_int_bigint q = quociente_e_resto(&numerador, b);

        quociente[len_q ++] = itoc(abs(q.valor_int));
        numerador = q.valor_bigint;
        len_s = (is_zero(&numerador) ? 0 : strlen(numerador.valor));
    }

    // Adiciona o caracetere terminador à string
    quociente[len_q] = '\0';

    return new_bigint(quociente);
}

// ---------------------------------------------------------------------

int count_pot = 0;
// Retorna o resultado da potenciação (a)^(b)
bigint potenciacao(bigint *a, bigint *b)
{
    if (b->sinal == NEGATIVO)
    {
        ERROR("ExponentiationError: %s", "O expoente deve ser maior ou igual a 0.");
    }

    if (is_zero(a) && is_zero(b))
    {
        ERROR("ZeroElevatedToZero: %s", "Zero elevado a zero (indefinido)");
    }

    if (is_zero(b))
    {
        return new_bigint("1");
    }

    bigint resultado;
    bigint um = new_bigint("1");
    bigint dois = new_bigint("2");

    if (is_even(b))
    {
        bigint aux = divisao(b, &dois);
        resultado = potenciacao(a, &aux); // a^(b/2)
        resultado = produto(&resultado, &resultado); // (a^(b/2))^2 == a^b
    }
    else
    {
        bigint aux1 = subtracao(b, &um);
        bigint aux = potenciacao(a, &aux1); // a ^ (b - 1)
        resultado = produto(a, &aux); // a * (a^(b - 1)) == a^b
    }

    return resultado;
}


// ---------------------------------------------------------------------


