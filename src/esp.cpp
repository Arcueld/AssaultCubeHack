#include "esp.h"
#include "constants.h"
#include <iostream>
#include "settings.h"
#include "imgui/imgui.h"

const float FOV = 90.0f; // Field of view in degrees



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

void drawCenteredLine(std::string text, float x, float y) {
	float textWidth = ImGui::CalcTextSize(text.c_str()).x;
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - textWidth / 2, y), IM_COL32(255, 255, 255, 255), text.c_str());
}

void drawScalingBar(float x1, float y1, float x2, float y2, float width, float value, float max, ImColor color) {
	float heightDiff = y2 - y1;
	float scaledHeight = heightDiff * (value / max);
	
	ImGui::GetBackgroundDrawList()->AddRect(
		ImVec2(x1, y1),
		ImVec2(x2, y2),
		IM_COL32(255, 255, 255, 255),
		0, 0, width
	);

	ImGui::GetBackgroundDrawList()->AddRectFilled(
		ImVec2(x1, y1), 
		ImVec2(x2, y1 + scaledHeight), 
		color
	);
}

void ESP::drawESP(){
	if (!Settings::ESP::enabled) return;

	bool isTeammate = false;
	EntityList* pEntityList = *ppEntityList;
	for (int i = 1; i < playerNums + 1; i++) {
		Player* player = pEntityList->players[i];
		if (player->HP <=0 || player->HP > 100) continue; // pass died players

		isTeammate = player->team == pPlayer->team;
		if (!Settings::ESP::drawTeam && isTeammate) continue; // do not draw teammates
		
		vec3 headPos = { player->position.x,player->position.y,player->position.z + player->eyeHeight };
		vec3 feetPos = player->position;

		vec3 headScreenPos = OpenGLWorldToScreen(headPos, viewMatrix, 1024, 860);
		vec3 feetScreenPos = OpenGLWorldToScreen(feetPos, viewMatrix, 1024, 860);


		float height = abs(headScreenPos.y - feetScreenPos.y);
		float width = height ;

		ImVec2 topLeft = ImVec2(headScreenPos.x - width / 4, headScreenPos.y);
		ImVec2 topRight = ImVec2(headScreenPos.x + width / 4, headScreenPos.y);
		ImVec2 bottomLeft = ImVec2(feetScreenPos.x - width / 4, feetScreenPos.y);
		ImVec2 bottomRight = ImVec2(feetScreenPos.x + width / 4, feetScreenPos.y);

		ImColor espColor = isTeammate? *Settings::ESP::teamColor : *Settings::ESP::enemyColor;
	
		ImGui::GetBackgroundDrawList()->AddQuad(topLeft, bottomLeft, bottomRight, topRight, espColor, 1.0f);
	
		drawCenteredLine(player->name, headScreenPos.x, headScreenPos.y - height / 2);
		drawScalingBar(bottomLeft.x - 5 - width / 10, bottomLeft.y, bottomLeft.x -5, topRight.y,1, player->HP, 100.0f, ImColor(0,255,0,255));

	}

}
