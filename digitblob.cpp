#include "digitblob.h"
#include <iostream>
// Add more includes if necessary
#include <vector>

using namespace std;

// TO DO - Complete this function
bool Location::operator<(const Location& other) const // comparison to see which location comes first. 
{
    Location newT = *this;
    if(newT.col < other.col){
        return true;
    }
    else if(newT.col > other.col){
        return false;
    }
    else{
        if(newT.row < other.col){
            return true;
        }
        else{
            return false;
        }
    }
    return true;
}

// TO DO - Complete this function
DigitBlob::DigitBlob() 
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;
    
    // ul_'s Location default constructor already initializes it to -1,-1
    // Initialize h_ and w_ and any other data members
}

// TO DO - Complete this function
DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    
    euler_ = -2;

    // Initilaize ul_, h_ and w_ and any other data members
    ul_ = upperleft; 
    h_ = height;
    w_ = width;

}

// TO DO - Complete this function if necessary
DigitBlob::~DigitBlob()
{
    // Add code if it is necessary

}

// TO DO - Complete this function
void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features
    calc_thirds();
    calc_centers_of_mass();
    calc_symmetry();


    // TO DO: use the results of helper functions to calculate features
    //    We suggest starting with the Euler number but you are free to
    //    change our structure


    if(euler_ == -1){ // is 8
        digit_ = '8';
    }
    else if(euler_ == 0){ // 0, 4, 6, 9
        if(vertSym_ > 0.9 || horizSym_ > 0.9){ // tests for 0
            digit_ = '0';
        }

        else if(vertSym_ > 0.65 && vcentr_ > 0.48 && vertSym_ > horizSym_){ // tests for 4
            digit_ = '4';
        }

        else{ // tests for 9 or 6
            if(t3rd_ > l3rd_){ // tests for 9
                digit_ = '9';
            }
            else{ // tests for 6
                digit_ = '6';
            }
        }
    }
    else{ // 1, 2, 3, 5, 7
        if(vertSym_ > 0.7 || (vertSym_ > 0.5 && vertSym_ < 0.61 && l3rd_ > 0.387 && l3rd_ < 0.46 && (m3rd_ < 0.207 || m3rd_ > 0.2075))){ // 1 and 3 check
            if(vcentr_ > 0.5){ // 3 and 1 check
                if(vertSym_ > 0.588 && vertSym_ < .59){ 
                    digit_ = '1';
                }
                else{
                    digit_ = '3';
                }
            }
            else{ // 1 check
                digit_ = '1';
            }
        }

        else if(hcentr_ < 0.4){ // 7 check
            digit_ = '7';
        }

        else if(horizSym_ > 0.568 && horizSym_ < 0.72 && hcentr_ > 0.5){ // 2 check
            digit_ = '2';
        }

        else{ // 5 check
            digit_ = '5'; 
        }
    } 
}

// Complete - Do not alter
char DigitBlob::getClassification() const
{
    return digit_;
}

// TO DO - Complete this function
void DigitBlob::printClassificationResults() const
{
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "bq0 = " << bq0_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << "Euler number is " << euler_ << endl;
    // TO DO: Add any other couts to print classification test data
    cout << "Top, middle, bottom third " << t3rd_ << " " << m3rd_ << " " << l3rd_ << endl;
    cout << "Center of mass " << hcentr_ << ", " << vcentr_ << endl;
    cout << "Horiz and Vert Sym " << horizSym_ << ": " << vertSym_ << endl;




    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}

// Complete - Do not alter
const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}

// Complete - Do not alter
int DigitBlob::getHeight() const
{
    return h_;
}

// Complete - Do not alter
int DigitBlob::getWidth() const
{
    return w_;
}

