#ifndef _CSTACKAT_H_
#define _CSTACKAT_H_

template <class T>
class cStackAT
{
 public:
  // constructors and destructor:
     cStackAT(); // default constructor
     ~cStackAT();

  // stack operations:
     bool Empty() const;
     void Push(T newItem, bool& success);
     void Pop(bool& success);
     void Pop(T& stackTop, bool& success);
     void GetStackTop(T& stackTop, bool& success) const;

private:
    T *items;         // array of stack items
    int top;          // index to top of stack
    int capacity;     // array capacity
}; // end class

#endif // _CSTACKAT_H_
// End of header file.
