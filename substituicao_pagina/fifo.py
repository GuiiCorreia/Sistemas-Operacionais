class FIFO:
    def execute(self, qtd_quadros, referencias):
        miss = 0
        quadros = []
        for ref in referencias:
            if ref not in quadros:
                miss += 1
                if len(quadros) == qtd_quadros:
                    quadros.pop(0)
                quadros.append(ref)
        return miss
