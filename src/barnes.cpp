#include <iostream>

// If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
// Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
// Otherwise, run the procedure recursively on each of the current node’s children.

bool isExternalNode() {
    return true;
}

double sdRatio() {

}

void calcForce() {
    if (isExternalNode()) {
        
    }
    else if (sdRatio() < 0) {

    }
    else {
        //run recursively on children
    }
}
