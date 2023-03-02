#pragma once


#include <algorithm>
#include <typeinfo>
#include <cmath>
#include <iomanip>
#include <corecrt_math_defines.h>
#include <sstream>
#include <string>

#include <iostream>
#include <ppl.h>
#include <map>

#include "OGL.h"

#include "Geometry.h"



namespace tank
{
    class Object;

    using pObject = std::shared_ptr<Object>;
    using vpObject = std::vector< pObject>;


    enum  class  Type_ID : uint8_t
    {
        non = 0,

        Point,
        Vector3d,
        Object,
        Barrier,
        Tank,
        Bullet,

        end = 255
    };

    enum class SubType_ID : uint8_t
    {
        non = 0,
        field,
        brick,
        concrete,
        forest,
        easy,
        middle,
        heavy,
        Wall_H,
        Wall_V,

        end = 255
    };




    class Object;
    typedef std::map<uint16_t, pObject> Registry;

    // Реестр прототипов определен в виде Singleton Мэйерса
    Registry& getRegistry();


    static int64_t Get_ID()
    {
        static int64_t id = 0;
        return ++id;
    }



    class Object
    {
    public:

        static pObject createObject(Type_ID _type, SubType_ID _subtype);

        static void addPrototype(SubType_ID _subtype, pObject prototype);  // Добавление прототипа в множество прототипов
     
        static void removePrototype(Type_ID _type, SubType_ID _subtype);   // Удаление прототипа из множества прототипов

        static void Collision_test(vpObject& obj1, vpObject& obj2); // Обнаружение столкновения


        class Event;

        int countr = 0;

        Type_ID type;
        SubType_ID subtype;
        std::string name;

        bool autoPlay;
        bool collision;
        bool modified;

        int64_t id = 0;
        int64_t ancestor_id = 0;
        int lives;
        int speed;  //  множитель скорости
        int power; // сила наносимого столкновения/удара
        int strength; // устойчевость к столкновению/удару        

        Geometry geometry;
        vpObject node;

        //int max_speed;  //  максимальный множитель скорости
        //int acceleration;


        enum class CMD : uint8_t
        {
            nop = 0,
            move_Straight,
            move_Up,
            move_Right,
            move_Left,
            move_Down,
            turn_Right,
            turn_Left,
            turn_Back,
            autoPlayOn,
            autoPlayOff,
            shot,
            fire,
            resume
        } cmd;

        enum  class STATE : uint8_t
        {
            Normal = 0,
            Fail,
            NotNeeded,
            Delete
        } state;


        

        virtual void set_Ref(vec3 r);

        virtual vec3 get_Ref();

        virtual void set_Direction(int r);

        virtual int get_Direction();

        virtual ~Object() = 0;

        virtual pObject clone() = 0;

        virtual vpObject& get_vpObject() = 0;

        virtual void AutoPlay() = 0; // Автоплей

        virtual void make(CMD _cmd = CMD::nop); // Выполнить команду

        virtual void ClearNodes(); // Чистка дерева

        virtual void Tick(int64_t countr); // Сделать такт

        virtual void Revival(); // Возродить

        virtual int height(); // Высота объекта

        virtual void Fail(); // Проваливаться

        virtual void Move_Straight(); // Движение вперед

        virtual void Move_Up(); // Движение вверх

        virtual void Move_Right(); // Движение вправо

        virtual void Move_Left(); // Движение влево

        virtual void Move_Down(); // Движение вниз

        virtual void Interact(Object& obj); // Взоимедействовать с объектом

        static bool Collision_test(Object& obj1, Object& obj2); // Обнаружение столкновения

        virtual void Draw(const glm::mat4& view, const glm::mat4& projection); // Рисовать

        virtual void SetColor(glm::vec4 color)
        {
            geometry.mesh.color = color;
        }
        
    };





    class Barrier final : public Object
    {

        pObject clone()
        {
            auto cl = std::make_shared<Barrier>(Barrier(*this));
            cl->id = Get_ID();
            return cl;
        }


    private:
        Barrier()
        {
            type = Type_ID::Barrier;
            id = Get_ID();
            SetColor(glm::vec4(0.5f, 0.7f, 0.1f, 0.3f));
            Object::addPrototype(SubType_ID::non, std::make_shared<Barrier>(*this));
        }

        static Barrier Prototype;


    public:
        static vpObject _vpObject;

        vpObject& get_vpObject()
        {
            return _vpObject;
        }



        static void Draw(const glm::mat4& view, const glm::mat4& projection) // Рисовать
        {
            for (auto& x : _vpObject)
                x->Draw(view, projection);
        }

        static void Tick(int64_t _countr) // 
        {
            for (int i = 0; i < _vpObject.size(); ++i)
                _vpObject[i]->Tick(_countr);
        }

        void AutoPlay();

        static void Clearing() // 
        {
            auto i = std::remove_if(_vpObject.begin(), _vpObject.end(),
                [](auto& x) { return x->state == STATE::Delete; });

            _vpObject.erase(i, _vpObject.end());
        }

    };



    class Tank final : public Object
    {
        

        pObject clone()
        {
            auto cl = std::make_shared<Tank>(Tank(*this));
            cl->id = Get_ID();
            return cl;
        }


    private:
        Tank()
        {
            type = Type_ID::Tank;
            id = Get_ID();
            SetColor(glm::vec4(0.3f, 0.3f, 0.3f, 0.3f));
            Object::addPrototype(SubType_ID::non, std::make_shared<Tank>(*this));
        }




        static Tank Prototype;


    public:
        static vpObject _vpObject;

        vpObject& get_vpObject()
        {
            return _vpObject;
        }




        static void Draw(const glm::mat4& view, const glm::mat4& projection) // Рисовать
        {
            for (auto& x : _vpObject)
                x->Draw(view, projection);
        }

        static void Tick(int64_t _countr) // 
        {
            for(int i =0; i< _vpObject.size();++i)
                _vpObject[i]->Tick(_countr);
        }

        static void Clearing() // 
        {
            auto i = std::remove_if(_vpObject.begin(), _vpObject.end(),
                [](auto& x) {return x->state == STATE::Delete; });

            _vpObject.erase(i, _vpObject.end());
        }


        void AutoPlay();

        void Shot(); // Сделать выстрел

    };


    class Bullet final : public Object
    {


        pObject clone()
        {
            auto cl = std::make_shared<Bullet>(Bullet(*this));
            cl->id = Get_ID();
            return cl;
        }


    private:
        Bullet()
        {
            type = Type_ID::Bullet;
            id = Get_ID();
            SetColor(glm::vec4(0.7f, 0.7f, 0.7f, 0.7f));
            Object::addPrototype(SubType_ID::non, std::make_shared<Bullet>(*this));
        }




        static Bullet Prototype;

    public:

        static vpObject _vpObject;

        vpObject& get_vpObject()
        {
            return _vpObject;
        }



        static void Draw(const glm::mat4& view, const glm::mat4& projection) // Рисовать
        {
            for (auto& x : _vpObject)
                x->Draw(view, projection);
        }

        static void Tick(int64_t _countr) // 
        {
            for (int i = 0; i < _vpObject.size(); ++i)
                _vpObject[i]->Tick(_countr);
        }

        static void Clearing() // 
        {
            auto i = std::remove_if(_vpObject.begin(), _vpObject.end(),
                [](auto& x) {return x->state == STATE::Delete; });

            _vpObject.erase(i, _vpObject.end());
        }


        void AutoPlay();
    };






}

