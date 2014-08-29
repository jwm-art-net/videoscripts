

class shape
{
  public:
    shape(int id, int sides, float radius);
    ~shape();

    // returns 0 on success:
    int set_attach(const char* str);

    int get_id() { return m_id; }
    int get_sides() { return m_sides; }
    float get_radius() { return m_radius; }
    const int* get_attach() { return m_attach; }

  private:
    int     m_id;
    int     m_sides;
    float   m_radius;
    int*    m_attach;
};



