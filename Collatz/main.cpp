#include <iostream>
using namespace std;

int main() {
	int num;
	do{
		cout << "Entre com o numero maior que 2: ";
		cin >> num;
		if(num <= 1) cout << "Numero menor que 1 entre com outro!!!" << endl;
	}while(num <= 1);	
	
	cout << "Sequencia de numeros: ";
	while(num != 1)
	{
		cout << num << ",";
		if(num%2){
			num = 3*num + 1;
		}else{
			num = num/2;
		}
	}
	cout << num <<"."<<endl;
	
	return 0;
}
