#pragma once


namespace fldr {
    class Triangle{
        public:
        Triangle(float x, float y, float z);
        float x()const;
        float y()const;
        float z()const;
        private:
        float m_x, m_y, m_z;
    };
}