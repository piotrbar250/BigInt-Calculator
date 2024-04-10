// Piotr Baranowski
// Big Integer System Converter and Calculator

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

void copy(char *source, char *dest, int n)
{
    for (int i = 0; i < n; i++)
        dest[i] = source[i];
    dest[n] = '\0';
}

int *string_to_int(char *arr, int n)
{
    int *intarr;
    intarr = (int *)malloc((n + 1) * sizeof(int));
    for (int i = 0; i < n; i++)
        intarr[i] = (int)arr[i] - (int)'0';
    return intarr;
}

char *int_to_string(int *arr, int n)
{
    char *chararr;
    chararr = (char *)malloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; i++)
        chararr[i] = (char)((int)arr[i] + (int)'0');
    chararr[n] = '\0';
    return chararr;
}

char *normalize(char *arr, int len, int n)
{
    char *normarr;
    normarr = (char *)malloc((n + 1) * sizeof(char));
    for (int i = n - len; i < n; i++)
        normarr[i] = arr[i - (n - len)];
    for (int i = 0; i < (n - len); i++)
        normarr[i] = '0';

    normarr[n] = '\0';
    return normarr;
}

char *shift_left_max(char *arr, int n, int *reslen)
{
    int ind = n - 1;
    for (int i = 0; i < n; i++)
        if (arr[i] != '0')
        {
            ind = i;
            break;
        }
    *reslen = n - ind;
    char *shifted = (char *)malloc((*reslen + 1) * sizeof(char));
    for (int i = 0; i < *reslen; i++)
        shifted[i] = arr[i + ind];

    shifted[*reslen] = '\0';
    return shifted;
}

char *shift_right(char *a, int n, int *reslen)
{
    char *shifted = (char *)malloc((n + 2) * sizeof(char));
    shifted[n] = '0';
    for (int i = 0; i < n; i++)
        shifted[i] = a[i];
    *reslen = n + 1;
    shifted[*reslen] = '\0';
    return shifted;
}

char *add(char *a, char *b, int an, int bn, int *reslen)
{
    int n = max(an, bn) + 1;

    char *acopy = normalize(a, an, n);
    char *bcopy = normalize(b, bn, n);

    int *intarr = string_to_int(acopy, n);
    int *intbarr = string_to_int(bcopy, n);
    int *intsum = (int *)malloc((n + 1) * sizeof(int));

    int overflow = 0;
    for (int i = n - 1; i >= 0; i--)
    {
        int sum = intarr[i] + intbarr[i] + overflow;
        intsum[i] = sum % 10;
        overflow = sum / 10;
    }

    char *chararr = int_to_string(intsum, n);

    char *chararr_shifted = shift_left_max(chararr, n, reslen);

    chararr_shifted[*reslen] = '\0';
    free(acopy);
    free(bcopy);
    free(intarr);
    free(intbarr);
    free(intsum);
    free(chararr);
    return chararr_shifted;
}

char *single_mul(char *a, char b, int an, int *reslen)
{
    char *acopy = normalize(a, an, an + 1);
    an++;
    int *intarr = string_to_int(acopy, an);
    int *intsum = (int *)malloc((an + 1) * sizeof(int));
    int intb = (int)(b) - (int)('0');
    int overflow = 0;

    for (int i = an - 1; i >= 0; i--)
    {
        int sum = intarr[i] * intb + overflow;
        intsum[i] = sum % 10;
        overflow = sum / 10;
    }

    char *chararr = int_to_string(intsum, an);
    char *chararr_shifted = shift_left_max(chararr, an, reslen);

    chararr_shifted[*reslen] = '\0';

    free(acopy);
    free(intarr);
    free(intsum);
    free(chararr);

    return chararr_shifted;
}

