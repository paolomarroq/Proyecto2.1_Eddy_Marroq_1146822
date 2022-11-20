#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "List.h"
#include "LinkedList.h"
#include "Sticker.h"
#include "Category.h"
#include "AlbumReader.h"
#include "windows.h"
#include <stdlib.h>
#include "pch.h"

using namespace std;
using namespace System;

AlbumReader albumReader;
string sourceFile;
List<Sticker>* albumlist;
vector<string> codes_list;

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void MarkedScreen();
void FilterScreen(string tipeFilter, string repetidasOpt);
void MainScreen();
int StickerStock(string input);

void menuBusqueda();

void ReadFile(string file_location)
{
    fstream file(file_location, ios::in);
    string line, word;
    codes_list.clear();
    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream str(line);

            while (getline(str, word, ','))
            {
                codes_list.push_back(word);
            }
        }
    }
}

int StickerStock(string input)
{

    for (int i = 0; i < albumlist->get_size(); i++)
    {
        if (albumlist->get(i)->get_code().compare(input) == 0)
        {
            return i;
        }
    }
    return -1;
}

int StickerStockByName(string input)
{

    for (int i = 0; i < albumlist->get_size(); i++)
    {
        if (albumlist->get(i)->get_name().compare(input) == 0)
        {
            return i;
        }
    }
    return -1;
}

void AddCodeList()
{
    int marking;
    for (int i = 0; i < codes_list.size(); i++)
    {
        marking = StickerStock(codes_list[i]);
        if (marking >= 0)
        {
            albumlist->get(marking)->mark();
        }
    }
}

void DeleteCodeList()
{
    int marking;
    for (int i = 0; i < codes_list.size(); i++)
    {
        marking = StickerStock(codes_list[i]);
        if (marking >= 0)
        {
            albumlist->get(marking)->unmark();
        }
    }
}



int MissingCount()
{
    int missing = 0;
    for (int i = 0; i < albumlist->get_size(); i++)
    {
        if (!albumlist->get(i)->is_marked())
        {
            missing++;
        }
    }
    return missing;
}

int CompleteCount()
{
    int marked = 0;
    for (int i = 0; i < albumlist->get_size(); i++)
    {
        if (albumlist->get(i)->is_marked())
        {
            marked++;
        }
    }
    return marked;
}

void MarkedScreen()
{
    string selection;
    string input;
    int select;
    int index;

    do
    {
        system("cls");
        cout << "MARCADADO & DESAMARCADO\n";
        cout << "Ingrese el Numero de Opcion:\n";
        cout << "1. Marcar estampas individualmente\n";
        cout << "2. Desmarcar estampas individualmente\n";
        cout << "3. Marcar estampas por archivo\n";
        cout << "4. Desmarcar estampas por archivo\n";
        cout << "0. Regresar Menu Principal\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            cout << "Ingrese el codigo unico de la estampa a marcar\n";
            getline(cin, input);

            index = StickerStock(input);
            if (index >= 0)
            {
                albumlist->get(index)->mark();
                cout << "Marcado existosamente\n";
            }
            else
            {
                cout << "No existe un sticker con ese codigo\n";
            }

            cout << "Presione cualquier tecla para continuar...\n";
            getwchar();

            break;
        case 2:
            cout << "Ingrese el codigo unico de la estampa a desmarcar\n";
            getline(cin, input);

            index = StickerStock(input);
            if (index >= 0)
            {
                albumlist->get(index)->unmark();
                cout << "Desmarcado existosamente\n";
            }
            else
            {
                cout << "No existe un sticker con ese codigo\n";
            }

            cout << "Presione cualquier tecla para continuar...\n";
            getwchar();
            break;
        case 3:
            cout << "Ingrese la direccion del archivo con las estampas a desmarcar\n";
            cout << "EL archivo debe de contener los codigos de las estamas en el siguiente formato:\n";
            cout << "Primeras tres letras del Pais (BRA = Brasil) o FWC para estampas de Museo, Numero de Estampa por Pais (ARG1) y separados por comas en una sola linea.\n";

            getline(cin, input);
            ReadFile(input);
            AddCodeList();

            cout << "Los codigos ingresados han sido marcados\n";
            cout << "Presione cualquier tecla para continuar...\n";
            getwchar();
            break;
        case 4:
            cout << "Ingrese la direccion del archivo con las estampas a desmarcar\n";
            cout << "EL archivo debe de contener los codigos de las estamas en el siguiente formato:\n";
            cout << "Primeras tres letras del Pais (BRA = Brasil) o FWC para estampas de Museo, Numero de Estampa por Pais (ARG1) y separados por comas en una sola linea.\n";

            getline(cin, input);
            ReadFile(input);
            DeleteCodeList();

            cout << "Los codigos ingresados se han desmarcado correctamente\n";
            cout << "Presione cualquier tecla para continuar...\n";
            getwchar();
            break;
        case 0:
            return;
        default:
            break;
        }
    } while (select != 0);
}

