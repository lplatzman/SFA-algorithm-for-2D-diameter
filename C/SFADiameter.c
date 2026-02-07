//
//  SFADiameter.c
//

/*
MIT License

Copyright (c) 2026 Loren Platzman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "SFADiameter.h"

typedef struct {
    SFAPoint *point;
    float r2, theta;
} Bin;

typedef struct {
    int n, k, m;
    SFAPoint *hub;
    Bin *sorter, *packer;
    float diameter2;
    SFAResult *result;
} Local;

// Utilities

// Returns the square of the distance between points a and b.
// This avoids taking square roots when comparing distances.
static float d2(SFAPoint *a, SFAPoint *b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    return dx * dx + dy * dy;
}

// Returns the angle of the line segment from a to b.
// The angle ranges from 0 (right) to 2*pi (just before full circle).
// This makes it possible to compare angles in the algorithms
// without having to do modulo operations.
static float theta(SFAPoint *a, SFAPoint *b) {
    float atan = atan2(b->y - a->y, b->x - a->x);
    if (atan >= 0)
        return atan;
    else
        return atan + (float) (2 * M_PI);
}

// Returns the index of the slice containing point p
static int binIndex(SFAPoint *p, Local *local) {
    float th = theta(local->hub, p);
    return floor(local->k * th / M_PI);
}

// Basic operation underlying the packer.
// It is invoked from two places and so appears in its own function.
static void addToPacker(SFAPoint *p, Local *local) {
    Bin *b;
    float theta1, theta2=42;

    while (local->m > 0) {
        b = &local->packer[local->m - 1];
        theta1 = b->theta;
        theta2 = theta(b->point, p);
        if (theta2 > theta1)
            break;
        local->m--;
    }

    b = &local->packer[local->m++];
    b->point = p;
    b->theta = theta2;
}

// Accumulates pairs of points retaining the longest.
static void apply(int i, int j, Local *local) {
    SFAPoint *a = local->packer[i].point;
    SFAPoint *b = local->packer[j].point;
    float d2new = d2(a, b);

    if (d2new > local->diameter2) {
        local->result->p1 = a;
        local->result->p2 = b;
        local->diameter2 = d2new;
    }
}

void SFADiameter(SFAPoint *points, int numberOfPoints, float epsilon, SFAResult *result) {
    Local local;
    local.diameter2 = 0;
    local.result = result;
    local.n = numberOfPoints;
    local.k = ceil(2 * M_PI / epsilon);
    
    int workArraySize = (local.k+3) * sizeof(Bin);
    local.sorter = (Bin *) malloc(workArraySize);
    memset(local.sorter, 0, workArraySize);
    local.packer = (Bin *) malloc(workArraySize);

    
    ////////////////////////////////////////////////////
    // Compute hub

    local.hub = &points[0];

    for (int i = 1; i < local.n; i++) {
        SFAPoint *p = &points[i];
        if (p->y > local.hub->y)
            continue;
        if (p->y < local.hub->y || p->x < local.hub->x)
            local.hub = p;
    }

    ////////////////////////////////////////////////////
    // Sort (bin sort / each bin has a capacity of one)

    for (int pointIndex = 0; pointIndex < numberOfPoints; pointIndex++) {
        SFAPoint *p = &points[pointIndex];
        float r2 = d2(local.hub, p);
        if (r2 == 0)
            continue;

        int i = binIndex(p, &local);
        Bin *bin = &local.sorter[i];
        if (!bin->point) {
            bin->point = p;
            bin->r2 = r2;
            continue;
        }

        if (bin->r2 >= r2)
            continue;

        bin->point = p;
        bin->r2 = r2;
    }

    ////////////////////////////////////////////////////
    // Pack (Graham scan)

    Bin *bin = local.packer;
    bin->point = local.hub;
    bin->theta = -1E-5f;
  
    local.m = 1;

    for (int i = 0; i < local.k; i++) {
        bin = &local.sorter[i];
        if (!bin->point)
            continue;
        addToPacker(bin->point, &local);
    }

    switch (local.m) {
    case 1:
        result->p1 = result->p2 = local.hub;
        result->diameter = 0;
        return;

    case 2:
        result->p1 = local.hub;
        result->p2 = local.packer[1].point;
            result->diameter = (float) sqrt(d2(result->p1, result->p2));
        return;

    default:
        break;
    }

    addToPacker(local.hub, &local);

    local.packer[0].theta = local.packer[local.m - 1].theta - 2 * (float) M_PI;

    bin = &local.packer[local.m++];
    bin->point = local.packer[1].point;
    bin->theta = local.packer[1].theta + 2 * (float) M_PI;
   
    ////////////////////////////////////////////////////
    // Measure (rotating calipers)

    float th = local.packer[1].theta + (float) M_PI;

    int i = 0;
    int j = 2;
    while (local.packer[j + 1].theta < th) {
        j++;
    }

    apply(i, j, &local);

    while (j < local.m - 1) {
        if (local.packer[i + 1].theta + (float) M_PI < local.packer[j + 1].theta) {
            i++;
        } else {
            j++;
        }
        apply(i, j, &local);
    }

    result->diameter = (float) sqrt(local.diameter2);
    
    free(local.sorter);
    free(local.packer);
}


void ExactDiameter(SFAPoint *points, int numberOfPoints, SFAResult *result) {
    float d2max = 0;
    
    for (int i = 0; i < numberOfPoints; i++) {
        for (int j = 0; j < i; j++) {
            float d2ij = d2(&points[i], &points[j]);
            if (d2ij > d2max) {
                d2max = d2ij;
                result->p1 = &points[i];
                result->p2 = &points[j];
            }
        }
    }
    
    result->diameter = (float) sqrt(d2max);
}
