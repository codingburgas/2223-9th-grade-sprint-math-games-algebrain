#include <iostream>
using namespace std;

struct entity {
	int x = 5;
};


int main() {

	entity *e = new entity();

	cout << e->x;


}