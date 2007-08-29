def factorial(n):
   facto = 1L;
   
   if n == 0:
      return facto
   
   i = 1L
   while i <= n:
      facto *= i
      i += 1
      
   return facto


def gcd(a, b):
   while b != 0:
      t = b
      b = a % b
      a = t
   return a

def sumfractions(anum, bden, cnum, dden):
   anum = dden * anum
   borig = bden
   bden = dden * bden
   cnum = borig * cnum
   dden = borig * dden
   
   numerator = anum + cnum
   denominator = bden
   divisor = gcd(numerator, denominator)
   numerator = numerator / divisor
   denominator = denominator / divisor
   return (numerator, denominator)

def elemdivision(a, b):
   divisor = b
   dividend = a
   remainder = 0
   round
   print '%s %s' % (len(str(divisor)), len(str(dividend)))

if __name__ == '__main__':
   i = 0L
   n = 100
   acumnum = 0L
   acumden = 1L
      
   while i <= n:
      numerator = 1L
      denominator = factorial(i)
      (acumnum, acumden) = sumfractions(numerator, denominator, acumnum, acumden)
      #print '%s / %s' % (acumnum, acumden)

      i += 1
   
   elemdivision(acumnum, acumden)
   print '=%s / %s' % (acumnum, acumden)

   #euler = acumnum / acumden
   #print 'Eulers number is ' + str(euler)

