#include <DDECS.h>
#include <Asset.h>
#include <iostream>
#include <IO_API/IO_API.h>

class SystemDisplay : public System {
	ComponentID textureComponentID = -1;
	ComponentID bodyComponentID = -1;
	ComponentID cameraComponentID = -1;
public:
	virtual void init() {
		textureComponentID = ecs.registerComponent("texture", sizeof(TextureID));
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		cameraComponentID = ecs.registerComponent("camera", NULL);
	}
	virtual void run() {
		clock_t startTime = clock();

		Vec2D<int32_t> camPos = { 0, 0 };
		if (ecs.getComponentCount(cameraComponentID)) {
			EntityID entity = ecs.getOwner(cameraComponentID, 0);
			if (ecs.entityHasComponent(entity, bodyComponentID)) {
				BodyID bodyID = ecs.getEntityComponentAs<BodyID>(entity, bodyComponentID);
				camPos = physics.getPos<int32_t>(bodyID);
			}
		}
		unsigned int winWidth, winHeight;
		EE_getCanvasSize(&winWidth, &winHeight);
		int cx = -(camPos.x-winWidth / 2), cy = -(camPos.y-winHeight/2);

		uint32_t texCount = ecs.getComponentCount(textureComponentID);
		for (uint32_t i = 0; i < texCount; i++) {
			EntityID entity = ecs.getOwner(textureComponentID, i);
			if (ecs.entityHasComponent(entity, bodyComponentID) == false)
				continue;
			BodyID bodyID = ecs.getEntityComponentAs<BodyID>(entity, bodyComponentID);
			auto pos = physics.getPos<int32_t>(bodyID);
			auto siz = physics.getSize<int32_t>(bodyID);
			TextureID texID = ecs.getEntityComponentAs<TextureID>(entity, textureComponentID);
			EE_resizeTexture(TextureCodex::get(texID), siz.x, siz.y);  //TODO: rework this
			EE_drawTexture(TextureCodex::get(texID), pos.x+cx, pos.y+cy);
		}

		ms = clock() - startTime;
	}
	virtual const char* getName() {
		return "SystemDisplay";
	}
};