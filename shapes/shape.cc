#include "shape.hh"

#include <cstring>


shape::shape(int id, int sides, float radius)
 : m_id(id), m_sides(sides), m_radius(radius), m_attach(0)
{
    if (m_sides > 0) {
        if ((m_attach = new int[m_sides])) {
            for (int i = 0; i < m_sides; ++i)
                m_attach[i] = ' ';
        }
    }
}

shape::~shape()
{
    delete m_attach;
}

int shape::set_attach(const char* str)
{
    int l = strlen(str);

    if (l < 0 || l > m_sides)
        return -1;

    for (int i = 0; i < l; ++i)
        m_attach[i] = str[i];

    for (int i = l; i < m_sides; ++i)
        m_attach[i] = ' ';

    return l;
}
