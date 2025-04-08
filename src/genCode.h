#pragma once
class Vector3 {
public:
	float x;
	float z;
	float y;
private:

};



class Player
{
public:
	char pad_0004[12]; //
	Vector3 speed; //0x0010
	char pad_001C[12]; //0x001C
	Vector3 posision; //0x0028
	Vector3 vision; //0x0034
	char pad_0040[136]; //0x0040

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x00C8


class EntityList {
public:
	Player* players[64]; //0x0000
};