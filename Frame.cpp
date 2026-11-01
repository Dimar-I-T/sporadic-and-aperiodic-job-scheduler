#include <iostream>
#include <vector>
#include "Frame.h"

using namespace std;

Frame::Frame(double length) {
    Frame::length = length;
    Frame::remaining.resize(length, 1.0);
    Frame::slack = length;
}

double Frame::getLength() {
    return Frame::length;
}