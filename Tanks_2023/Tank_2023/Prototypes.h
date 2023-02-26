#pragma once




#include "Object.h"

#include "Data.h"




using namespace tank;




class Prototypes
{
	friend class tank::Object;

public:

	static void Init()
	{
		

		Registry& r = getRegistry();

		pObject obj;

		obj = Object::createObject(Type_ID::Barrier, SubType_ID::non);
		obj->name = "";
		obj->speed = 10;  //  множитель скорости
		obj->lives = 3;
		obj->power = 100; // сила наносимого столкновения/удара
		obj->strength = 3000; // устойчевость к столкновению/удару
		obj->geometry.mesh.Set_map_textur("Path/To/Textures/Field.bmp");
		obj->geometry.mesh.SetVTexture(textur);
		obj->geometry.mesh.SetVertices(vField);
		obj->geometry.contour[0] = vec2{ -10.5, -10.5 };
		obj->geometry.contour[1] = vec2{ -10.5,  -10.5 };
		obj->SetColor(glm::vec4(0.35f, 0.5f, 0.25f, 0.8f));
		Object::addPrototype(SubType_ID::field, obj);



		obj = Object::createObject(Type_ID::Barrier, SubType_ID::non);
		obj->name = "";
		obj->speed = 10;  //  множитель скорости
		obj->lives = 3;
		obj->power = 100; // сила наносимого столкновения/удара
		obj->strength = 30000; // устойчевость к столкновению/удару
		obj->geometry.mesh.Set_map_textur("Path/To/Textures/Wall.bmp");
		obj->geometry.mesh.SetVTexture(textur);
		obj->geometry.mesh.SetVertices(vWall_H);
		obj->geometry.contour[0] = vec2{ -10, -0.2 };
		obj->geometry.contour[1] = vec2{ 10,  0.2 };
		obj->SetColor(glm::vec4(0.4f, 0.4f, 0.2f, 0.8f));
		Object::addPrototype(SubType_ID::Wall_H, obj);




		obj = Object::createObject(Type_ID::Barrier, SubType_ID::non);
		obj->name = "";
		obj->speed = 10;  //  множитель скорости
		obj->lives = 3;
		obj->power = 100; // сила наносимого столкновения/удара
		obj->strength = 30000; // устойчевость к столкновению/удару
		obj->geometry.mesh.Set_map_textur("Path/To/Textures/Wall.bmp");
		obj->geometry.mesh.SetVTexture(textur);
		obj->geometry.mesh.SetVertices(vWall_V);
		obj->geometry.contour[0] = vec2{-0.2, -10  };
		obj->geometry.contour[1] = vec2{0.2 , 10  };
		obj->SetColor(glm::vec4(0.4f, 0.4f, 0.2f, 0.8f));
		Object::addPrototype(SubType_ID::Wall_V, obj);



		obj = Object::createObject(Type_ID::Barrier, SubType_ID::non);
		obj->name = "";
		obj->speed = 50;  //  множитель скорости
		obj->lives = 3;
		obj->power = 100; // сила наносимого столкновения/удара
		obj->strength = 3000; // устойчевость к столкновению/удару
		obj->autoPlay = true;
		obj->geometry.mesh.Set_map_textur("Path/To/Textures/Brick.bmp");
		obj->geometry.mesh.SetVTexture(textur);
		obj->geometry.mesh.SetVertices(vBarrier);
		obj->geometry.contour[0] = vec2{ -0.5, -0.5 };
		obj->geometry.contour[1] = vec2{  0.5,  0.5 };
		obj->SetColor(glm::vec4(0.5f, 0.35f, 0.2f, 0.7f));
		Object::addPrototype(SubType_ID::brick, obj);


		obj = Object::createObject(Type_ID::Tank, SubType_ID::non);
		obj->name = "";
		obj->speed = 50;  //  множитель скорости
		obj->lives = 3;
		obj->power = 1000; // сила наносимого столкновения/удара
		obj->strength = 3000; // устойчевость к столкновению/удару
		obj->autoPlay = true;
		obj->geometry.mesh.Set_map_textur("Path/To/Textures/Tank.bmp");
		obj->geometry.mesh.SetVTexture(textur);
		obj->geometry.mesh.SetVertices(vTank_M);
		obj->geometry.contour[0] = vec2{ -0.4, -0.4 };
		obj->geometry.contour[1] = vec2{  0.4,  0.4 };
		obj->SetColor(glm::vec4(0.3f, 0.3f, 0.3f, 0.7f));
		Object::addPrototype(SubType_ID::middle, obj);
		


		obj = Object::createObject(Type_ID::Bullet, SubType_ID::non);
		obj->name = "";
		obj->speed = 300;  //  множитель скорости
		obj->lives = 3;
		obj->power = 1000; // сила наносимого столкновения/удара
		obj->strength = 200; // устойчевость к столкновению/удару
		obj->autoPlay = true;
		obj->geometry.mesh.Set_map_textur("Path/To/Textures/Bullet.bmp");
		obj->geometry.mesh.SetVTexture(textur);
		obj->geometry.mesh.SetVertices(vBullet);
		obj->geometry.contour[0] = vec2{ -0.1, -0.1 };
		obj->geometry.contour[1] = vec2{  0.1,  0.1 };

		Object::addPrototype(SubType_ID::middle, obj);







	}
















};

