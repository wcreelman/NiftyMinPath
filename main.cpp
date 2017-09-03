/*****************************************************************************
 * Name:                                                                    *
 * Course:                                                                  *
 * Date:                                                                    *
 *	                                                                         *
 * Purpose:                                                                 *
 *	                                                                         *
 * Usage:                                                                   *
 *	                                                                         *
 * Revision History:                                                        *
 *	                                                                         *
 * Known Issues:                                                            *
 *	                                                                         *
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <apmatrix.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_DISPLAY *display;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *arial;
ALLEGRO_FONT *garamond;
ALLEGRO_EVENT_QUEUE *event_queue;

#define WHITE           al_map_rgb(219, 255, 219)

int initializeAllegro(int width, int height, const char title[]);
bool escapeKeypress();
const char *filename = "Colorado_480x480.dat";

bool readFile(const char *fname, apmatrix<short> &map);
short findMin(apmatrix<short> &map);
short findMax(apmatrix<short> &map);

void drawMap(short mapMin,short mapMax, apmatrix<short> map);
ALLEGRO_COLOR mapColour(short min,short max, short current);
using namespace std;

int main(int argc, char **argv) {
    apmatrix<short> map;

   if (readFile(filename, map)) {
        cout << "okay " << endl;

   }
   short mapMin = findMin(map);
   short mapMax = findMax(map);
   initializeAllegro(map.numcols()*2, map.numrows() *2, "Colorado Map");
   drawMap(mapMin, mapMax, map);
   return 0;
}

ALLEGRO_COLOR mapColour(short mapMin,short mapMax, short current) {
    int c =  (current - mapMin) / float(mapMax - mapMin)* 256.0;

    return al_map_rgb(c, c, c);
}

void drawMap(short mapMin,short mapMax, apmatrix<short> map) {
    const int squareSize = 2;
     al_clear_to_color(WHITE);
     for (int i=0; i<map.numrows(); i++) {
        for (int j=0; j<map.numcols(); j++) {
             al_draw_filled_rectangle(j*squareSize, i*squareSize,
                    j*squareSize+squareSize, i*squareSize+squareSize, mapColour(mapMin, mapMax, map[j][i]));
        }
     }
     al_flip_display();
     al_rest(3);

}

short findMin(apmatrix<short> &map) {
    short m = map[0][0];

    for (int i=0; i<map.numrows(); i++) {
        for (int j=0; j<map.numcols(); j++) {
            if (map[i][j] < m)
                m = map[i][j];
        }
    }
    return m;
}

short findMax(apmatrix<short> &map) {
    short m = map[0][0];

    for (int i=0; i<map.numrows(); i++) {
        for (int j=0; j<map.numcols(); j++) {
            if (map[i][j] > m)
                m = map[i][j];
        }
    }
    return m;
}


bool readFile(const char *fname, apmatrix<short> &map) {
    int nRows, nCols;
    ifstream fin(fname);
    if (!fin) {
        cout << fname << " not found " << endl;
        return false;
    }
    fin >> nRows >> nCols;
    map.resize(nRows, nCols);
    for (int i=0; i<nRows; i++) {
        for (int j=0; j<nCols; j++) {
             fin >> map[i][j];
             if (fin.fail()) {
                fin.close();
                cout << "Failure to read file at " << i << " " << j << endl;
                return false;
            }
        }
    }
    fin.close();
    return true;
}
