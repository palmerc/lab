"""

Another product of laziness

"""

class picklecache:
    import cPickle
    def __init__(self, fn):
        self.fn = "cache/" + fn
    def serve(self):
        f = file(self.fn, "rb")
        return self.cPickle.load(f)
    def save(self, stuff):
        f = file(self.fn, "wb")
        self.cPickle.dump(stuff, f)

def testfunc():
    import time
    cache = picklecache("blah1")
    try:
        data = cache.serve()
    except:
        time.sleep(5)
        data = "blah blah blah blah blah blah"
        cache.save(data)
    return data

if __name__=="__main__":
    print testfunc()
    print testfunc()
    print testfunc()
