#include <stdio.h>

class Point {
private:
    int m_x = 0;
    int m_y = 0;

public:
    Point() : Point{ 0, 0 } {};
        Point(int x, int y) : m_x{ x }, m_y{ y } 
        {}

    int x() const { return m_x; }
    int y() const { return m_y; }

    [[nodiscard]] bool is_valid() const {
        return m_x >= 0 && m_y >= 0;
    }

};

 void main_discard() {
     Point point;

     /*auto ret = */point.is_valid();     // warning: ignoring return value

     /*int x = */ printf("ABC");

}
