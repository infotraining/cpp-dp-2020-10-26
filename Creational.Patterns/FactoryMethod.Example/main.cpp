#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>

#include "rectangle.hpp"
#include "shape.hpp"
#include "shape_readers_writers/rectangle_reader_writer.hpp"
#include "shape_readers_writers/square_reader_writer.hpp"
#include "square.hpp"

using namespace std;
using namespace Drawing;
using namespace Drawing::IO;

unique_ptr<Shape> create_shape(const string& id)
{
    if (id == Rectangle::id)
        return make_unique<Rectangle>();
    else if (id == Square::id)
        return make_unique<Square>();

    throw runtime_error("Unknown shape id");
}

unique_ptr<ShapeReaderWriter> create_shape_rw(Shape& shape)
{
    if (typeid(shape) == typeid(Rectangle))
        return make_unique<RectangleReaderWriter>();
    else if (typeid(shape) == typeid(Square))
        return make_unique<SquareReaderWriter>();

    throw runtime_error("Unknown shape id");
}

namespace Refactoring
{
    using ShapeCreator = std::function<std::unique_ptr<Shape>()>;

    class ShapeFactory
    {
        std::unordered_map<std::string, ShapeCreator> creators_;

        auto create(const std::string& id)
        {
            return creators_.at(id)();
        }
        
    };

    using ShapeRWCreator = std::function<std::unique_ptr<ShapeReaderWriter>()>;

    class ShapeRWFactory
    {
        std::unordered_map<std::type_index, ShapeRWCreator> creators_;

        auto create(const std::string& id)
        {
            return creators_.at(id)();
        }
    };
}

class GraphicsDoc
{
    vector<unique_ptr<Shape>> shapes_;
    Refactoring::ShapeFactory& sf_;
    Refactoring::ShapeRWFactory& srwf_;

public:
    GraphicsDoc(Refactoring::ShapeFactory& sf, Refactoring::ShapeRWFactory& srwf)
        : sf_{sf}, srwf_{srwf}
        {}

    void add(unique_ptr<Shape> shp)
    {
        shapes_.push_back(move(shp));
    }

    void render()
    {
        for (const auto& shp : shapes_)
            shp->draw();
    }

    void load(const string& filename)
    {
        ifstream file_in{filename};

        if (!file_in)
        {
            cout << "File not found!" << endl;
            exit(1);
        }

        while (file_in)
        {
            string shape_id;
            file_in >> shape_id;

            if (!file_in)
                return;

            cout << "Loading " << shape_id << "..." << endl;

            auto shape = sf_.create(shape_id);
            auto shape_rw = srwf_.create(*shape);

            shape_rw->read(*shape, file_in);

            shapes_.push_back(move(shape));
        }
    }

    void save(const string& filename)
    {
        ofstream file_out{filename};

        for (const auto& shp : shapes_)
        {
            auto shape_rw = srwf_.create(*shp);
            shape_rw->write(*shp, file_out);
        }
    }
};

int main()
{
    cout << "Start..." << endl;

    Refactoring::ShapeFactory shape_factory;
    shape_factory.register(Rectangle::id, &std::make_unique<Rectangle>);
    shape_factory.register(Square::id, &std::make_unique<Square>);

    GraphicsDoc doc(shape_factory);

    doc.load("drawing.txt");

    cout << "\n";

    doc.render();

    doc.save("new_drawing.txt");
}
