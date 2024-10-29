// JantarDosFilosofos.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <vector>
#include <windows.h>
#include <locale.h>
#include <string>

using namespace std;
string n_filos[] = { "Tales de Mileto", "Pitão", "Heráclito","Parmênides","Sócrates","Platão","Aristóteles","Santo Agostinho","São Tomás de Aquino"};
int cont; // Para crirar um loop finito

HANDLE CrtGarfos;
HANDLE prints;
//Incia as variaies de Threads
int num_threads;

typedef struct {
    bool em_uso;
    int id;
}garfo;

vector<garfo> garfos;

typedef struct {
    int id;
}filosofo;

string filoname(int id) {
    if (id < 9) {
        return n_filos[id];
    }
    else {
        return to_string(id);
    }
}
void acao_filosofos(void* param)
{
    // Reber os parametros externos
    filosofo* filo = (filosofo*)param;

    while (cont < 20)
    {
        //Dormindo
        WaitForSingleObject(prints, INFINITE);
        cout << "Filosofo " << filoname(filo->id) << " dormindo." << endl;
        ReleaseMutex(prints);
        
        Sleep(rand() % 1000);

        //Adordou
        WaitForSingleObject(prints, INFINITE);
        cout << "Filosofo " << filoname(filo->id) << " acordou." << endl;
        ReleaseMutex(prints);


        //Pegar o garfo esquerdo
        WaitForSingleObject(CrtGarfos, INFINITE);
        while (garfos[filo->id].em_uso) {
            cout << "Filoso " << filoname(filo->id) << " esperando garfo esquerdo." << endl;
            Sleep(1000);
        }
        garfos[filo->id].em_uso = TRUE;
        
        WaitForSingleObject(prints, INFINITE);
        cout << "Filoso " << filoname(filo->id) << " pegou garfo esquedo." << endl;
        ReleaseMutex(prints);

        ReleaseMutex(CrtGarfos);

        //Pegar o garfo diretio
        WaitForSingleObject(CrtGarfos, INFINITE);
        
        while (garfos[(filo->id + 1) % num_threads].em_uso) {
            cout << "Filoso " << filoname(filo->id) << " esperando garfo esquerdo." << endl;
            Sleep(1000);
        }
        garfos[(filo->id + 1) % num_threads].em_uso = TRUE;

        WaitForSingleObject(prints, INFINITE);
        cout << "Filoso " << filoname(filo->id) << " pegou garfo direito." << endl;
        ReleaseMutex(prints);

        ReleaseMutex(CrtGarfos);

        //Comendo
        WaitForSingleObject(prints, INFINITE);
        cout << "Filosofo " << filoname(filo->id) << " esta comendo." << endl;
        ReleaseMutex(prints);
        Sleep(rand() % 1000);

        //Devolver os garfos
        garfos[filo->id].em_uso = FALSE;
        garfos[(filo->id + 1) % num_threads].em_uso = FALSE;
        WaitForSingleObject(prints, INFINITE);
        cout << "Filosofo " << filoname(filo->id) << " parou de comer e devolveu os garfos." << endl;
        ReleaseMutex(prints);

        WaitForSingleObject(prints, INFINITE);
        cout << "Filoso " << filoname(filo->id) << " pensando." << endl;
        ReleaseMutex(prints);
        Sleep(rand() % 1000);

        WaitForSingleObject(CrtGarfos, INFINITE);
        cont++;
        ReleaseMutex(CrtGarfos);
    }

    return;
}


int main()
{
    setlocale(LC_ALL, "pt_BR");
    num_threads = 5;
    cont = 0;
    for (int i = 0; i < num_threads; i++) {
        garfo aux;
        aux.em_uso = FALSE;
        aux.id = i;
        garfos.push_back(aux);
    }

    vector<HANDLE> hThread;// Armazenas as therds
    vector<filosofo> vParam;// Armazena os parametros de cada thread
    filosofo addve;
    CrtGarfos = CreateMutex(NULL, FALSE, NULL);
    prints = CreateMutex(NULL, FALSE, NULL);
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
    CloseHandle(prints);

    return 1;
}