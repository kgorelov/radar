#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

int main() {
    unsigned angle = 0;
    bool isGoingDown = false;

    srand((unsigned) time(NULL));

    // Radar field
    unsigned short field[181];
    memset(field, 0, sizeof(field));

    // Generate objects
    for (int i=0; i<3; ++i) {
        unsigned start = rand()%181;
        unsigned length = 1 + rand()%5;
        unsigned distance = 20 + rand()%60;
        if (start + length > 180) {
            length = 180-start+1;
        }
        int skew = rand()%20 - 10;
        for (unsigned o=start; o < (start+length); ++o) {
            field[o] = distance;
            distance += skew;
        }
    }

    // Generate radar signal
    while (1) {
        std::cout << angle << " " << field[angle] << std::endl;

        if (isGoingDown) {
            --angle;
        } else {
            ++angle;
        }
        if (angle == 0 || angle == 180) {
            isGoingDown = !isGoingDown;
        }
        usleep(50000);
    }
}
