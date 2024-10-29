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


string n_filos[] = { "Tales de Mileto", "Pitão", "Heráclito","Parmênides","Sócrates","Platão","Aristóteles","Santo Agostinho","São Tomás de Aquino" };

//Crontrola a retirada de garfos
HANDLE CrtGarfos;
//Incia as variaies de Threads
int num_threads;

typedef struct {
    bool em_uso;
}garfo;

vector<garfo> garfos;

typedef struct {
    int id;
}filosofo;

string filoname(int id) {
    return (id < 9) ?n_filos[id]:to_string(id);
}

void acao_filosofos(void* param)
{
    // Reber os parametros externos
    filosofo* filo = (filosofo*)param;

    while (TRUE)
    {
        //Pensando
        cout << "Filoso " << filoname(filo->id) << " pensando." << endl;
        Sleep(rand() % 1000);


        //Pegar o garfo esquerdo
        WaitForSingleObject(CrtGarfos, INFINITE);
        while (garfos[filo->id].em_uso) {
            cout << "Filoso " << filoname(filo->id) << " esperando garfo esquerdo." << endl;
            Sleep(1000);
        }
        garfos[filo->id].em_uso = TRUE;
        cout << "Filoso " << filoname(filo->id) << " pegou garfo esquedo." << endl;
        ReleaseMutex(CrtGarfos);

        //Pegar o garfo diretio
        WaitForSingleObject(CrtGarfos, INFINITE);

        while (garfos[(filo->id + 1) % num_threads].em_uso) {
            cout << "Filoso " << filoname(filo->id) << " esperando garfo direito." << endl;
            Sleep(1000);
        }
        garfos[(filo->id + 1) % num_threads].em_uso = TRUE;
        cout << "Filoso " << filoname(filo->id) << " pegou garfo direito." << endl;

        ReleaseMutex(CrtGarfos);

      
        cout << "Filosofo " << filoname(filo->id) << " esta comendo." << endl;
        Sleep(rand() % 1000);

        //Devolver os garfos
        garfos[filo->id].em_uso = FALSE;
        garfos[(filo->id + 1) % num_threads].em_uso = FALSE;
        cout << "Filosofo " << filoname(filo->id) << " parou de comer e devolveu os garfos." << endl;     
    }

    return;
}


int main()
{
    setlocale(LC_ALL, "pt_BR");
    num_threads = 5;
    for (int i = 0; i < num_threads; i++) {
        garfo aux;
        aux.em_uso = FALSE;
        garfos.push_back(aux);
    }

    vector<HANDLE> hThread;// Armazenas as therds
    vector<filosofo> vParam;// Armazena os parametros de cada thread
    filosofo addve;
    CrtGarfos = CreateMutex(NULL, FALSE, NULL);
    // Encher os parametros com o indice da thread e tbem para contar quantas threads ele pegou
    for (int i = 0; i < num_threads; i++)
    {
        addve.id = i;
        vParam.push_back(addve);
    }


    for (int i = 0; i < num_threads; i++)
        hThread.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&acao_filosofos, &vParam[i], 0, NULL));


    WaitForMultipleObjects(num_threads, hThread.data(), TRUE, INFINITE);


    for (int i = 0; i < num_threads; i++) {
        CloseHandle(hThread[i]);
    }

    CloseHandle(CrtGarfos);

    return 1;
}