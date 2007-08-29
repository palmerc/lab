void set_array(int num)
{
   int i;
   int array[10];
   for(i=0; i<10; i++)
   {
      array[i] = compare(num, i);
   }
}

int compare(int a, int b)
{
   if (sub(a, b) >= 0)
      return 1;
   else
      return 0;
}

int sub (int a, int b)
{
   return a-b;
}

int main()
{
   set_array(4);
}
