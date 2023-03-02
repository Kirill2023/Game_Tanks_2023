


#include "Object.h"

namespace tank
{









    Registry& getRegistry()
    {
        static Registry _instance;
        return _instance;
    }

















    pObject Object::createObject(Type_ID _type, SubType_ID _subtype)
    {
        uint16_t id = (static_cast<uint16_t>(_type) << 8) | static_cast<uint16_t>(_subtype);
        Registry& r = getRegistry();

        if (r.find(id) != r.end())
            return r[id]->clone();
        return nullptr;
    }

    // ƒобавление прототипа в множество прототипов
    void Object::addPrototype(SubType_ID _subtype, pObject prototype)
    {
        prototype->subtype = _subtype;
        uint16_t id = (static_cast<uint16_t>(prototype->type) << 8) | static_cast<uint16_t>(prototype->subtype);
        Registry& r = getRegistry();
        r[id] = prototype;
    }

    // ”даление прототипа из множества прототипов
    void Object::removePrototype(Type_ID _type, SubType_ID _subtype) {
        uint16_t id = (static_cast<uint16_t>(_type) << 8) | static_cast<uint16_t>(_subtype);
        Registry& r = getRegistry();
        r.erase(r.find(id));
    }

    void Object::Collision_test(vpObject& obj1, vpObject& obj2) // ќбнаружение столкновени€
    {
        if (&obj1 == &obj2)
            for (int x = 0; x + 1 < obj1.size(); ++x)
                for (int y = x + 1; y < obj2.size(); ++y)
                    Object::Collision_test(*(obj1[x]), *(obj2[y]));
        else
            for (auto& x : obj1)
                for (auto& y : obj2)
                    Object::Collision_test(*x, *y);
    }









    void Object::set_Ref(vec3 r)
    {
        geometry.ref = r;
        geometry.location = r;
    }

    vec3 Object::get_Ref()
    {
        return geometry.ref;
    }

    void Object::set_Direction(int r)
    {
        geometry.direction = r;
    }

    int Object::get_Direction()
    {
        return geometry.direction;
    }



    void Object::make(CMD _cmd )
    {
        static int t = 0;

        if (state == STATE::Normal)
            cmd = _cmd;

        ++t;
    }

    void Object::ClearNodes()
    {
        for (auto& x : node)
            if (x->state == STATE::NotNeeded)
            {
                if (x->node.size())
                {
                    for (auto& xx : x->node)
                        if (xx->state != STATE::Delete)
                            node.push_back(xx);
                    x->node.clear();
                }
                x->state = STATE::Delete;
            }
    }




    void Object::Tick(int64_t _countr) //  выполнение
    {
        

        for (auto& x : node)
            x->Tick(_countr);



        //if (state == STATE::Normal)
        //    switch (cmd)
        //    {
        //    case CMD::nop: break;
        //    case CMD::move_Straight: break;
        //    case CMD::move_Up: Move_Up();  break;
        //    case CMD::move_Right: Move_Right();  break;
        //    case CMD::move_Left: Move_Left(); break;
        //    case CMD::move_Down: Move_Down();  break;
        //    case CMD::turn_Right: break;
        //    case CMD::turn_Left: break;
        //    case CMD::turn_Back: break;
        //    case CMD::autoPlayOn: autoPlay = true;  break;
        //    case CMD::autoPlayOff: autoPlay = false;  break;
        //    case CMD::shot: Shot(); break;
        //    }
        //else
        //    if (state == STATE::Fail)
        //        Fail();

        if (autoPlay)
            AutoPlay();

        ++countr;
    }

    Object::~Object() {};

    void Object::Revival()
    {
        Tank::_vpObject.push_back(Object::createObject(Type_ID::Tank, SubType_ID::middle));
        Tank::_vpObject.back()->set_Direction(get_Direction());
        Tank::_vpObject.back()->set_Ref(get_Ref());
        Tank::_vpObject.back()->ancestor_id = id;
        Tank::_vpObject.back()->lives = lives;
    }

    int Object::height()
    {
        return 2;
    }


    void Object::Fail()
    {
        if (geometry.location.z < (-height()))
            state = STATE::NotNeeded;
        else
            geometry -= geometry.step_z * glm::vec3(speed, speed, speed);
    }


    void Object::Move_Straight()
    {
        switch (get_Direction())
        {
        case Geometry::RIGHT: Move_Right(); break;
        case Geometry::UP: Move_Up(); break;
        case Geometry::LEFT: Move_Left(); break;
        case Geometry::DOWN: Move_Down(); break;
        }
    }

