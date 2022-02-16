// Approximates the square root of an integer using half-precision floats
// One float is represented as an integer array with a length of 2 (int[2])

int mod(int a, int b) // This compiler doesn't support modulo for some reason
{
    return a - (b * (a / b));
}

int sdiv(int a, int b) // Division for signed numbers, because division doesn't follow the 2's complement
{
    int out = 0;
    if (a > 127 && b > 127)
    {
        out = (-a) / (-b);
    }
    if (a > 127 && b <= 127)
    {
        out = -((-a) / b);
    }
    if (a <= 127 && b > 127)
    {
        out = -(a / (-b));
    }
    if (a <= 127 && b <= 127)
    {
        out = a / b;
    }

    return out;
}

void psint(int a) // Print signed integer
{
    if (a > 127)
    {
        write_char('-');
        write_char(mod((-a) / 100, 10) + '0');
        write_char(mod((-a) / 10, 10) + '0');
        write_char(mod(-a, 10) + '0');
    }
    else
    {
        write_char(mod((a) / 100, 10) + '0');
        write_char(mod((a) / 10, 10) + '0');
        write_char(mod(a, 10) + '0');
    }
}

void fprint(int[] f) // print float in (m*2^e) format
{
    psint(f[0]);
    write_char('*');
    write_char('2');
    write_char('^');
    psint(f[1]);
    write_char('\n');
}

void fset(int[] f, int b, int e) // Set a float
{
    f[0] = b;
    f[1] = e;
}

void finv(int[] a) // Invert the float
{
    a[0] = sdiv(64, a[0]);
    a[1] = -6 - a[1];
}

void fmul(int[] a, int[] b) // Multiply two floats
{
    a[0] = a[0] * b[0];
    a[1] = a[1] + b[1];
}

int bsl(int a, int b) // Bitwise shift left (shift right for negative numbers)
{
    int out = a;
    int i = 0;
    if (b > 127)
    {
        while (i < -b)
        {
            out = out / 2;
            i = i + 1;
        }
    }
    else
    {
        while (i < b)
        {
            out = out * 2;
            i = i + 1;
        }
    }
    return out;
}

bool sgt(int b, int a) // If one signed number is greater than the other
{
    bool signa = a > 127;
    bool signb = b > 127;
    bool out = false;
    if (!((signa || signb) && !(signa && signb))) // Terrible not(xor) because using '==' on booleans doesn't work
    {
        if (signa)
        {
            out = a > b;
        }
        else
        {
            out = (-a) > (-b);
        }
    }
    else
    {
        out = signa && !signb;
    }
    return out;
}

int fround(int[] a) // Convert a float to a rounded integer
{
    return bsl(a[0], a[1]);
}

void fadd(int[] a, int[] b) // Add two floats
{
    int bigB = 0;
    int bigE = 0;
    int smallB = 0;
    int smallE = 0;
    if (sgt(a[1], b[1]))
    {
        bigB = a[0];
        bigE = a[1];
        smallB = b[0];
        smallE = b[1];
    }
    else
    {
        bigB = b[0];
        bigE = b[1];
        smallB = a[0];
        smallE = a[1];
    }
    int dif = bigE - smallE;
    a[0] = bsl(bigB, dif) + smallB;
    a[1] = (smallE);
}

void fcpy(int[] a, int[] b) // Copy one float to another
{
    a[0] = b[0];
    a[1] = b[1];
}

void prints(int[] s) // Print a string
{
    int i = 0;
    while (s[i] != 0)
    {
        write_char(s[i]);
        i = i + 1;
    }
}

void ffullprint(int[] a) // Print a float in 000.000 format
{
    int ten[2];
    int tmp[2];
    fset(ten, 10, 0);
    psint(fround(a));
    write_char('.');
    fcpy(tmp, a);
    fmul(tmp, ten);
    write_char(mod(fround(tmp), 10) + '0');
    fmul(tmp, ten);
    write_char(mod(fround(tmp), 10) + '0');
    fmul(tmp, ten);
    write_char(mod(fround(tmp), 10) + '0');
}

// All floats that will be used
int n[2];
int x[2];
int half[2];
int two[2];
int three[2];
int negone[2];
int temp[2];
int i = 0;

int num = 0;
int guess = 0;

int nstr[] = "Number (int)? ";
prints(nstr);

// Get input
int chr = -1;
while (chr != '\n')
{
    chr = read_char();
    if (chr != '\n')
    {
        num = num * 10 + (chr - '0');
    }
}
// End input

fset(two, 2, 0);
fset(n, num, 0);
fset(x, 1, -1);

fset(half, 1, -1);
fset(negone, -1, 0);

// Do one iteration of the Newton-Raphson method (x = 0.5 * x * (3 - n * x * x))
// This gets the inverse square root
while (i < 1)
{
    fcpy(temp, n);
    fmul(temp, x);
    fmul(temp, x);
    fmul(temp, negone);
    fset(three, 3, 0);
    fadd(three, temp);
    fmul(three, x);
    fmul(three, half);
    fcpy(x, three);
    i = i + 1;
}

finv(x); // Invert the result to just get the square root
ffullprint(x);