char *multiplicate(char *a, char *b, int an, int bn, int *reslen)
{
    char *acopy = (char *)malloc((an + 1) * sizeof(char));
    copy(a, acopy, an);

    int sumlen = 1;
    char *sum = (char *)malloc((sumlen + 1) * sizeof(char));
    sum[0] = '0';
    sum[1] = '\0';

    for (int i = bn - 1; i >= 0; i--)
    {
        int single_mul_arrlen = 0;
        char *single_mul_arr = single_mul(acopy, b[i], an, &single_mul_arrlen);

        char *sumcopy = (char *)malloc((sumlen + 1) * sizeof(char));
        copy(sum, sumcopy, sumlen);
        free(sum);
        sum = add(sumcopy, single_mul_arr, sumlen, single_mul_arrlen, &sumlen);
        free(sumcopy);

        char *acopy2 = (char *)malloc((an + 1) * sizeof(char));
        copy(acopy, acopy2, an);
        free(acopy);
        acopy = shift_right(acopy2, an, &an);

        free(acopy2);
        free(single_mul_arr);
    }

    *reslen = sumlen;
    sum[*reslen] = '\0';

    free(acopy);

    return sum;
}

char *subtract(char *a, char *b, int an, int bn, int *reslen)
{
    if (an < bn)
    {
        char *ret = (char *)malloc((2) * sizeof(char));
        ret[0] = '0';
        ret[1] = '\0';
        return ret;
    }

    int negative = 0;
    int n = an;
    char *bcopy = normalize(b, bn, n);

    int *intarr = string_to_int(a, n);
    int *intbarr = string_to_int(bcopy, n);
    int *intdiff = (int *)malloc((n + 1) * sizeof(int));

    if (an >= bn)
    {
        for (int i = n - 1; i >= 0; i--)
        {
            int diff = intarr[i] - intbarr[i];
            if (diff < 0)
            {
                if (i == 0)
                    negative = 1;
                else
                {
                    diff += 10;
                    intarr[i - 1]--;
                }
            }
            intdiff[i] = diff;
        }
    }
    else
        negative = 1;
    if (negative)
        for (int i = 0; i < n; i++)
            intdiff[i] = 0;

    char *chararr = int_to_string(intdiff, n);
    char *shifted = shift_left_max(chararr, n, reslen);

    free(bcopy);
    free(intarr);
    free(intbarr);
    free(intdiff);
    free(chararr);

    return shifted;
}

char *divide(char *a, char *b, int an, int bn, int *reslen)
{
    if (bn > 0 && b[0] == '0')
    {
        printf("Invalid input: division by 0\n");
        exit(0);
    }

    int tmp_len = 1;
    int *intarr = string_to_int(a, an);
    int *intbarr = string_to_int(b, bn);
    int *intdiv = (int *)malloc((an + 1) * sizeof(int));
    int *inttmp = (int *)malloc((5) * sizeof(int));

    inttmp[0] = 0;

    char *inttmpchar;

    for (int i = 0; i < an; i++)
    {
        if (i != 0)
            free(inttmpchar);
        inttmpchar = int_to_string(inttmp, tmp_len);

        char *inttmpchar_copy = (char *)malloc((tmp_len + 1) * sizeof(char));
        copy(inttmpchar, inttmpchar_copy, tmp_len);
        free(inttmpchar);
        inttmpchar = shift_right(inttmpchar_copy, tmp_len, &tmp_len);
        free(inttmpchar_copy);

        free(inttmp);
        inttmp = string_to_int(inttmpchar, tmp_len);

        inttmp[tmp_len - 1] = intarr[i];

        free(inttmpchar);
        inttmpchar = int_to_string(inttmp, tmp_len);

        inttmpchar_copy = (char *)malloc((tmp_len + 1) * sizeof(char));
        copy(inttmpchar, inttmpchar_copy, tmp_len);
        free(inttmpchar);
        inttmpchar = shift_left_max(inttmpchar_copy, tmp_len, &tmp_len);
        free(inttmpchar_copy);

        free(inttmp);
        inttmp = string_to_int(inttmpchar, tmp_len);

        int cnt = 0;
        while (1)
        {
            inttmpchar[tmp_len] = '\0';

            int diff_len = 0;
            char *diff = subtract(inttmpchar, b, tmp_len, bn, &diff_len);
            if (diff[0] == '0')
            {
                int equal = 1;
                if (tmp_len == bn)
                {
                    for (int j = 0; j < bn; j++)
                        if (inttmpchar[j] != b[j])
                            equal = 0;
                }
                else
                    equal = 0;

                if (equal == 0)
                {
                    free(diff);
                    break;
                }
            }
            free(diff);

            inttmpchar_copy = (char *)malloc((tmp_len + 1) * sizeof(char));
            copy(inttmpchar, inttmpchar_copy, tmp_len);
            free(inttmpchar);
            inttmpchar = subtract(inttmpchar_copy, b, tmp_len, bn, &tmp_len);
            free(inttmpchar_copy);

            cnt++;
        }
        intdiv[i] = cnt;
        free(inttmp);
        inttmp = string_to_int(inttmpchar, tmp_len);
    }
    char *chardiv = int_to_string(intdiv, an);
    char *shifted = shift_left_max(chardiv, an, reslen);

    free(intarr);
    free(intbarr);
    free(intdiv);
    free(inttmp);
    free(inttmpchar);
    free(chardiv);
    return shifted;
}

