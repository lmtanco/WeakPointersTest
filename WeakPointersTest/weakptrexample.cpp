//
//  main.cpp
//  WeakPointersTest
//
//  Created by Luis lmtanco@uma.es on 3/3/22.
//

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::cout;
using std::endl;

// Ya sé que son mucho más complicadísimas
struct Wall {
    Wall(int n, float a) : num{n}, absorption{a} {}
    int num;
    float absorption;
};

class Room {
public:

    void createSomeWalls() {
        // Se pueden crear punteros así:
        _walls.push_back(make_shared<Wall>(1,0.5)); // parámetros del constructor de Wall
        _walls.push_back(make_shared<Wall>(2,0.3));
        // O así:
        shared_ptr<Wall> sp(new Wall(3,0.4));
        _walls.push_back(sp);
    }
    
    vector<weak_ptr<Wall>> getWalls()
    {
        vector<weak_ptr<Wall>> result;
        for (auto i=0; i<_walls.size(); ++i) {
            result.push_back(weak_ptr<Wall>(_walls[i]));
        }
        return result;
    }
    
    void changeFirstWall() {
        if (_walls.size() > 0) {
            _walls[0]->absorption += 0.1;
        }
    }
    
    void deleteLastWall() {
        if (_walls.size() > 0) {
            _walls.pop_back();
        }
    }
    
private:
    vector<shared_ptr<Wall>> _walls;
};

void CheckAbsorptions(const vector<weak_ptr<Wall>> & punteros) {
    for_each(punteros.cbegin(), punteros.cend(), [](weak_ptr<Wall> wp) { // (*)
        auto p = wp.lock();
        if (p) {
            cout << "Absorption of wall " << p->num << " = " << p->absorption << endl;
        }
        else {
            cout << "Null object" << endl;
        }
    });
    cout << "===" << endl;
}

int main(int argc, const char * argv[]) {
    Room r;
    r.createSomeWalls();
    
    auto punteros = r.getWalls();
    cout << "Lista punteros actualizada: " << endl;
    CheckAbsorptions(punteros);
    
    r.changeFirstWall();
    cout << "Cambiamos una pared: " << endl;
    CheckAbsorptions(punteros);
    
    r.deleteLastWall();
    cout << "Borramos una pared: " << endl;
    CheckAbsorptions(punteros);
    
    r.deleteLastWall();
    cout << "Borramos otra pared: " << endl;
    CheckAbsorptions(punteros);
    
    punteros = r.getWalls(); // Actualizar lista de Walls
    cout << "Lista punteros actualizada: " << endl;
    CheckAbsorptions(punteros);
    
    // Copia de puntero weak también funciona bien, se pueden hacer las que se quieran.
    // Arriba (*) también se hacen copias aunque esté más escondido.
    cout << "Cambiamos una pared: " << endl;
    r.changeFirstWall();
    auto copia = punteros[0]; // copia es un weak_ptr
    {
        auto p = copia.lock();    // p es un smart_ptr
        if (p) {
            cout << "Absorption of wall " << p->num << " = " << p->absorption << endl;
        }
        else {
            cout << "Null object" << endl;
        }
        cout << "===" << endl;
    }
    
    cout << "Borramos la última pared: " << endl;
    r.deleteLastWall();
    {
        auto p = copia.lock();    // p es un smart_ptr
        if (p) {
        cout << "Absorption of wall " << p->num << " = " << p->absorption << endl;
        } else {
            cout << "Null object" << endl;
        }
        cout << "===" << endl;
    }
    return 0;
}
