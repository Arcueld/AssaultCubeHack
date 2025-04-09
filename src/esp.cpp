#include "esp.h"
#include "constants.h"
#include <iostream>

const float FOV = 90.0f; // Field of view in degrees


//bool isInFOV(Player* owner,vec3 looking) {
//	vec3 angle = CalcAngle(owner->position , looking);
//	vec3 playerAngle(owner->vision.x + 90, owner->vision.y, 0.0f);
//	playerAngle.NormalizeAngle();
//	
//	vec3 delta = playerAngle - angle;
//	delta.NormalizeAngle();
//
//
//	return (fabs(delta.x) <= FOV / 2 && fabs(delta.y) <= FOV / 2);
//}


bool isInFOV(Player* owner, vec3 targetPos) {
	vec3 angleToTarget = CalcAngle(owner->position, targetPos);
	vec3 playerAngle(owner->vision.x - 90.0f, owner->vision.y, 0.0f);  

	playerAngle.NormalizeAngle();

	vec3 delta = playerAngle - angleToTarget;
	// delta.NormalizeAngle();
	if (delta.x > 180) delta.x -= 360;
	if (delta.x < -180) delta.x += 360;
	if (delta.y > 180) delta.y -= 360;
	if (delta.y < -180) delta.y += 360;

	return (fabs(delta.x) <= FOV / 2 && fabs(delta.y) <= FOV / 2);
}




bool isValidTarget(Player* player) {
	if (player == nullptr || player == pPlayer)
		return false;
	if (player->HP <= 0 || player->HP > 100)
		return false;
	if (!isInFOV(pPlayer, player->position)) {
	
		return false;
	}
	return true;
}



Player* ESP::getNearestEntityAngle() {
	EntityList* pEntityList = *ppEntityList;

	vec3 playerAngle(pPlayer->vision.x + 90, pPlayer->vision.y, 0.0f);
	playerAngle.NormalizeAngle();

	Player* nearestPlayer = nullptr;
	float smallestAngle = 99999999.0f;

	for (int i = 1; i < playerNums + 1; i++) {
		Player* player = pEntityList->players[i];
		if (!isValidTarget(player)) continue;

		vec3 targetAngle = CalcAngle(pPlayer->position, player->position);
		vec3 delta = playerAngle - targetAngle;
		delta.NormalizeAngle();
		float angleMagnitude = delta.Length();

		if (angleMagnitude < smallestAngle) {
			smallestAngle = angleMagnitude;
			nearestPlayer = player;
		}
	}
	return nearestPlayer;
}


Player* ESP::getNearestPlayer() {
	EntityList* pEntityList = *ppEntityList;

	Player* nearestPlayer = nullptr;
	float nearestDistance = 99999999.0f;


	for (int i = 1; i < playerNums + 1; i++) {
		Player* player = pEntityList->players[i];
		if (!isValidTarget(player)) continue;
		
		float distance = pPlayer->position.Distance(player->position);
		if (distance < nearestDistance) {
			nearestDistance = distance;
			nearestPlayer = player;
		}
	}

	return nearestPlayer;
}

void ESP::aimbot() {
	if (!GetAsyncKeyState(VK_SHIFT))
		return;

	Player* target = getNearestPlayer();
	if (target == nullptr)
		return;
	if (pPlayer->team == target->team) return;

	vec3 angle = CalcAngle(pPlayer->position, target->position);
	angle.x += 90;
	angle.NormalizeAngle();


	pPlayer->vision.x = angle.x;
	pPlayer->vision.y = angle.y;
}