from random import uniform
from time import sleep
from threading import Thread, Lock

pratos = [0, 0, 0, 0, 0]  


class Filosofo(Thread):
    execute = True  

    def __init__(self, nome, garfo_esquerda, garfo_direita): 
        Thread.__init__(self)
        self.nome = nome
        self.garfo_esquerda = garfo_esquerda
        self.garfo_direita = garfo_direita

    def run(self):
        while self.execute:
            print(f"\n {self.nome} está pensando")
            sleep(uniform(5, 15))
            self.comer()

    def comer(self):
        garfo1, garfo2 = self.garfo_esquerda, self.garfo_direita

        while self.execute: 
            garfo1.acquire(True) 
            locked = garfo2.acquire(False) 
            if locked:
                break
            garfo1.release()
        else:
            return 

        print(f"\n {self.nome} começou a comer")
        sleep(uniform(5, 10))
        print(f"\n {self.nome} parou de comer")
        pratos[nomes.index(self.nome)] += 1  
        print(pratos)
        garfo1.release()  
        garfo2.release() 


nomes = ['Aristóteles', 'Platão', 'Sócrates', 'Pitágoras', 'Demócrito']  
garfos = [Lock() for _ in range(5)]
mesa = [Filosofo(nomes[i], garfos[i % 5], garfos[(i + 1) % 5]) for i in range(5)]
for _ in range(50):
    Filosofo.execute = True 
    for filosofo in mesa:
        try:
            filosofo.start() 
            sleep(2)
        except RuntimeError: 
            pass
    sleep(uniform(5, 15))
    Filosofo.execute = False  