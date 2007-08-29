// *********************************************************
// Implementation file cstackat.hpp for the ADT stack.
// Array-based implementation.
// *********************************************************
#include "infixcalculator.h" // header file

template <class T>
cStackAT<T>::cStackAT() : top(-1), capacity(100)
{
  items = new T[100];
} // end default constructor

template <class T>
cStackAT<T>::~cStackAT()
{
  delete [] items;
}

template <class T>
bool cStackAT<T>::Empty() const
{
  return bool(top < 0);

} // end StackIsEmpty

template <class T>
void cStackAT<T>::Push(T newItem,
                      bool& success)
{
  success = bool(top < capacity - 1);

  if (success) // if stack has room for another item
     items[++top] = newItem;

} // end Push

template <class T>
void cStackAT<T>::Pop(bool& success)
{
  success = bool(!Empty());


  if (success) // if stack is not empty,
     --top;    // pop top
} // end Pop

template <class T>
void cStackAT<T>::Pop(T& stackTop,
                     bool& success)
{
  success = bool(!Empty());

  if (success)                // if stack is not empty,
     stackTop = items[top--]; // retrieve top
} // end Pop

template <class T>
void cStackAT<T>::GetStackTop(T& stackTop, bool& success) const
{
  success = bool(!Empty());


  if (success)              // if stack is not empty,
     stackTop = items[top]; // retrieve top
} // end GetStackTop
// End of implementation file.
