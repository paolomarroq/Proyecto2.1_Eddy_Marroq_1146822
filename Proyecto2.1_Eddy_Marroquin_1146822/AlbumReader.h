#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "List.h"
#include "LinkedList.h"
#include "Sticker.h"
#include "Category.h"
#include <vector>



using namespace std;

class AlbumReader
{
private:
    static vector<string> split(string& str, char delimiter) {
        vector<string> result;

        int beginning = 0;
        int i = 0;

        while ((i = str.find(delimiter, i)) != string::npos) {
            string current_string(str.substr(beginning, i - beginning));

            result.push_back(current_string);

            i++;
            beginning = i;
        }

        result.push_back(str.substr(beginning, i - beginning));

        return result;
    }

    static void add_stickers_from_line(List<Sticker>* stickers,
        Category* category,
        string stickers_line) {
        vector<string> sticker_line = split(stickers_line, ',');

        for (vector<string>::iterator i = sticker_line.begin();
            i != sticker_line.end();
            i++) {

            vector<string> sticker_info = split(*i, '-');
            Sticker* sticker
                = new Sticker(category, stoi(sticker_info[0]), sticker_info[1]);

            stickers->add(sticker);

            sticker = nullptr;
            delete sticker;
        }
    }

    static List<Sticker>* read_album_from_file(fstream& file) {
        List<Sticker>* stickers = new LinkedList<Sticker>();

        string category_line;

        Category* current_category;

        while (getline(file, category_line))
        {
            vector<string> category_info = split(category_line, ',');

            current_category = new Category(category_info[1], category_info[0]);

            string stickers_line;

            getline(file, stickers_line);
            add_stickers_from_line(stickers, current_category, stickers_line);

            current_category = nullptr;
            delete current_category;
        }

        return stickers;
    }

public:
    static List<Sticker>* read_album(string file_path) {
        fstream file;
        file.open(file_path, ios_base::in);

        if (file.good()) {
            List<Sticker>* stickers = read_album_from_file(file);

            file.close();

            return stickers;
        }

        return new LinkedList<Sticker>();
    }
};