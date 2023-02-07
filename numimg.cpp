#include "digitblob.h"
#include "numimg.h"
#include "bmplib.h"
#include <algorithm> // for std::sort
#include <deque>
#include <iomanip>
#include <iostream>

using namespace std;

// TO DO: Complete this function
NumImg::NumImg(const char* bmp_filename)
{
    //  Note: readGSBMP dynamically allocates a 2D array 
    //    (i.e. array of pointers (1 per row/height) where each  
    //    point to an array of unsigned char (uint8_t) pixels)

    // TO DO:
    // call readGSBMP to initialize img_, h_, and w_;
    
    img_ = readGSBMP(bmp_filename, h_, w_);
    // Leave this check
    if(img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }
    
    // Convert to Black and White using a fixed threshold 
    for(int i =0; i < h_; i++){
        for(int j = 0; j < w_; j++){
            if(img_[i][j] > 150){
                img_[i][j] = 255;
            }
            else {
                img_[i][j] = 0;
            }
        }
    }
    // Perform any other initialization you need
    explored = new bool* [h_];
    for(int i=0; i<h_; i++){
        explored[i] = new bool[w_];
        for(int j=0; j<w_; j++){
            explored[i][j] = false;
        }
    }

}

// TO DO: Complete this function
NumImg::~NumImg()
{
    // Add code here if necessary
    for(int i=0; i<h_; i++){
        delete[] img_[i];
    }
    delete[] img_;
    for(int i=0; i<h_; i++){ // deletes allocated memory
      delete[] explored[i];
    }
    delete[] explored;

}



// TO DO: Complete this function
size_t NumImg::findAndCreateDigitBlobs()
{
    
    size_t blobC = 0;
    for(int i=0; i < h_; i++){
        for(int j=0; j<w_; j++){ // looping through each index of image
            if(explored[i][j] == false && img_[i][j] == 0){ // if it has not been explored, and is black, enter if statement
                DigitBlob tempBlob;
                explored[i][j] = true;
                tempBlob = createDigitBlob(explored, i, j); // runs create digit blob
                blobs_.push_back(tempBlob); // adds digit blob to blobs vector
                blobC++;
            }
        }
    }

    sortDigitBlobs();

    return blobC;
}

// Complete - Do not alter
std::string NumImg::classify(bool withDebug)
{
    std::string res;
    for(size_t i = 0; i < blobs_.size(); i++){
        blobs_[i].classify();
        if(withDebug){
            blobs_[i].printClassificationResults();
        }
        char c = blobs_[i].getClassification();
        res += c;
    }
    return res;
}

// Complete - Do not alter
void NumImg::printBoundingBoxes() const
{
    cout << setw(2) << "i" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for(size_t i = 0; i < blobs_.size(); i++){
        const DigitBlob& b = blobs_[i];
        cout << setw(2) << i << setw(6) << b.getUpperLeft().row << setw(6) << b.getUpperLeft().col 
        << setw(4) << b.getHeight() << setw(4) << b.getWidth()  << endl;
        // cout << "Blob " << i << " ul=(" << b.getUpperLeft().row << "," << b.getUpperLeft().col 
        //     << ") h=" << b.getHeight() << " w=" << b.getWidth() << endl;
    }
}

// Complete - Do not alter
const DigitBlob& NumImg::getDigitBlob(size_t i) const
{
    if(i >= blobs_.size()){
        throw std::out_of_range("Index to getDigitBlob is out of range");
    }
    return blobs_[i];
}

// Complete - Do not alter
size_t NumImg::numDigitBlobs() const
{
    return blobs_.size();
}

// Complete - Do not alter
void NumImg::sortDigitBlobs()
{
    std::sort(blobs_.begin(), blobs_.end());
}

// Complete - Do not alter
void NumImg::drawBoundingBoxesAndSave(const char* filename)
{
    for(size_t i=0; i < blobs_.size(); i++){
        Location ul = blobs_[i].getUpperLeft();
        int h = blobs_[i].getHeight();
        int w = blobs_[i].getWidth();
        for(int i = ul.row-1; i < ul.row + h + 1; i++){
            img_[i][ul.col-1] = 128;
            img_[i][ul.col+w] = 128;
        }
        for(int j = ul.col-1; j < ul.col + w + 1; j++){
            img_[ul.row-1][j] = 128;
            img_[ul.row+h][j] = 128;
        }
    }
    writeGSBMP(filename, img_, h_, w_);
}

// TO DO:
// Add other (helper) function definitions here

DigitBlob NumImg::createDigitBlob(bool** explored, int pr, int pc)
{
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    // int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    // int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};

    // Add your code here
    int minR = h_; 
    int minC = w_; 
    int maxR = 0;
    int maxC = 0;
   
    deque<Location> dq; // creates deque for locations that need to be searched
    Location startVal;
    Location now;
    startVal.row = pr;
    startVal.col = pc;
    dq.push_back(startVal);
    Location topL;

    while(!dq.empty()){ // while deque is not empty, keep on running
        now = dq.front(); // remove front var
        dq.pop_front();

        Location north, east, south, west, nw, ne, se, sw;

        north.row = now.row - 1; 
        north.col = now.col;

        east.row = now.row;
        east.col = now.col + 1;

        south.row = now.row + 1;
        south.col = now.col;

        west.row = now.row;
        west.col = now.col - 1;

        nw.row = now.row - 1;
        nw.col = now.col - 1;

        ne.row = now.row - 1;
        ne.col = now.col + 1;

        se.row = now.row + 1;
        se.col = now.col + 1;

        sw.row = now.row + 1;
        sw.col = now.col - 1;

        Location toExplore[8] = {north, east, south, west, nw, ne, se, sw};
        for(int k=0; k<8; k++){
            if((toExplore[k].row < h_) && (toExplore[k].row >= 0) && (toExplore[k].col < w_) && (toExplore[k].col >= 0)){ // checks to see if within range
                if((explored[toExplore[k].row][toExplore[k].col] == false) && (img_[toExplore[k].row][toExplore[k].col] == 0) ){ // sees if value black and not explored
                    dq.push_back(toExplore[k]); // adds index to queue
                    explored[toExplore[k].row][toExplore[k].col] = true;

                    if(toExplore[k].row < minR){ // checks to see if val is min 
                        minR = toExplore[k].row;
                    }

                    if(toExplore[k].row > maxR){// checks to see if val is max 
                        maxR = toExplore[k].row;
                    }

                    if(toExplore[k].col < minC){// checks to see if val is min 
                        minC = toExplore[k].col;
                    }

                    if(toExplore[k].col > maxC){// checks to see if val is max 
                        maxC = toExplore[k].col;
                    }
                }
            }
        }
    }
    int width = maxC - minC + 1; //declaring w for bounding box
    int height = maxR - minR + 1; //declaring h for bounding box
    topL.col = minC;
    topL.row = minR;
    DigitBlob tempBlob = DigitBlob(img_, topL, height, width);
    // cout << "MaxC: " << maxC << " MaxR: " << maxR << " MinC " << minC << " minR " << minR << endl;
    // cout << "DistR " << distR << endl;
    // cout << "DistC " << distC << endl;

    return tempBlob;
}


