#ifndef CONVEXHULLGENERATOR_H
#define CONVEXHULLGENERATOR_H

#include <deque>

struct pt
{
    float x, y;
};

class ConvexHullGenerator
{
public:
    ConvexHullGenerator();
    virtual ~ConvexHullGenerator();
    void Draw();
    void Init();
    void Update();
    void CalculateHull();
    void ClearPts();
    float CalcAngle(pt,pt,pt);
protected:
private:
    bool mouseRecentlyClicked;
    //static const int PLOTTING_MODE = 1;
    //static const int COMPLETE_MODE = 2;
    enum ConvexHullGeneratorMode { PLOTTING_MODE, COMPLETE_MODE };
    ConvexHullGeneratorMode currentMode;
    std::deque<pt> unusedPts;
    std::deque<pt> usedPts;
};



#endif // CONVEXHULLGENERATOR_H
