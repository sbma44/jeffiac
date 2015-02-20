import __future__
import sys
from subprocess import Popen, PIPE, STDOUT

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        for (i, line) in enumerate(f.readlines()):
            p = Popen(['say', '-v', 'Fred', '-o', 'wav/{}.aiff'.format(str(i+1))], stdout=PIPE, stdin=PIPE, stderr=STDOUT)    
            grep_stdout = p.communicate(input=bytes(line, 'utf-8'))[0]
