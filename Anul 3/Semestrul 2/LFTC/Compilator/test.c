int f(int n)
{
    if(n<3) return n;
    return n*f(n-1);
}
int main()
{
    put_i(f(5));
    return 0;
}