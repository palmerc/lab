import hotshot.stats
import sys

def examine(fn):
    s = hotshot.stats.load(fn)
    s.strip_dirs()
    s.sort_stats('time', 'calls')
    s.print_stats(20)

if __name__ == "__main__":
    examine(sys.argv[1])

