#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Shape
{
    // shape interface
    class IShape
    {
      public:
        virtual ~IShape() = default;
        virtual void move(int x, int y) = 0;
        virtual void draw() const = 0;
        virtual unique_ptr<IShape> clone() = 0;
    };

    template <typename T>
    class ShapeWrapper : public IShape
    {
        T shape_;

      public:
        ShapeWrapper(const T& shp)
            : shape_{shp}
        {
        }

        void draw() const override
        {
            shape_.draw();
        }

        void move(int x, int y) override
        {
            shape_.move(x, y);
        }

        unique_ptr<IShape> clone() override
        {
            return make_unique<ShapeWrapper<T>>(shape_);
        }
    };

  public:
    template <typename T>
    Shape(const T& shp)
        : shape_(make_unique<ShapeWrapper<T>>(shp))
    {
    }

    Shape(const Shape &src)
        : shape_{src.shape_->clone()}
    {
    }

    Shape &operator=(const Shape &src)
    {
        Shape temp(src);
        swap(temp);

        return *this;
    }

    template <class T>
    Shape &operator=(T&& src)
    {
        Shape(std::forward<T>(src)).swap(*this);
        return *this;
    }

    Shape(Shape &&) noexcept = default;
    Shape &operator=(Shape &&) noexcept = default;

    void swap(Shape &other)
    {
        shape_.swap(other.shape_);
    }

    void draw() const
    {
        shape_->draw();
    }

    void move(int x, int y)
    {
        shape_->move(x, y);
    }

  private:
    unique_ptr<IShape> shape_;
};

struct Circle
{
    int x, y;
    int r;

    Circle(int x, int y, int r)
        : x{x}, y{y}, r{r}
    {
    }

    void draw() const
    {
        cout << "Circle([" << x << ", " << y << "], " << r << ")\n";
    }

    void move(int dx, int dy)
    {
        x += dx;
        y += dy;
    }
};

struct Square
{
    int x, y;
    int size;

    Square(int x, int y, int r)
        : x{x}, y{y}, size{r}
    {
    }

    void draw() const
    {
        cout << "Square([" << x << ", " << y << "], " << size << ")\n";
    }

    void move(int dx, int dy)
    {
        x += dx;
        y += dy;
    }
};

class GraphicDocs
{
    vector<Shape> shapes_;

  public:
    GraphicDocs() = default;

    GraphicDocs(const vector<Shape> &shapes)
        : shapes_(shapes)
    {
    }

    void add(Shape shp)
    {
        shapes_.push_back(shp);
    }

    void render()
    {
        for (const auto &shp : shapes_)
            shp.draw();
    }
};

int main(int argc, char *argv[])
{
    auto c = Circle(1, 2, 10);

    Shape shp1 = c;
    shp1.draw();

    cout << "\n\n";

    Shape shp2 = shp1;
    shp1 = Square(30, 40, 100);

    shp1.draw();
    shp2.draw();

    cout << "\n\n";

    vector<Shape> shapes = {shp1, shp2, Circle(100, 200, 2), Square{10, 20, 300}};

    shp1.move(10, 1000);
    shp1.draw();

    for (const auto &shp : shapes)
        shp.draw();

    cout << "\n\n";

    GraphicDocs doc(shapes);
    doc.add(Circle(1, 1, 20));
    doc.add(Square(2, 3, 10));
    doc.add(Circle(10, 2, 50));
    doc.render();
}
