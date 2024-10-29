#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PLACA_LENGTH 8
#define CODIGO_LENGTH 14

typedef struct {
    char placa[PLACA_LENGTH];
    float peso_max;
    float volume_max;
} Veiculo;

typedef struct {
    char codigo[CODIGO_LENGTH];
    float valor;
    float peso;
    float volume;
    int usado;
} Pacote;

int comparar_valor(const void *a, const void *b) {
    Pacote *pacote_a = (Pacote *)a;

    if (pacote_a->valor >pacote_a->valor) return -1;
    if (pacote_a->valor < pacote_a->valor) return 1;
    return 0;
}

void lerDadosVeiculos(FILE *input, Veiculo veiculos[], int num_veiculos) {
    for (int i = 0; i < num_veiculos; i++) {
        fscanf(input, "%s %f %f \n", veiculos[i].placa, &veiculos[i].peso_max, &veiculos[i].volume_max);
    }
}

void lerDadosPacotes(FILE *input, Pacote *pacotes, int num_pacotes) {
    for (int i = 0; i < num_pacotes; i++) {
        fscanf(input, "%s %f", pacotes[i].codigo, &pacotes[i].valor);
        if (fscanf(input, "%f %f", &pacotes[i].peso, &pacotes[i].volume) != 2) {
            // Se não houver peso e volume informados, definimos como 0
            pacotes[i].peso = 0;
            pacotes[i].volume = 0;
        }
        pacotes[i].usado = 0;
    }
}

void processarEntregas(Veiculo *veiculos, int num_veiculos, Pacote *pacotes, int num_pacotes, FILE *output) {

    //Ordem pois para fazer um algoritmo de mochila temos que ter uma ordem 
    // Nosso caso queremos levar os que foram mais pesados e caros
    qsort(pacotes, num_pacotes, sizeof(Pacote), comparar_valor);
    for (int i = 0; i < num_veiculos; i++) {
        Pacote aux_pac[num_pacotes];
        int pont = 0;
        float peso_total = 0, volume_total = 0, valor_total = 0;
        for (int j = 0; j < num_pacotes; j++) {         
            if (pacotes[j].usado != 1 &&
                pacotes[j].peso + peso_total <= veiculos[i].peso_max &&
                pacotes[j].volume + volume_total <= veiculos[i].volume_max ) {
                volume_total += pacotes[j].volume;
                peso_total += pacotes[j].peso;
                valor_total += pacotes[j].valor;
                pacotes[j].usado = 1;
                aux_pac[pont] = pacotes[j];
                pont += 1;
            }
        }
        fprintf(output, "[%s]R$%0.2f,%0.0fKG(%0.0f%%),%0.0fL(%0.0f%%)\n",veiculos[i].placa,valor_total,peso_total,(peso_total / veiculos[i].peso_max) * 100,volume_total,(volume_total / veiculos[i].volume_max) * 100);
        for(int j = 0; j < pont; j++){
            fprintf(output, "%s\n",aux_pac[j].codigo);
        }
    }
    Pacote aux_pac[num_pacotes];
    int pont = 0;
    float peso_total = 0, volume_total = 0, valor_total = 0;
    for(int j = 0; j < num_pacotes; j++){
        if(pacotes[j].usado == 0){
            volume_total += pacotes[j].volume;
            peso_total += pacotes[j].peso;
            valor_total += pacotes[j].valor;
            aux_pac[pont] = pacotes[j];
            pont += 1;
        }
    } 
    fprintf(output, "[PENDENTE]R$%0.2f,%0.0fKG,%0.0fL\n",valor_total,peso_total,volume_total);
    for(int j = 0; j < pont; j++){
            fprintf(output, "%s\n",aux_pac[j].codigo);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s arquivo_entrada arquivo_saida\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    

    int num_veiculos, num_pacotes;
    
    fscanf(input, "%d", &num_veiculos);
    Veiculo veiculos[num_veiculos];
    lerDadosVeiculos(input, veiculos, num_veiculos);
    fscanf(input, "%d", &num_pacotes);
    Pacote pacotes[num_pacotes];
    lerDadosPacotes(input, pacotes, num_pacotes);


    FILE *output = fopen(argv[2], "w");
    if (output == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
        fclose(input);
        return 1;
    }
    
    processarEntregas(veiculos,num_veiculos,pacotes,num_pacotes,output);

    fclose(input);
    fclose(output);
    return 0;
}
