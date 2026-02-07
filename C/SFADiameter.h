//
//  SFADiameter.h
//

#ifndef SFADiameter_h
#define SFADiameter_h

typedef struct {
    float x, y;
} SFAPoint;

typedef struct {
    float diameter;
    SFAPoint *p1, *p2;
} SFAResult;

// Note:
// The array points is not modified by either function.
// The pointers p1 and p2 returned in result point to elements of this array.

// Computes the SFA diameter and returns the two points most distant from each other.
void SFADiameter(SFAPoint *points, int numberOfPoints, float epsilon, SFAResult *result);

// Bonus: Brute force exact solution for testing
void ExactDiameter(SFAPoint *points, int numberOfPoints, SFAResult *result);

#endif /* SFADiameter_h */
