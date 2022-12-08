class LRU:
    def execute(self, qtd_quadros, referencias):
        miss = 0
        instante = 0
        quadros = {}
        for ref in referencias:
            if ref not in quadros.keys():
                miss += 1
                if len(quadros) == qtd_quadros:
                    mais_antiga = self.__pagina_mais_antiga(quadros)
                    del quadros[mais_antiga]
            quadros[ref] = instante
            instante += 1

        return miss


    def __pagina_mais_antiga(self, quadros):
        mais_antiga = quadros.keys()[0]
        menor_instante = quadros.values()[0]
        for chave in quadros.keys():
            if quadros[chave] < menor_instante:
                mais_antiga = chave
                menor_instante = quadros[chave]
        return mais_antiga
