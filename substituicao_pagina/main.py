import sys
from fifo import *
from otm import *
from lru import *

def main():
    qtd_quadros = int(sys.stdin.readline())
    referencias = map(int, sys.stdin.readlines())
    
    fifo = FIFO()
    miss_fifo = fifo.execute(qtd_quadros, referencias)
    
    otm = OTM()
    miss_otm = otm.execute(qtd_quadros, referencias)
    
    lru = LRU()
    miss_lru = lru.execute(qtd_quadros, referencias)
    
    saida = 'FIFO {0}\nOTM {1}\nLRU {2}'
    print(saida.format(miss_fifo, miss_otm, miss_lru))


if __name__ == "__main__":
    main()