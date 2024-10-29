/***
  Disciplina: Sistemas Operacionais - 2023 - 1
  Faculdade: FACET, UFGD
  Docente: Marcos Paulo Moro
  Discente: Maykoll Rocha
  Jantar dos Filósofos com DeadLock
***/

#include <iostream>
#include <vector>
#include <windows.h>
#include <locale.h>
#include <string>

using namespace std;
//-----------------------------Variáveis---------------------------------------------------------

//Lista com nome dos filosofos na tela
string n_filos[] = { "Tales de Mileto", "Pitão", "Heráclito","Parmênides","Sócrates","Platão","Aristóteles","Santo Agostinho","São Tomás de Aquino" };

//Crontrola a retirada de garfos
HANDLE CrtGarfos;
//Contrtola os prints
HANDLE prints;
//Incia as variaies de Threads
int num_filosofos;

//Strutura dos Garfos
typedef struct {
    bool em_uso;
}Garfo;

//Vetor dos Garfos
vector<Garfo> garfos;

//Estrutura dos filosofos
typedef struct {
    int id;
}Filosofo;

//---------- Proptotipação das Funções ---------
//Função que dado o ID do filosofo retorna seu nome de sendo 9 nomes dispponíveis mais que isso retorna o proprio numero
string filoname(int id);

//Ação de pensar ou comer do filosofo ela é a função da nossa thread.
void acao_filosofos(void* param);

int main()
{
    setlocale(LC_ALL, "pt_BR");

    num_filosofos = 5;//Numero de threads que sera o numero de filosofos
    //Enche o Gafos FAlSE é que não esta sendo usado TRUE é que está sendo usado.
    for (int i = 0; i < num_filosofos; i++) {
        Garfo aux;
        aux.em_uso = FALSE;
        garfos.push_back(aux);
    }

    vector<HANDLE> AcoesFilosofos;// Armazenas as therds
    vector<Filosofo> vfilosofos;// Armazena os parametros de cada thread
    Filosofo addfilosofos;//Pega temporariamente os filosofos funcina como auxiliar

    CrtGarfos = CreateMutex(NULL, FALSE, NULL);
    prints = CreateMutex(NULL, FALSE, NULL);

    // Encher os parametros com o indice da thread e tbem para contar quantas threads ele pegou
    for (int i = 0; i < num_filosofos; i++)
    {
        addfilosofos.id = i;
        vfilosofos.push_back(addfilosofos);
    }


    for (int i = 0; i < num_filosofos; i++)
        AcoesFilosofos.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&acao_filosofos, &vfilosofos[i], 0, NULL));


    WaitForMultipleObjects(num_filosofos, AcoesFilosofos.data(), TRUE, INFINITE);


    for (int i = 0; i < num_filosofos; i++) {
        CloseHandle(AcoesFilosofos[i]);
    }

    (CrtGarfos != 0) ? CloseHandle(CrtGarfos) : 1;
    (prints != 0) ? CloseHandle(prints) : 1;

    return 1;
}

string filoname(int id) {
    return (id < 9) ? n_filos[id] : to_string(id);
}

void acao_filosofos(void* param)
{
    // Reber os parametros externos
    Filosofo* filo = (Filosofo*)param;

    while (TRUE)
    {
        //-----Pensando--------
        WaitForSingleObject(prints, INFINITE);
        cout << "Filoso " << filoname(filo->id) << " pensando." << endl;
        ReleaseMutex(prints);
        Sleep(rand() % 1000);
        //-----Parou de pensar----

        //---Tentar pegar os garfos---
        //Tenta pegar o garfo da esquerda
        WaitForSingleObject(CrtGarfos, INFINITE);
        while (garfos[filo->id].em_uso) {
            cout << "Filoso " << filoname(filo->id) << " esperando garfo esquerdo." << endl;
            Sleep(1000);
        }
        garfos[filo->id].em_uso = TRUE;
        cout << "Filoso " << filoname(filo->id) << " pegou garfo esquedo." << endl;
        ReleaseMutex(CrtGarfos);

        //Tenta pegar o garfo da direita
        WaitForSingleObject(CrtGarfos, INFINITE);

        while (garfos[(filo->id + 1) % num_filosofos].em_uso) {
            cout << "Filoso " << filoname(filo->id) << " esperando garfo direito." << endl;
            Sleep(1000);
        }
        garfos[(filo->id + 1) % num_filosofos].em_uso = TRUE;
        cout << "Filoso " << filoname(filo->id) << " pegou garfo direito." << endl;
        ReleaseMutex(CrtGarfos);
        //-----Conseguiu pegar os garfos----

        //-----Comendo----
        WaitForSingleObject(prints, INFINITE);
        cout << "Filosofo " << filoname(filo->id) << " esta comendo." << endl;
        ReleaseMutex(prints);
        Sleep(rand() % 1000);
        //----Terminou de comer-------

        //----Devolver os garfos------
        garfos[filo->id].em_uso = FALSE;
        garfos[(filo->id + 1) % num_filosofos].em_uso = FALSE;
        WaitForSingleObject(prints, INFINITE);
        cout << "Filosofo " << filoname(filo->id) << " terminou de comer e devolveu os garfos." << endl;
        ReleaseMutex(prints);
        //----Parou de comer e voltou a pensar lá em cima----
    }

    return;
}