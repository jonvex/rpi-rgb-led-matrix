#include "../include/led-matrix.h"

#include "pixel-mapper.h"
#include "graphics.h"

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>

using namespace rgb_matrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

class Board {
public:
    Board(Canvas *m) {
        m_canvas = m;
    }
    void Square(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b) {
        for (int i = x1; i <= x2; ++i) {
            for (int j = y1; j <=y2; ++j) {
                m_canvas->SetPixel(i,j,r,g,b);
            }
        }
    }
private:
    Canvas *m_canvas;
};

int main() {
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;
    matrix_options.chain_length = 1;
    matrix_options.parallel = 1;
    matrix_options.rows = 32;
    matrix_options.cols=64;
    matrix_options.hardware_mapping="adafruit-hat";
    matrix_options.disable_hardware_pulsing=true;

    RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    if (matrix == NULL) {
        return 1;
    }

    printf("Size: %dx%d. Hardware gpio mapping: %s\n", matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    printf("Press <CTRL-C> to exit and reset LEDs\n");

    Canvas *canvas = matrix;
    Board *b = new Board(canvas);
    b->Square(0,0,1,1,255,0,0);
    
    delete b;
    delete canvas;
    printf("Received CTRL-C. Exiting.\n");
    return 0;
}