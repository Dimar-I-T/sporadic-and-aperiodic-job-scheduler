#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <vector>
#include <string>

class Frame {
    public:
        Frame(double length);
        double getLength();
        void setSlack(double slack);
        std::vector<double> remaining; // [0, 1]
        std::vector<std::string> jobs;
        double slack;
        double serverSlot = 0;
    private:
        double length;
};

#endif