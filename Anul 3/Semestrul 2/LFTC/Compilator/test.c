int f(int n)
{
    if(n<3)return n;
    return n*f(n-1);
}

int main()
{
	int x;
	x=5;
    put_i(f(x));
    return 0;
}