char *modulo(char *a, char *b, int an, int bn, int *reslen)
{
    char *a_copy = (char *)malloc((an + 1) * sizeof(char));
    copy(a, a_copy, an);

    int retlen = 0;
    char *ret = divide(a_copy, b, an, bn, &retlen);

    int tmplen = 0;
    char *tmp = multiplicate(ret, b, retlen, bn, &tmplen);

    char *mod = subtract(a, tmp, an, tmplen, reslen);

    free(a_copy);
    free(ret);
    free(tmp);

    return mod;
}

char *power2(char *a, char *b, int an, int bn, int *reslen)
{
    char *acopy = (char *)malloc((an + 1) * sizeof(char));
    copy(a, acopy, an);

    if (an == 1 && a[0] == '1')
    {
        *reslen = 1;
        return acopy;
    }

    int *intbarr = string_to_int(b, bn);
    int exp = 0;
    int ten = 1;
    for (int i = bn - 1; i >= 0; i--)
    {
        exp += ten * intbarr[i];
        ten *= 10;
    }

    int sumlen = 1;
    char *sum = (char *)malloc((sumlen + 1) * sizeof(char));
    sum[0] = '1';
    sum[1] = '\0';

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            char *sumcopy = (char *)malloc((sumlen + 1) * sizeof(char));
            copy(sum, sumcopy, sumlen);
            free(sum);
            sum = multiplicate(sumcopy, acopy, sumlen, an, &sumlen);
            free(sumcopy);
        }
        char *tmpcopy = (char *)malloc((an + 1) * sizeof(char));
        char *tmpcopy2 = (char *)malloc((an + 1) * sizeof(char));
        copy(acopy, tmpcopy, an);
        copy(acopy, tmpcopy2, an);
        acopy = multiplicate(tmpcopy, tmpcopy2, an, an, &an);
        free(tmpcopy);
        free(tmpcopy2);
        exp /= 2;
    }
    free(acopy);
    free(intbarr);
    *reslen = sumlen;
    return sum;
}

int *char_to_int(char c, int *reslen)
{
    int *intarr;

    if (c >= '0' && c <= '9')
    {
        *reslen = 1;
        intarr = (int *)malloc(2 * sizeof(int));
        intarr[0] = (int)c - (int)'0';
    }
    else
    {
        *reslen = 2;
        intarr = (int *)malloc(3 * sizeof(int));
        intarr[0] = 1;
        intarr[1] = (int)c - (int)'A';
    }
    return intarr;
}

char string_to_char(char *a, int an)
{
    int *intarr = string_to_int(a, an);
    char c;
    if (an == 1)
    {
        c = (char)(intarr[0] + (int)'0');
    }
    else if (an == 2)
    {
        c = (char)(intarr[1] + (int)'A');
    }
    free(intarr);
    return c;
}

