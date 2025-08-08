#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

const int MAX_ARTICULOS = 10000;
const int MAX_COMPRAS   = 100;
const int MAX_RUBROS    = 15;

struct Articulo {
    int codArt;
    short codRub;
    string descripcion;
    unsigned short stock;
    float precio;
    string unidadMed;
    short promoTipo; // solo usado para simular un descuento
};

struct IndDescrip {
    string descripcion;
    int posArt;
    bool estado;
};

struct Rubro {
    short codRub;
    string descripcion;
};

struct Compra {
    string descripcion;
    short cantidad;
};

bool LeerArt(ifstream &arch, Articulo &art) {
    if (!(arch >> art.codArt >> art.codRub)) return false;
    arch.ignore();
    getline(arch, art.descripcion, '\t');
    while (!art.descripcion.empty() && art.descripcion.back() == ' ')
        art.descripcion.pop_back();
    arch >> art.stock >> art.precio >> art.unidadMed >> art.promoTipo;
    return true;
}

bool LeerInd(ifstream &arch, IndDescrip &ind) {
    if (!(getline(arch, ind.descripcion, '\t'))) return false;
    int pos, est;
    arch >> pos >> est;
    ind.posArt = pos;
    ind.estado = est == 1;
    arch.ignore();
    return true;
}

bool LeerRub(ifstream &arch, Rubro &rub) {
    if (!(arch >> rub.codRub)) return false;
    arch.ignore();
    getline(arch, rub.descripcion);
    while (!rub.descripcion.empty() && rub.descripcion.back() == ' ')
        rub.descripcion.pop_back();
    return true;
}

bool LeerCompra(ifstream &arch, Compra &comp) {
    if (!(getline(arch, comp.descripcion, '\t'))) return false;
    arch >> comp.cantidad;
    arch.ignore();
    return true;
}

int BuscarEnIndice(IndDescrip ind[], int nInd, const string &desc) {
    for (int i = 0; i < nInd; i++) {
        if (ind[i].descripcion == desc) return i;
    }
    return -1;
}

void ProcCompras(Compra compras[], int nCompras,
                 IndDescrip ind[], int nInd,
                 Articulo arts[], int nArts) {
    for (int i = 0; i < nCompras; i++) {
        int posInd = BuscarEnIndice(ind, nInd, compras[i].descripcion);
        if (posInd != -1 && ind[posInd].estado) {
            Articulo &art = arts[ind[posInd].posArt];
            if (art.stock >= compras[i].cantidad) {
                art.stock -= compras[i].cantidad;
            } else {
                compras[i].cantidad = art.stock;
                art.stock = 0;
            }
        } else {
            compras[i].cantidad = 0;
        }
    }
}

void EmitirTicket(const Compra compras[], int nCompras,
                  const IndDescrip ind[], const Articulo arts[]) {
    cout << "K O T T O\n";
    cout << "Yo te reconozco\n";
    cout << "SUC 170\n";
    cout << "C.U.I.T. 99-99999999-9\n";
    cout << "Fecha: Vie 08/08/2025\n";
    cout << "Hora: 19:30:00\n";
    cout << "Nro. Ticket: 0001-00000001\n";
    cout << "Nro. Caja: 0001\n";
    cout << "----------------------------------------\n";
    cout << "F A C T U R A - B\n";
    cout << "ORIGINAL\n";
    cout << "----------------------------------------\n";

    float subtotal = 0, totalDescuentos = 0;
    for (int i = 0; i < nCompras; i++) {
        int posInd = BuscarEnIndice((IndDescrip*)ind, nCompras, compras[i].descripcion);
        if (posInd != -1 && compras[i].cantidad > 0) {
            const Articulo &art = arts[ind[posInd].posArt];
            float totalItem = compras[i].cantidad * art.precio;
            subtotal += totalItem;

            // Simulamos que promoTipo != 0 implica un descuento del 10%
            float descuento = (art.promoTipo != 0) ? totalItem * 0.10f : 0;
            totalDescuentos += descuento;

            cout << setw(3) << compras[i].cantidad << " x $" << setw(8) << fixed << setprecision(2) << art.precio << "\n";
            cout << left << setw(30) << art.descripcion << " " << art.unidadMed << "\n";
            cout << setw(8) << art.codArt << "   $" << setw(10) << fixed << setprecision(2) << totalItem << "\n";
            if (descuento > 0) {
                cout << left << setw(15) << "Promo" << right << setw(3) << art.promoTipo
                     << "   $" << setw(10) << fixed << setprecision(2) << -descuento << "\n";
            }
            cout << "\n";
        }
    }

    cout << "SubTot. sin descuentos....: $" << setw(10) << fixed << setprecision(2) << subtotal << "\n";
    cout << "Descuentos por promociones: $" << setw(10) << fixed << setprecision(2) << -totalDescuentos << "\n";
    cout << "========================================\n";
    cout << "T O T A L:                  $" << setw(10) << subtotal - totalDescuentos << "\n";
    cout << "========================================\n";
    cout << "Su pago con Efectivo:       $" << setw(10) << subtotal - totalDescuentos << "\n";
    cout << "Su vuelto:                  $      0.00\n";
    cout << "G R A C I A S  P O R  S U  C O M P R A\n";
    cout << "Para consultas, sugerencias o reclamos\n";
    cout << "comunicarse al correo infoKotto.com.ar\n";
}

void EmitirArt_x_Rubro(const Articulo arts[], int nArts, const Rubro rubros[], int nRubros) {
    cout << "\n===== Listado por Rubro =====\n";
    for (int r = 0; r < nRubros; r++) {
        cout << "\nCod. Rubro: " << rubros[r].codRub << " " << rubros[r].descripcion << "\n";
        cout << "-------------------------------------------------\n";
        for (int i = 0; i < nArts; i++) {
            if (arts[i].codRub == rubros[r].codRub) {
                cout << setw(8) << arts[i].codArt << " "
                     << left << setw(25) << arts[i].descripcion
                     << right << setw(4) << arts[i].stock << " "
                     << setw(8) << fixed << setprecision(2) << arts[i].precio
                     << " " << arts[i].unidadMed << "\n";
            }
        }
    }
}

int main() {
    Articulo articulos[MAX_ARTICULOS];
    IndDescrip indices[MAX_ARTICULOS];
    Rubro rubros[MAX_RUBROS];
    Compra compras[MAX_COMPRAS];

    int cantArt = 0, cantInd = 0, cantRub = 0, cantComp = 0;

    ifstream fa("Articulos.txt");
    while (LeerArt(fa, articulos[cantArt])) cantArt++;
    fa.close();

    ifstream fi("IndDescripArt.txt");
    while (LeerInd(fi, indices[cantInd])) cantInd++;
    fi.close();

    ifstream fr("Rubros.txt");
    while (LeerRub(fr, rubros[cantRub])) cantRub++;
    fr.close();

    ifstream fc("ListaCompras.txt");
    while (LeerCompra(fc, compras[cantComp])) cantComp++;
    fc.close();

    ProcCompras(compras, cantComp, indices, cantInd, articulos, cantArt);
    EmitirTicket(compras, cantComp, indices, articulos);
    EmitirArt_x_Rubro(articulos, cantArt, rubros, cantRub);

    return 0;
}