string getLineWrite(Sticker* looking)
{
    string code = looking->get_code();
    string marcada = "Complete";
    if (looking->is_marked())
    {
        marcada = "Repeated";
    }
    int apariciones = looking->get_appeareances() - 1;
    if (apariciones < 0)
    {
        apariciones = 0;
    }
    string result = code + "," + looking->get_name() + "," + marcada + "," + to_string(apariciones) + "\n";
    return result;
}

void saveFile(string direccionArchivo, string toSave)
{
    ofstream archivo;
    archivo.open(direccionArchivo);
    archivo << toSave;
    archivo.close();
}

string Filter(string tipeFilter, string repetidasOpt)
{
    string result = "Codigo,Nombre,Marcada,Repeticiones\n";
    if (tipeFilter.compare("Ninguno") != 0 && repetidasOpt.compare("Repeated") == 0)
    {
        for (int i = 0; i < albumlist->get_size(); i++)
        {
            Sticker* looking = albumlist->get(i);
            if (looking->get_category()->get_code().compare(tipeFilter) == 0 && looking->is_repeated())
            {
                result += getLineWrite(looking);
            }
        }

    }
    else if (tipeFilter.compare("Ninguno") != 0)
    {
        for (int i = 0; i < albumlist->get_size(); i++)
        {
            Sticker* looking = albumlist->get(i);
            if (looking->get_category()->get_code().compare(tipeFilter) == 0)
            {
                result += getLineWrite(looking);
            }
        }
    }
    else if (repetidasOpt.compare("Repeated") == 0)
    {
        for (int i = 0; i < albumlist->get_size(); i++)
        {
            Sticker* looking = albumlist->get(i);
            if (looking->is_repeated())
            {
                result += getLineWrite(looking);
            }
        }
    }
    return result;
}

void FilterScreen(string tipeFilter, string repetidasOpt)
{
    string filter = tipeFilter;
    string reps = repetidasOpt;
    string selection, input, archv;
    int select;
    string searchResult;

    do
    {

        system("cls");
        cout << "FILTRADO\n";
        cout << "Ingrese el numero de la opcion:\n";
        cout << "1. Filtrar por tipo/seleccion: " << filter << "\n";
        cout << "2. Filtrar por repetidas: " << reps << "\n";
        cout << "3. Ejecutar filtrado\n";
        cout << "0. Regresar a Menu Principal\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            cout << "Ingrese el codigo de la categoria que desea filtrar\n";
            getline(cin, filter);
            break;
        case 2:
            if (reps.compare("Completed") == 0)
            {
                reps = "Complete";
            }
            else if (reps.compare("Repeated") == 0)
            {
                reps = "Repeated";
            }
            break;
        case 3:
            searchResult = Filter(filter, reps);
            cout << searchResult;
            cout << "Presione cualquer tecla para continuar...\n";
            getwchar();

            cout << "Desea guardar el resultado en sus Archivos.\n";
            cout << "1. Si\n";
            cout << "0. No\n";

            getline(cin, input);

            if (input.compare("1") == 0)
            {
                cout << "Ingrese la ubicacion del archivo:\n";
                getline(cin, archv);
                saveFile(archv, searchResult);

                cout << "El archivo CSV ha sido exportado";
                cout << "Presione cualquier tecla para continuar...\n";
                getwchar();
            }

            break;
        case 0:
            MainScreen();
            return;
        default:
            break;
        }
    } while (select != 0);
}