char *convert_to_dec(char *num, char *base, int num_len, int base_len, int *reslen)
{
    int *baseintarr = string_to_int(base, base_len);
    int baseint;
    if (base_len == 1)
    {
        baseint = baseintarr[0];
    }
    else if (base_len == 2)
    {
        baseint = 10 + baseintarr[1];
    }
    else
    {
        printf("Invalid input: Incorrect number system\n");
        exit(0);
    }
    if (baseint < 2 || baseint > 16)
    {
        printf("Invalid input: Incorrect number system\n");
        exit(0);
    }
    for (int i = 0; i < num_len; i++)
    {
        int digitintarrlen;
        int *digitintarr = char_to_int(num[i], &digitintarrlen);
        int digitint;

        if (digitintarrlen == 1)
        {
            digitint = digitintarr[0];
        }
        else if (digitintarrlen == 2)
        {
            digitint = digitintarr[0] * 10 + digitintarr[1];
        }
        else
        {
            printf("Invalid input: Incorrect number system\n");
            exit(0);
        }

        if (digitint < 0 || digitint >= baseint)
        {
            printf("Invalid input: Number does not belong to the specified numerical system\n");
            exit(0);
        }
        free(digitintarr);
    }

    char *multiplier = (char *)malloc((2) * sizeof(char));
    multiplier[0] = '1';
    multiplier[1] = '\0';
    int multiplier_len = 1;

    char *decnum;
    int allocated = 2, decnum_len = 1;
    decnum = realloc(NULL, allocated * sizeof(char));
    decnum[0] = '0';
    decnum[1] = '\0';

    for (int i = num_len - 1; i >= 0; i--)
    {
        int decvalue_len;
        int *decvalue = char_to_int(num[i], &decvalue_len);
        char *decvaluechar = int_to_string(decvalue, decvalue_len);

        char *decvaluechar_copy = (char *)malloc((decvalue_len + 1) * sizeof(char));
        copy(decvaluechar, decvaluechar_copy, decvalue_len);
        free(decvaluechar);
        decvaluechar = multiplicate(decvaluechar_copy, multiplier, decvalue_len, multiplier_len, &decvalue_len);
        free(decvaluechar_copy);

        char *decnum_copy = (char *)malloc((decnum_len + 1) * sizeof(char));
        copy(decnum, decnum_copy, decnum_len);
        free(decnum);
        decnum = add(decnum_copy, decvaluechar, decnum_len, decvalue_len, &decnum_len);
        free(decnum_copy);

        char *multiplier_copy = (char *)malloc((multiplier_len + 1) * sizeof(char));
        copy(multiplier, multiplier_copy, multiplier_len);
        free(multiplier);
        multiplier = multiplicate(multiplier_copy, base, multiplier_len, base_len, &multiplier_len);
        free(multiplier_copy);

        free(decvalue);
        free(decvaluechar);
    }
    free(multiplier);
    free(baseintarr);
    decnum[decnum_len] = '\0';
    *reslen = decnum_len;
    return decnum;
}

char *convert_from_dec(char *num, char *base, int num_len, int base_len, int *reslen)
{
    int converted_len = 0, allocated = 1;
    char *converted;
    converted = realloc(NULL, (allocated + 1) * sizeof(char));

    char *num_copy = (char *)malloc((num_len + 1) * sizeof(char));
    copy(num, num_copy, num_len);
    int num_copy_len = num_len;

    converted[0] = '0';

    while (num_copy[0] != '0' || num_copy_len != 1)
    {
        converted_len++;
        int digitarr_len;
        char *digitarr = modulo(num_copy, base, num_copy_len, base_len, &digitarr_len);
        char digit = string_to_char(digitarr, digitarr_len);

        if (converted_len >= allocated)
        {
            allocated += 2;
            converted = realloc(converted, allocated * sizeof(char));
        }
        converted[converted_len - 1] = digit;

        char *num_copy2 = (char *)malloc((num_copy_len + 1) * sizeof(char));
        copy(num_copy, num_copy2, num_copy_len);
        free(num_copy);
        num_copy = divide(num_copy2, base, num_copy_len, base_len, &num_copy_len);

        free(digitarr);
        free(num_copy2);
    }

    if (converted_len == 0)
        converted_len++;

    free(num_copy);
    *reslen = converted_len;

    converted[*reslen] = '\0';

    for (int i = 0; i < converted_len / 2; i++)
    {
        char tmp = converted[i];
        converted[i] = converted[converted_len - i - 1];
        converted[converted_len - i - 1] = tmp;
    }
    return converted;
}

