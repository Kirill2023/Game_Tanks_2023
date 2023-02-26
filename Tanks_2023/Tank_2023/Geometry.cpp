#include "Geometry.h"





        Geometry::Geometry()
        {
            ref = glm::vec3(0, 0, 0);
            location = ref;

            direction = RIGHT;

            get_mat();


        }







        double Geometry::angle()
        {
            return M_PI * (direction) / 2;
        }

        bool Geometry::operator == (DIR _Dir)
        {
            return direction == _Dir;
        }

        bool Geometry::operator != (DIR _Dir)
        {
            return direction != _Dir;
        }

        void Geometry::Right()
        {
            direction = DIR::RIGHT;
        }

        void Geometry::Up()
        {
            direction = DIR::UP;
        }

        void Geometry::Left()
        {
            direction = DIR::LEFT;
        }

        void Geometry::Down()
        {
            direction = DIR::DOWN;
        }

        void Geometry::Turn_Right()
        {
            direction = 0b11 & (direction - 1);
        }

        void Geometry::Turn_Left()
        {
            direction = 0b11 & (direction + 1);
        }

        void Geometry::Turn_Back()
        {
            direction = 0b11 & (direction + 2);
        }

        void Geometry::rand()
        {
            direction = std::rand() % 0b100;
        }



        Geometry& Geometry::operator+=(const glm::vec3& p)
        {
            location += p;
            return *this;
        }

        const Geometry Geometry::operator+(const glm::vec3& p) const
        {
            return Geometry(*this) += p;
        }

        Geometry& Geometry::operator-=(const glm::vec3& p)
        {
            location -= p;
            return *this;
        }

        const Geometry Geometry::operator-(const glm::vec3& p) const
        {
            return Geometry(*this) -= p;
        }





        glm::mat4 Geometry::set_orientation(GLfloat angle, glm::vec3 axis)
        {
            return (mat_orientation = glm::rotate(glm::mat4(1), angle, axis));
        }



        glm::mat4  Geometry::set_location(glm::vec3 location)
        {
            return (mat_location = glm::translate(glm::mat4(1), location));
        }


        glm::mat4 Geometry::get_mat()
        {
            return (mat = set_location(location) * set_orientation(angle(), glm::vec3(0, 0, 1)));
        }
