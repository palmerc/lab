class Stack:
    def __init__(self):
        self.stack = []
        
    def push(self, object):
       self.stack.append(object)
            
    def pop(self):
        if len(self.stack) == 0:
            raise "Error", "stack empty"
        else:
            obj = self.stack[-1]
            del self.stack[-1]
            return obj
            
    def is_empty(self):
        if len(self.stack) == 0:
            return 1 #true
        else:
            return 0 #false
            
    def size_of(self):
        return length(self.stack)       
              
if __name__ == "__main__":
    s = Stack()
    for i in range(0, 10):
        print "Pushing:", i
        s.push(i)

    while not s.is_empty():
        print "Popping:", s.pop()