char *read(FILE *in, int *input_len, char *c)
{
    char *input;
    size_t base_len = 2, len = 0;
    input = realloc(NULL, base_len * sizeof(char));

    while ((*c = getc(in)) != EOF && *c != '\n')
    {
        len++;
        if (len >= base_len)
        {
            base_len += 2;
            input = realloc(input, base_len * sizeof(char));
        }
        input[len - 1] = *c;
    }
    input[len] = '\0';
    *input_len = len;
    return input;
}

void driver_function()
{
    FILE *in = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");
    char c = ' ';
    int cnt = 0;
    while (c != EOF)
    {
        cnt++;
        printf("Calculating test number: %d\n", cnt);
        c = ' ';
        char *returned;
        int input_len1;
        char *input1;
        returned = read(in, &input_len1, &c);
        input1 = (char *)malloc((input_len1 + 1) * sizeof(char));
        copy(returned, input1, input_len1);
        free(returned);

        int input_len2;
        char *input2;
        returned = read(in, &input_len2, &c);
        input2 = (char *)malloc((input_len2 + 1) * sizeof(char));
        copy(returned, input2, input_len2);
        free(returned);

        int input_len3;
        char *input3;
        returned = read(in, &input_len3, &c);
        input3 = (char *)malloc((input_len3 + 1) * sizeof(char));
        copy(returned, input3, input_len3);
        free(returned);

        int input_len4;
        char *input4;
        returned = read(in, &input_len4, &c);
        input4 = (char *)malloc((input_len4 + 1) * sizeof(char));
        copy(returned, input4, input_len4);
        free(returned);

        int input_len5;
        char *input5;
        returned = read(in, &input_len5, &c);
        input5 = (char *)malloc((input_len5 + 1) * sizeof(char));
        copy(returned, input5, input_len5);
        free(returned);

        if (input_len1 > 1 && input1[0] == '0' || input_len2 > 1 && input2[0] == '0' || input_len2 > 1 && input2[0] == '0' || input_len3 > 1 && input3[0] == '0' || input_len4 > 1 && input4[0] == '0' || input_len5 > 1 && input5[0] == '0')
        {
            printf("Invalid input: number contains leading zeros\n");
            exit(0);
        }

        if (input1[0] == '-')
        {
            printf("Invalid input: unsupported operation\n");
            exit(0);
        }

        if (input1[0] != '+' && input1[0] != '*' && input1[0] != '/' && input1[0] != '^' && input1[0] != '%')
        {
            int base_len1, base_len2;
            char *base1 = (char *)malloc(3 * sizeof(char));
            char *base2 = (char *)malloc(3 * sizeof(char));
            base1[0] = input1[0];

            if (input1[1] == ' ')
            {
                base_len1 = 1;
                base2[0] = input1[2];
                if (input_len1 == 4)
                {
                    base_len2 = 2;
                    base2[1] = input1[3];
                }
                else
                    base_len2 = 1;
            }
            else if (input1[2] == ' ')
            {
                base_len1 = 2;
                base1[1] = input1[1];
                base2[0] = input1[3];
                if (input_len1 == 5)
                {
                    base_len2 = 2;
                    base2[1] = input1[4];
                }
                else
                    base_len2 = 1;
            }
            base1[base_len1] = '\0';
            base2[base_len2] = '\0';

            int input_decimal_len3;
            char *input_decimal3 = convert_to_dec(input3, base1, input_len3, base_len1, &input_decimal_len3);

            int input_converted_len3;
            char *input_converted3 = convert_from_dec(input_decimal3, base2, input_decimal_len3, base_len2, &input_converted_len3);

            fputs(input1, out);
            fputs("\n\n", out);
            fputs(input3, out);
            fputs("\n\n", out);
            fputs(input_converted3, out);
            fputs("\n\n", out);

            free(input1);
            free(input2);
            free(input3);
            free(input4);
            free(input5);
            free(input_decimal3);
            free(input_converted3);
            free(base1);
            free(base2);

            printf("Calculated test number: %d\n", cnt);

            continue;
        }

        int input_len6;
        char *input6;
        returned = read(in, &input_len6, &c);
        input6 = (char *)malloc((input_len6 + 1) * sizeof(char));
        copy(returned, input6, input_len6);
        free(returned);

        int input_len7;
        char *input7;
        returned = read(in, &input_len7, &c);
        input7 = (char *)malloc((input_len7 + 1) * sizeof(char));
        copy(returned, input7, input_len7);
        free(returned);

        if (input_len6 > 1 && input6[0] == '0' || input_len7 > 1 && input7[0] == '0')
        {
            printf("Invalid input: number contains leading zeros\n");
            exit(0);
        }

        int base_len;
        char *base;

        if (input_len1 == 3)
        {
            if (input1[0] == '+' || input1[0] == '*' || input1[0] == '/' || input1[0] == '^' || input1[0] == '%')
            {
                base = (char *)malloc(2 * sizeof(char));
                base_len = 1;
                base[0] = input1[2];
                base[1] = '\0';
            }
        }
        else if (input_len1 == 4)
        {
            base = (char *)malloc(3 * sizeof(char));
            base_len = 2;
            base[0] = input1[2];
            base[1] = input1[3];
            base[2] = '\0';
        }

        int input_decimal_len3;
        char *input_decimal3 = convert_to_dec(input3, base, input_len3, base_len, &input_decimal_len3);

        int input_decimal_len5;
        char *input_decimal5 = convert_to_dec(input5, base, input_len5, base_len, &input_decimal_len5);

        int reslen = 0;
        char *ret;
        if (input1[0] == '+')
            ret = add(input_decimal3, input_decimal5, input_decimal_len3, input_decimal_len5, &reslen);
        else if (input1[0] == '*')
            ret = multiplicate(input_decimal3, input_decimal5, input_decimal_len3, input_decimal_len5, &reslen);
        else if (input1[0] == '/')
            ret = divide(input_decimal3, input_decimal5, input_decimal_len3, input_decimal_len5, &reslen);
        else if (input1[0] == '%')
            ret = modulo(input_decimal3, input_decimal5, input_decimal_len3, input_decimal_len5, &reslen);
        else if (input1[0] == '^')
            ret = power2(input_decimal3, input_decimal5, input_decimal_len3, input_decimal_len5, &reslen);

        int ret_converted_len;
        char *ret_converted = convert_from_dec(ret, base, reslen, base_len, &ret_converted_len);

        fputs(input1, out);
        fputs("\n\n", out);
        fputs(input3, out);
        fputs("\n\n", out);
        fputs(input5, out);
        fputs("\n\n", out);
        fputs(ret_converted, out);
        fputs("\n\n", out);

        free(input1);
        free(input2);
        free(input3);
        free(input4);
        free(input5);
        free(input6);
        free(input7);
        free(input_decimal3);
        free(input_decimal5);
        free(ret);
        free(ret_converted);
        free(base);

        printf("Calculated test number: %d\n", cnt);
    }
    fclose(in);
    fclose(out);
    printf("\nThe program ended successfully\n");
}

int main()
{
    printf("Large number calculator\n\n");
    printf("Author: Piotr Baranowski\n\n");
    printf("Note: The input file input.txt must be in the same folder\n\n");
    printf("Results will be saved in the file output.txt\n\n");
    printf("Press q and confirm with enter to close the program or press enter to start ");
    char c = getchar();
    if (c == 'q')
        exit(0);
    printf("Started calculations\n\n\n");
    driver_function();
    return 0;
}