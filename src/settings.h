#pragma once

class ImColor;

namespace Settings {
	namespace ESP {
		extern bool enabled;
		extern bool drawTeam;
		extern ImColor* teamColor;
		extern ImColor* enemyColor;

	}
	namespace Aimbot {
		extern bool enabled;
		extern bool smoothing;
		extern float smoothingAmount;
		extern bool checkInFOV;
		extern float fov;
		extern bool drawFovCircle;
	}
}