    void Object::Move_Up()
    {
        if (geometry != Geometry::UP)
            geometry.Up();
        else
            if (collision)
            {
                collision = false;
                geometry -= geometry.step_y * glm::vec3(speed, speed, speed);
            }
            else
                geometry += geometry.step_y * glm::vec3(speed, speed, speed);
    }

    void Object::Move_Right()
    {
        if (geometry != Geometry::RIGHT)
            geometry.Right();
        else
            if (collision)
            {
                collision = false;
                geometry -= geometry.step_x * glm::vec3(speed, speed, speed);
            }
            else
                geometry += geometry.step_x * glm::vec3(speed, speed, speed);
    }

    void Object::Move_Left()
    {
        if (geometry != Geometry::LEFT)
            geometry.Left();
        else
            if (collision)
            {
                collision = false;
                geometry += geometry.step_x * glm::vec3(speed, speed, speed);
            }
            else
                geometry -= geometry.step_x * glm::vec3(speed, speed, speed);
    }

    void Object::Move_Down()
    {
        if (geometry != Geometry::DOWN)
            geometry.Down();
        else
            if (collision)
            {
                collision = false;
                geometry += geometry.step_y * glm::vec3(speed, speed, speed);
            }
            else
                geometry -= geometry.step_y * glm::vec3(speed, speed, speed);
    }



    void Object::Interact(Object& obj)
    {
        if (state == STATE::Normal)
        {
            collision = true;
            strength -= obj.power;
            if (strength <= 0)
                state = STATE::Fail;
        }
    }


    inline bool test(vec2 A, vec2 C, vec2 W)
    {
        return (A.x <= W.x) && (W.x <= C.x) && (A.y <= W.y) && (W.y <= C.y);
    }



    bool Object::Collision_test(Object& obj1, Object& obj2)
    {
        vec2 A1, A2, C1, C2;

        A1 = obj1.geometry.get_Contour()[0];
        C1 = obj1.geometry.get_Contour()[1];
        A2 = obj2.geometry.get_Contour()[0];
        C2 = obj2.geometry.get_Contour()[1];

        bool c = !((obj1.id == obj2.ancestor_id) || (obj2.id == obj1.ancestor_id));

        c &=
            test(A1, C1, A2) ||
            test(A1, C1, C2) ||
            test(A1, C1, vec2(A2.x, C2.y)) ||
            test(A2, C2, A1) ||
            test(A2, C2, C1) ||
            test(A2, C2, vec2(A1.x, C1.y));


        if (c)
        {
            obj1.Interact(obj2);
            obj2.Interact(obj1);
        }

        return c;
    }


    void Object::Draw( const glm::mat4& view, const glm::mat4& projection)
    {
        OGL::Draw(*(geometry.mesh.GetVAO()), geometry.mesh.GetTEX(), geometry.get_mat(), view, projection);

        //OGL::Draw(*(geometry.mesh.GetVAO()), geometry.get_mat(), view, projection, geometry.mesh.Type, geometry.mesh.color);

        for (auto& x : node)
            x->Draw( view, projection);
    }


    void Bullet::AutoPlay()
    {

        if (state == STATE::Normal)
        {
            if (collision)
            {
                Move_Straight();
                geometry.rand();
            }
            else
                Move_Straight();


        }

        if (state == STATE::Fail)
            Fail();

        if (state == STATE::NotNeeded)
            state = STATE::Delete;

    }


    void Tank::AutoPlay()
    {

        if (state == STATE::Normal)
        {
            if (collision)
            {
                Move_Straight();
                geometry.rand();
            }
            else
                Move_Straight();

            if (!(countr % 100))
                if (!(rand() % 3))
                    Shot();

            if (!(countr % 100))
                if (!(rand() % 5))
                    geometry.rand();
        }

        if (state == STATE::Fail)
            Fail();

        if (state == STATE::NotNeeded)
        {
            if (lives < 1)
                ;
            else
            {
                --lives;
                Revival();
            }
            state = STATE::Delete;
        }
    }

    void Barrier::AutoPlay()
    {
        if (state == STATE::Fail)
            Fail();

        if (state == STATE::NotNeeded)
            state = STATE::Delete;
    }





    void Tank::Shot()
    {
        Bullet::_vpObject.push_back(Object::createObject(Type_ID::Bullet, SubType_ID::middle));
        Bullet::_vpObject.back()->set_Direction(get_Direction());
        Bullet::_vpObject.back()->geometry.location = geometry.location;
        Bullet::_vpObject.back()->ancestor_id = id;
    }





    Barrier Barrier::Prototype = Barrier();
    Tank Tank::Prototype = Tank();
    Bullet Bullet::Prototype = Bullet();

    vpObject Barrier::_vpObject;
    vpObject Tank::_vpObject;
    vpObject Bullet::_vpObject;


}