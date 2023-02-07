#include <iostream>
#include "bigint.h"
using namespace std;

// Write your implementation below

BigInt::BigInt(std::string s, int base){
  for(int i=s.size(); i >= 0; i--){
    if(s[i] >= 'A' && s[i] <= 'Z'){
      vec.push_back(static_cast<int>(s[i] - 'A') + 10);
    }
    if(s[i] >= '0' && s[i] <= '9'){
      vec.push_back(static_cast<int>(s[i]-'0'));
    }
  }
  this->base = base;
  removeLeadingZeroes();

}

std::string BigInt::to_string() const{

  string output = "";
  for(int i=vec.size()-1; i >= 0; i--){
    if(vec[i]>=0 && vec[i]<10){
      output += static_cast<char>(vec[i]+'0');
    }
    else{
      output += static_cast<char>(vec[i]-10+'A');
    }
  }

  return output;
}

void BigInt::add(BigInt b){ // adds two big ints together
  while(vec.size() != b.vec.size()){
    if(vec.size() < b.vec.size()){ // makes vectors equal size
      vec.push_back(0);
    }
    else{
      b.vec.push_back(0);  
    }
  }
  int cy = 0;
  int val= 0;
  for(int i=0; i < (signed) vec.size(); i++){ // adds two values in index of vector together
    val = vec[i] + b.vec[i] + cy;
    if(base > vec[i] + b.vec[i] + cy){ // if sum is less than base, carry for next digit is 0
      cy = 0;
    }
    else{ 
      cy =  val / base;
      val = val % base;
    }
    vec[i] = val;
  }
  if(cy == 1){ // if carry is one at the end, pushes back another value. 
    vec.push_back(cy);
  }
  this->removeLeadingZeroes();

}


void BigInt::sub(BigInt b){ // subtracts two bigints. 
  while(vec.size() != b.vec.size()){
  if(vec.size() < b.vec.size()){
    vec.push_back(0);
    }
  else{
    b.vec.push_back(0);  
    }
  }
  int cy = 0; // carry 
  int val= 0;
  for(int i=0; i < (signed) vec.size(); i++){
    if(vec[i] - b.vec[i] + cy < 0){ // if subtraction is less than 0, borrows one from the next index of vector
      val = base + (vec[i] - b.vec[i] + cy);
      cy = -1;
    }
    else{
      val = vec[i] - b.vec[i] + cy;
      cy = 0;

    }
    vec[i] = val;
  }
  this->removeLeadingZeroes(); // removes leading zeroes

}

bool BigInt::less(BigInt b){ // compares two values and returns true or false if the first is less than the second. 
  if(b.vec.size() > vec.size()){ //returns true if b is smaller than other vector.
    return true;
  }
  else if(b.vec.size() < vec.size()){ // returns false if b is smaller than other vector. 
    return false;
  }
  else{ // loops through values until values in two vectors are not equal, then compares them and returns bool. 
    for(int i=0; i < (signed) vec.size(); i++){
      int lastI = (signed) vec.size() - 1;
      if(b.vec[lastI-i] > vec[lastI-i]){
        return true;
      }
      else if(b.vec[lastI-i] < vec[lastI-i]){
        return false;
      }
    }
  }
  return true; // should never reach this line. 
}


BigInt BigInt::operator+(const BigInt& rhs) const{ // operator overloading for + 
  BigInt newT = *this;
  newT.add(rhs); // performs add function
  newT.removeLeadingZeroes();
  return newT;
}

BigInt BigInt::operator-(const BigInt& rhs) const{// operator overloading for -
  BigInt newT = *this;
  newT.sub(rhs); // performs sub function
  newT.removeLeadingZeroes();
  return newT;
}

bool BigInt::operator<(const BigInt& rhs) const{// operator overloading for <
  BigInt newT = *this;
  newT.removeLeadingZeroes();
  return newT.less(rhs); // performs less function and returns func. 
}


void BigInt::removeLeadingZeroes(){
  if(vec.size() == 1 && vec[0] == 0){ // if size of vector is 1 and the value is 0, do nothing. 
    return;
  }
  for(int i=0; i < (signed) vec.size() ; i++){ // loops through each value and if value is zero, erases the zero. 
    int max = (signed) vec.size() - 1;
    if(vec[max] != 0){
      break;
    }
    else{
      vec.erase(vec.end()-1);
    }
  }
}



