#ifndef COLOR_H
#define COLOR_H

// Used to easily pack and
// communicate rgb values.
class Color
{
    public:
        Color() { r = g = b = 0; }
        void set_red(unsigned char red) { r = red; }
        void set_green(unsigned char green) { g = green; }
        void set_blue(unsigned char blue) { b = blue; }
        unsigned char get_red() const { return r; }
        unsigned char get_green() const { return g; }
        unsigned char get_blue() const { return b; }

    private:
        unsigned char r, g, b;
};

#endif
