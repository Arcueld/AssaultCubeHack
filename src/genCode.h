#pragma once
#include <cstdint>
#include "geom.h"



class Player
{
public:
	Vector3 o; //0x0004
	Vector3 speed; //0x0010
	char pad_001C[12]; //0x001C
	Vector3 position; //0x0028
	Vector3 vision; //0x0034
	char pad_0040[16]; //0x0040
	float eyeHeight; //0x0050
	float N000009AB; //0x0054
	char pad_0058[148]; //0x0058
	int32_t HP; //0x00EC
	int32_t Armor; //0x00F0
	char pad_00F4[20]; //0x00F4
	int32_t weapon2_ReserveAmmo; //0x0108
	char pad_010C[16]; //0x010C
	int32_t weapon1_ReserveAmmo; //0x011C
	char pad_0120[12]; //0x0120
	int32_t weapon2_CurrentAmmo; //0x012C
	char pad_0130[16]; //0x0130
	int32_t weapon1_CurrentAmmo; //0x0140
	int32_t booms; //0x0144
	char pad_0148[148]; //0x0148
	int32_t kills; //0x01DC
	char pad_01E0[37]; //0x01E0
	char name[60]; //0x0205
	char pad_0241[203]; //0x0241
	int32_t team; //0x030C
	char pad_0310[16]; //0x0310

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
}; //Size: 0x0320










class EntityList {
public:
	Player* players[64]; //0x0000
};