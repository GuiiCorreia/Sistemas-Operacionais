#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

FILE *pont_arq;


int NLEI, NESCR;
int nlida, nescrita;
int dig;

int var = 8;
int l_cont = 0;
int e_cont = 0;
int l_esp = 0;
int e_esp = 0;

pthread_cond_t pode_escr;
pthread_cond_t podeler;
pthread_mutex_t mutex;

int num_Digitos(int i){
	int contaDigitos = 0, valor;

	valor = i;
	if (valor == 0) contaDigitos = 1;
	else
	while (valor != 0)
	{
	   contaDigitos = contaDigitos + 1;
	   valor = valor / 10;
	}
  return contaDigitos;
}

int comeca_leitura(int i) {

	int leitura;
	pthread_mutex_lock(&mutex);
	nlida++;
	if (e_cont == 1 || e_esp > 0) {
		fprintf(pont_arq, "\nl_entrou_fila(%d,%d)", i,var);
		l_esp++;
		while (1) {
			pthread_cond_wait(&podeler, &mutex);
			fprintf(pont_arq, "\nl_recebeu_sinal(%d,%d)", i,var);
			if (e_cont == 0) break;
			fprintf(pont_arq, "\nl_continuou_fila(%d,%d)", i,var);
		}

		fprintf(pont_arq, "\nl_saiu_fila(%d,%d)", i,var);
		l_esp--;

	}

	printf("\nLeitora %d está lendo a variável %d", i,var);

	fprintf(arq, "\n",var);
	l_cont++;
	fprintf(pont_arq, "\nleitora_leu(%d,%d)", i,var);
	leitura = var;
	pthread_mutex_unlock(&mutex);
	return leitura;

};

void termina_leitura(int i)
{

	pthread_mutex_lock(&mutex);
	fprintf(pont_arq, "\ntermina_leitura(%d,%d)", i,var);
	if (--l_cont == 0){
		fprintf(pont_arq, "\nl_manda_sinal_l(%d,%d)", i,var);
		pthread_cond_signal(&pode_escr);
	}

	pthread_mutex_unlock(&mutex);
};

void comeca_escrita(int i)
{
	pthread_mutex_lock(&mutex);
	nescrita++;
	if (e_cont == 1 || l_cont > 0) {
		++e_esp;
		fprintf(pont_arq, "\ne_entrou_fila(%d,%d)", i,var);
		while(1){
			pthread_cond_wait(&pode_escr, &mutex);
			fprintf(pont_arq, "\ne_recebeu_sinal(%d,%d)", i,var);
			if (e_cont == 0 && l_cont == 0) break;
			fprintf(pont_arq, "\ne_continuou_fila(%d,%d)", i,var);

		}
		--e_esp;
		fprintf(pont_arq, "\ne_saiu_fila(%d,%d)", i,var);
	}
	e_cont = 1;
	fprintf(pont_arq, "\nescritora_escreveu(%d,%d)", i,var);
	printf("\nEscritora tá escrevendo a variável %d", i);
	var = i;
	pthread_mutex_unlock(&mutex);
};

void para_escrita(int i)
{
	pthread_mutex_lock(&mutex);
	e_cont = 0;
	fprintf(pont_arq, "\ntermina_escrita(%d,%d)", i,var);
	if (l_esp > 0){
		pthread_cond_broadcast(&podeler);
		fprintf(pont_arq, "\ne_manda_sinal_l(%d,%d)", i,var);
	}

	else{
		pthread_cond_signal(&pode_escr);
		fprintf(pont_arq, "\ne_manda_sinal_e(%d,%d)", i,var);
	}
	pthread_mutex_unlock(&mutex);
};

void* leitor(void* id_le)
{
	int j =0;
	int a = 0;
	int i = *(int*)id_le;
	char nome[dig+4];
	int leit[NLEI];

	printf("\nLeitora %d criada", i);
	fprintf(pont_arq, "\nlei_criada(%d,%d)", i,var);

	while (a<NLEI) {
		usleep(1);
		leit[j]=comeca_leitura(i);
		termina_leitura(i);
		a++;
		j++;
	}

	FILE *arq;
	sprintf(nome, "%i", i);
	strcat(nome, ".txt");
	arq = fopen(nome,"wt");

	for(j=0;j<NLEI;j++){
		fprintf(arq, "\n%d",leit[j]);
	}

	fprintf(pont_arq, "\nlei_encerrada(%d,%d)", i,var);
  pthread_exit(NULL);
};

void* escritor(void* id_esc)
{
	int c = 0;
	int i = *(int*)id_esc;

	printf("\nEscritora %d criada", i);
	fprintf(pont_arq, "\nesc_criada(%d,%d)", i,var);

	while (c<NESCR) {
		usleep(1);
		comeca_escrita(i);
		para_escrita(i);
		c++;
	}

	fprintf(pont_arq, "\nesc_encerrada(%d,%d)", i,var);
  pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	if(argc < 6) {
    printf("Use: %s <nº de threads leitoras> <nº de threads escritoras> <nº de leituras de cada thread> <nº de escrita de cada thread>  <nome do arquivo de saída>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
	int NTHREADSL, NTHREADSE;
	int num;

	NTHREADSL = atoi(argv[1]);
	NTHREADSE = atoi(argv[2]);
	NLEI = atoi(argv[3]);
	NESCR = atoi(argv[4]);

	dig = num_Digitos(NTHREADSL);

	pont_arq = fopen(argv[5], "wt");

	if (pont_arq == NULL)
	{
	   printf("Problemas na CRIACAO do arquivo\n");
	   return 0;
	}
	fprintf(pont_arq, "\nnum_thread_tipo (%d,%d)", NTHREADSL,NTHREADSE);
	fprintf(pont_arq, "\nnum_exec_thread (%d,%d)", NLEI, NESCR);
	pthread_t th_le[NTHREADSL];
	int id_le[NTHREADSL];
	pthread_t th_esc[NTHREADSE];
	int id_esc[NTHREADSE];
	int l = 0, e = 0;

	pthread_cond_init(&podeler, NULL);
	pthread_cond_init(&pode_escr, NULL);
	pthread_mutex_init(&mutex, NULL);


	if (NTHREADSL >= NTHREADSE) num = NTHREADSL;
	else num = NTHREADSE;

	for (int i = 0; i < num; i++) {
		if(i<NTHREADSE){
			id_esc[e] = e;
			e++;
			pthread_create(&th_esc[i], NULL, &escritor, &id_esc[i]);

		}
		if(i<NTHREADSL){
			id_le[l] = l;
			l++;
			pthread_create(&th_le[i], NULL, &leitor, &id_le[i]);

		}
	}

	for (int i = 0; i < NTHREADSL; i++) {
		pthread_join(th_le[i], NULL);;
	}

	for (int i = 0; i < NTHREADSE; i++) {
		pthread_join(th_esc[i], NULL);
	}


}