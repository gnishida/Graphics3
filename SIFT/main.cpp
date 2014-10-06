#include <iostream>
#include <time.h>
#include "SIFT.h"
#include "TIFF.h"

using namespace std;

int main() {
	TIFFImage image("image.tif");
    
    clock_t start = clock();
    
    SIFT sift(&image);
    
    clock_t end = clock();
    cout << "time = " << (double)(end-start) / CLOCKS_PER_SEC << " [sec]" << endl;
    
    return 0;
}
