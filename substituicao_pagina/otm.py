class OTM:
    def execute(self, qtd_quadros, referencias):
        miss = 0
        quadros = []
        
        for indice, ref in enumerate(referencias):
            if ref not in quadros:
                miss += 1 
                if len(quadros) == qtd_quadros:
                    indice_depois = self.__pagina_referenciada_depois(indice, quadros, referencias)
                    quadros.remove(indice_depois)
                quadros.append(ref)
        return miss


    def __pagina_referenciada_depois(self, indice_atual, quadros, referencias):
        tempos = {}
        for q in quadros:
            tempos[q] = 0
            for r in range(indice_atual+1, len(referencias)):
                if referencias[r] != q:
                    tempos[q] += 1
                else:
                    break 
                
        depois = tempos.values()[0]
        chave = tempos.keys()[0] 
        for t in tempos.keys():
            if tempos[t] > depois:
                depois = tempos[t]
                chave = t
        return chave
