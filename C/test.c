//
//  test.c
//

#include <stdlib.h>
#include <stdio.h>

#include "SFADiameter.h"

int main(int argc, const char * argv[]) {
    
    SFAPoint points[] = {
        {0, 0},
        {0.5001f, 0.8661f},
        {0.5424f, 0.8399f},
        {-0.5001f, 0.8661f},
        {-0.5424f, 0.8399f}
    };
    
    int numberOfPoints = sizeof(points)/sizeof(SFAPoint);
    float epsilon = 0.1;
    SFAResult result;
    
    SFADiameter(points, numberOfPoints, epsilon, &result);
    
    printf("Approximate diameter = %f\n", result.diameter);
    printf("(%f, %f)\n", result.p1->x, result.p1->y);
    printf("(%f, %f)\n", result.p2->x, result.p2->y);
    printf("\n");
    float approx = result.diameter;
    
    ExactDiameter(points, numberOfPoints, &result);

    printf("Exact diameter = %f\n", result.diameter);
    printf("(%f, %f)\n", result.p1->x, result.p1->y);
    printf("(%f, %f)\n", result.p2->x, result.p2->y);
    printf("\n");
    float exact = result.diameter;
    
    if (exact > 0) {
        printf("Eps = %f\n", epsilon);
        printf("Err = %f\n", 1 - approx/exact);
    }
 
    return EXIT_SUCCESS;
}
