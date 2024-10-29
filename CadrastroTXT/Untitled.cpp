#include <iostream>
#include <vector>
#include <windows.h>
#include <locale.h>
#include <string>

using namespace std;

string n_filos[] = { "Tales de Mileto", "Pitão", "Heráclito", "Parmênides", "Sócrates", "Platão", "Aristóteles", "Santo Agostinho", "São Tomás de Aquino" };

int cont; // Para criar um loop finito

HANDLE esperas;
HANDLE contador;

// Iniciar as variáveis de Threads
int num_threads;

typedef struct {
    bool em_uso;
} Garfo;

vector<Garfo> garfos;

typedef struct {
    int id;
} Filosofo;

string filoname(int id) {
    if (id < 9) {
        return n_filos[id];
    }
    else {
        return to_string(id);
    }
}

void acao_filosofos(void* param) {
    Filosofo* filo = (Filosofo*)param;

    while (cont < 20) {
        // Dormindo
        cout << "Filosofo " << filoname(filo->id) << " dormindo." << endl;
        Sleep(rand() % 1000);

        // Acordou
        cout << "Filosofo " << filoname(filo->id) << " acordou." << endl;
        // Pegar Garfos
        WaitForSingleObject(esperas, INFINITE);

        while (garfos[filo->id].em_uso || garfos[(filo->id + 1) % num_threads].em_uso) {
            cout << "Filoso " << filoname(filo->id) << " esperando garfo" << endl;
            Sleep(1000);
        }

        garfos[filo->id].em_uso = TRUE;
        garfos[(filo->id + 1) % num_threads].em_uso = TRUE;
        cout << "Filoso " << filoname(filo->id) << " pegou garfos." << endl;

        ReleaseSemaphore(esperas, 1, NULL);

        // Comendo
        cout << "Filosofo " << filoname(filo->id) << " está comendo." << endl;
        Sleep(rand() % 100);

        // Devolver os garfos
        garfos[filo->id].em_uso = FALSE;
        garfos[(filo->id + 1) % num_threads].em_uso = FALSE;
        cout << "Filosofo " << filoname(filo->id) << " parou de comer e devolveu os garfos." << endl;

        // Pensando
        cout << "Filoso " << filoname(filo->id) << " pensando." << endl;
        Sleep(rand() % 1000);

        ReleaseSemaphore(esperas, 1, NULL);
        
    }

    return;
}

int main() {
    setlocale(LC_ALL, "pt_BR");
    num_threads = 6;
    cont = 0;

    for (int i = 0; i < num_threads; i++) {
        Garfo aux;
        aux.em_uso = FALSE;
        garfos.push_back(aux);
    }

    vector<HANDLE> hThread;
    vector<Filosofo> vParam;
    Filosofo addve;

    esperas = CreateSemaphore(NULL, num_threads, num_threads, NULL);
    contador = CreateMutex(NULL, FALSE, NULL);

    for (int i = 0; i < num_threads; i++) {
        addve.id = i;
        vParam.push_back(addve);
    }

    for (int i = 0; i < num_threads; i++)
        hThread.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&acao_filosofos, &vParam[i], 0, NULL));

    WaitForMultipleObjects(num_threads, hThread.data(), TRUE, INFINITE);

    for (int i = 0; i < num_threads; i++) {
        CloseHandle(hThread[i]);
    }

    CloseHandle(esperas);
    CloseHandle(contador);
    cout << "Acabou o Jantar" << endl;
    return 1;
}
