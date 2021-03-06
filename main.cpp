#include <iostream>
#include <fstream>

using namespace std;

/*
 * Structure of image data:
 * For every pixel we have 2 bytes with 5 bits for every color:
 * X X X X X X X X | X X X X X X X X - bits
 * G G G R R R R R | 0 B B B B B G G - colors
 * 3 2 1 5 4 3 2 1 | 0 5 4 3 2 1 5 4 - importance of bits
 * Where:
 * G - green
 * B - blue
 * R - red
 * 0 - unused
 * 1, 2, 3, 4, 5 - bit number (1 - least important)
 *
 *
 */

int main(int argc, char* argv[]){
    ifstream inFile;
    ofstream outFile;
    char byte1, byte2;
    uint8_t red5, green5, blue5, green25;
    uint8_t red8, green8, blue8;
    for (int i=1;i<argc;i++){
        inFile.open(argv[i], std::fstream::binary);
        //Creating .data filename for usage with GIMP
        char output[5000];
        sprintf(output, "%s.data", argv[i]);
        outFile.open(output, std::fstream::binary);
        if (inFile.is_open()) {
            cout << "File opened" << endl;
            int pixelNumber = 1;
            while (!inFile.eof()) {
                //Reading 2 bytes
                inFile.read(&byte1, 1);
                inFile.read(&byte2, 1);
                //Extracting 5 red bits
                red5 = byte1 & 0b00011111;
                //Extracting 5 blue bits
                blue5 = byte2 & 0b01111100;
                blue5 >>= 2;
                //Extracting 5 green bits
                green5 = byte2 & 0b00000011;
                green5 <<= 3;
                green25 = byte1 & 0b11100000;
                green25 >>= 5;
                green5 = green5 | green25;
                //Converting to 8 bit
                red8 = ((red5 * 527) + 23) >> 6;
                green8 = ((green5 * 527) + 23) >> 6;
                blue8 = ((blue5 * 527) + 23) >> 6;
                //Writing to file
                outFile.write(reinterpret_cast<const char *>(&red8), sizeof (red8));
                outFile.write(reinterpret_cast<const char *>(&green8), sizeof (green8));
                outFile.write(reinterpret_cast<const char *>(&blue8), sizeof (blue8));
                pixelNumber++;
            }
            cout << "THE END" << endl;
            inFile.close();
            outFile.close();
        } else {
            cout << "Error opening file" << argv[i];
        }
    }
    return 0;
    }
