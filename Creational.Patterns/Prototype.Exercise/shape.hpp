#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "point.hpp"
#include <memory>

namespace Drawing
{
    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual void move(int x, int y) = 0;
        virtual void draw() const = 0;
        virtual std::unique_ptr<Shape> clone() const = 0;
    };

    template <typename ShapeT, typename ShapeBaseT = Shape>
    class CopyableShape : public ShapeBaseT
    {
    public:
        std::unique_ptr<Shape> clone() const override
        {
            return std::make_unique<ShapeT>(*static_cast<const ShapeT*>(this));
        }
    };

    template <typename ShapeT>
    class ShapeBase : public CopyableShape<ShapeT>
    {
        Point coord_; // composition
    public:
        Point coord() const
        {
            return coord_;
        }

        void set_coord(const Point& pt)
        {
            coord_ = pt;
        }

        ShapeBase(int x = 0, int y = 0)
            : coord_ {x, y}
        {
        }

        void move(int dx, int dy) override
        {
            coord_.translate(dx, dy);
        }
    };
}

#endif