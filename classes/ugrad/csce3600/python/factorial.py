def factorial(n):
   facto = 1L;
   
   if n == 0:
      return facto
   
   i = 1
   while i <= n:
      facto *= i
      i += 1
   
   return facto
   
if __name__ == '__main__':
   print('%d' % factorial(6))
