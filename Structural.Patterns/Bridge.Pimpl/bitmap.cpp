#include "bitmap.hpp"
#include <algorithm>
#include <array>
#include <deque>

using namespace std;

struct Bitmap::BitmapImpl
{
    std::deque<uint8_t> image_;
};

Bitmap::Bitmap(size_t size, char fill_char) 
    : pimpl_{std::make_unique<BitmapImpl>(BitmapImpl{std::deque<uint8_t>(size, fill_char)})}
{
}

Bitmap::~Bitmap() = default;

void Bitmap::draw()
{
    cout << "Image: ";
    for (const auto& pixel : pimpl_->image_)
        cout << pixel;
    cout << endl;
}
