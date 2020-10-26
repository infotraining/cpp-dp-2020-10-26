#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.hpp"

namespace Drawing
{
    // TODO - add a circle to Shape hierarchy

    class Circle : public ShapeBase
    {
        int radius_;
    public:
        static constexpr const char* id = "Circle";

        Circle(int x = 0, int y = 0, int r = 0)
            : ShapeBase{x, y}, radius_{r}
        {}

        int r() const
        {
            return radius_;
        }

        void set_r(int r)
        {
            radius_ = r;
        }

        void draw() const override
        {
            std::cout << "Drawing circle: " << coord() << " r = " << radius_ << "\n";
        }
    };
}

#endif // CIRCLE_H
