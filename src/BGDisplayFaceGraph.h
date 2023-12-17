#ifndef BGDISPLAYFACEGRAPH_H
#define BGDISPLAYFACEGRAPH_H

#include "BGDisplayFaceGraphBase.h"

class BGDisplayFaceGraph : public BGDisplayFaceGraphBase {
  public:
    void showReadings(const std::list<GlucoseReading> &readings) const override;
    void markDataAsOld() const override;
};

#endif