void menuBusqueda()
{
    string selection, input, result;
    int select, index;

    do
    {
        system("cls");
        cout << "Menu de busqueda\n";
        cout << "Seleccione una opcion\n";
        cout << "1| Buscar estampa por codigo unico\n";
        cout << "2| Buscar estampa por nombre\n";
        cout << "0| Regresar\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            cout << "Ingrese el codigo unico de la estampa a buscar\n";
            getline(cin, input);
            index = StickerStock(input);
            if (index >= 0)
            {
                result = "Codigo,Nombre,Marcada,Repeticiones\n";
                result += getLineWrite(albumlist->get(index));
                cout << result;
            }
            else
            {
                cout << "No existe un sticker con ese codigo en el album\n";
            }

            cout << "Presione una tecla para continuar\n";
            getwchar();

            break;
        case 2:
            cout << "Ingrese el nombre de la estampa a buscar\n";
            getline(cin, input);
            index = StickerStockByName(input);
            if (index >= 0)
            {
                result = "Codigo,Nombre,Marcada,Repeticiones\n";
                result += getLineWrite(albumlist->get(index));
                cout << result;
            }
            else
            {
                cout << "No existe un sticker con ese codigo en el album\n";
            }
            cout << "Presione una tecla para continuar\n";
            getwchar();
            break;

        case 0:
            MainScreen();
            return;
        default:
            break;
        }
    } while (select != 0);
}

void MainScreen()
{
    string selection;
    int select;
    int count;

    do
    {
        system("cls");
        cout << "Album FIFA Qatar 2022\n";
        cout << "Ingrese una opcion para comenzar:\n";
        cout << "1. Marcar o Desmarcar estampas\n";
        cout << "2. Mostrar cuantas estampas faltantes\n";
        cout << "3. Mostrar cuantas estampas marcadas\n";
        cout << "4. Filtrar estampas\n";
        cout << "5. Buscar una estampa\n";
        cout << "0. Salir del programa\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            MarkedScreen();
            break;
        case 2:

            count = MissingCount();
            if (count > 1)
            {
                cout << "Faltan " << count << " estampas\n";
            }
            else if (count == 1)
            {
                cout << "Faltan " << count << " estampa\n";
            }
            else
            {
                cout << "¡Feliciades! Has completado tu Album.\n";
            }

            cout << "Presione cualquier tecla para continuar...\n";
            getwchar();
            break;
        case 3:
            count = CompleteCount();

            if (count > 1)
            {
                cout << "Hay " << count << " estampas marcadas\n";
            }
            else if (count == 1)
            {
                cout << "Hay " << count << " estampa marcada\n";
            }
            else
            {
                cout << "Aun no posees estampas en tu album.\n";
            }

            cout << "Presione cualquier tecla para continuar...\n";
            getwchar();
            break;
        case 4:
            FilterScreen("Ninguno", "Complete");
            break;
        case 5:
            menuBusqueda();
            break;
        case 0:
            exit(0);
            return;
        default:
            break;
        }
    } while (select != 0);
}

int main()
{

    cout << "ALBUM PANINI 2022 | Eddy Marroquin 1146822\n";
    cout << "Ingrese una lista CSV para ejecutar el Programa\n";
    //getline(cin, sourceFile);
    //albumlist = albumReader.read_album(sourceFile);

    //MainScreen();

    Console::ReadKey();

    return 0;



}