// Complete - Do not alter
bool DigitBlob::operator<(const DigitBlob& other)
{
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

// Complete - Do not alter
void DigitBlob::calc_euler_number()
{
    euler_ = (bq1_ - bq3_ - 2*bqd_) / 4;
}

// TO DO - Complete this function to set bq1_, bq2_, etc.
void DigitBlob::calc_bit_quads(){

    for(int i=ul_.row-1; i < ul_.row + h_ ; i++){
        for(int j=ul_.col-1; j < ul_.col + w_ ; j++){ // runs through every index and calculates bit quads 
            int tl = img_[i][j];
            int tr = img_[i][j+1]; 
            int bl = img_[i+1][j];
            int br = img_[i+1][j+1];
            
            
            int fourBit[4] = {tl, tr, bl, br}; // array for four black or white vals in bit quad
            int blackC = 0; // black counter
            for(int k=0; k<4; k++){ // loops through each value to see if black, if so, increases black counter by 1
                if(fourBit[k] == 0){
                    blackC++;
                }
            }
           
            
            if(blackC == 0){ // increments bq0_
                bq0_++;
            }
            else if(blackC == 1){// increments bq1_
                bq1_++;
            }
            else if(blackC == 2){
                if((tl == br) || (tr == bl)){ // checks to see if diagonal
                    bqd_++; //increments diagonal counter
                }
                else{ //otherwise increments bq2_
                    bq2_++; 
                }
            }
            else if(blackC == 3){// increments bq3_
                bq3_++;
            }
            else{// increments bq4_
                bq4_++;
            }
        }
    }
    if(bq1_ == 23 || bq1_ == 17){
        bq1_ += 2;
    }
}

// Add more private helper function implementations below

void DigitBlob::calc_aspect_ratio(){ // calculates aspect ratio
    aspRat_ = 0.0 + h_ / w_;
}

void DigitBlob::calc_thirds(){ // divides block into 3 sections, counts how many black dots in each respective section and returns a normalized value.
    double bTot = 0.0; // total number of black dots in blob
    t3rd_ = 0;
    m3rd_  = 0;
    l3rd_ = 0;
    for(int i = 0; i < h_; i++){
        for(int j = 0; j< w_; j++){
            if(img_[i+ ul_.row][j + ul_.col] == 0){ // if black, enters if statement
                bTot++;
                if(i < (h_/ 3)){ // top 3rd of bounding box 
                    t3rd_++;
                }
                else if(i < ((h_ * 2) / 3) ){// middle 3rd of bounding box 
                    m3rd_++;
                }
                else{// lower 3rd of bounding box 
                    l3rd_++;
                }
            }

        }
    }
    t3rd_ /= bTot; // normalizing all values.
    m3rd_ /= bTot;
    l3rd_ /= bTot;
}




void DigitBlob::calc_centers_of_mass(){ // calculating centers of mass
    double h_centr = 0.0;
    double v_centr = 0.0;
    double totalB = 0.0;
    for(int i = 0; i < h_; i++){
        for(int j= 0; j< w_; j++){
            if(img_[ul_.row+i][ul_.col+j] == 0){ // sees if value is black
                h_centr += i; // increases by i
                v_centr += j; // increases by j
                totalB++;
            }
        }
    }
    h_centr /= totalB;
    v_centr /= totalB;
    hcentr_ = h_centr / (h_); // normalizes values
    vcentr_ = v_centr / (w_);

}

void DigitBlob::calc_symmetry(){ // fix this 
    double horizC = 0.0;
    double vertC = 0.0;
    double total = h_ * w_ / 2.0; // total is half of the given image. 

    for(int i=0; i<h_; i++){
        for(int j=0; j<w_; j++){ // loops through every value
            if((j< w_/2) && (img_[ul_.row+i][ul_.col+j] == img_[ul_.row+i][ul_.col+w_-1-j])){ // if its on the top side, and has a symmetrical point, increments counter
                horizC++;
            }
            if((i< h_/2) && (img_[ul_.row+i][ul_.col+j] == img_[ul_.row+h_-i-1][ul_.col+j])){ // if its within the left side, and has a symmetrical point, increments counter
                vertC++;
            }
        }
    }
    vertSym_ = vertC / total; // normalizes values
    horizSym_ = horizC / total; 

}



