#pragma once

#include "perplex_defs.h"
#include "perplex_bool.h"
#include "perplex_math.h"
#include "perplex_pixel.h"
#include "perplex_renderer.h"

#include <stdint.h>

#define PX_MEMBER_POINTER_SYMBOL(component, member) unit.AddSymbol(#component"_Get"#member"Ptr", +[](Entity entity) { return entity.HasComponent<component>() ? &entity.GetComponent<component>().member : NULL; });

typedef unsigned long long UUID;
typedef unsigned long long PrefabAsset;
typedef unsigned long long SceneAsset;

typedef struct Scene
{

} Scene;

typedef struct Entity
{
	int EntityHandler;
	Scene* ScenePtr;
} Entity;

PX_EXTERN Scene* scene;
PX_EXTERN Entity self;

// ENTITY

PX_EXTERN bool Entity_HasTag(Entity entity, const char* tag);
PX_EXTERN void Entity_GetTag(Entity entity, char* buf, size_t size);
PX_EXTERN void Entity_SetEnabled(Entity entity, bool enabled);

PX_EXTERN Vec3* Entity_GetPositionPtr(Entity entity);
#define position_of(entity) (*Entity_GetPositionPtr(entity))
#define position (*Entity_GetPositionPtr(self))

PX_EXTERN Vec3* Entity_GetRotationPtr(Entity entity);
#define rotation_of(entity) (*Entity_GetRotationPtr(entity))
#define rotation (*Entity_GetRotationPtr(self))

PX_EXTERN Vec3* Entity_GetScalePtr(Entity entity);
#define scale_of(entity) (*Entity_GetScalePtr(entity))
#define scale (*Entity_GetScalePtr(self))

PX_EXTERN void Entity_ToPerpixel(Entity entity);

PX_EXTERN void Entity_Destroy(Entity entity);
PX_EXTERN void Entity_DestroyDelay(Entity entity, float delay);

// SCENE

PX_EXTERN void Scene_InvokeEvent(Scene* scene, const char* eventName, void* data);
#define event(eventName, data) (Scene_InvokeEvent(scene, eventName, data))

PX_EXTERN void Scene_SetPaused(Scene* scene, bool paused);
#define set_paused(paused) (Scene_SetPaused(scene, paused))

PX_EXTERN void Scene_Pause(Scene* scene);
#define pause() (Scene_Pause(scene))

PX_EXTERN void Scene_Resume(Scene* scene);
#define resume() (Scene_Resume(scene))

PX_EXTERN Entity Scene_Spawn(Scene* scene, PrefabAsset prefab);
#define spawn(prefab) (Scene_Spawn(scene, prefab))

PX_EXTERN float Scene_GetTime(Scene* scene);
#define get_time (Scene_GetTime(scene))

PX_EXTERN void Scene_CameraShake(Scene* scene, float trauma);
#define camera_shake(trauma) (Scene_CameraShake(scene, trauma))

// DEBUG

PX_EXTERN void Trace(char* msg);
PX_EXTERN void Info(char* msg);
PX_EXTERN void Warn(char* msg);
PX_EXTERN void Error(char* msg);

// MATH

PX_EXTERN float Degrees(float rad);
PX_EXTERN float Radians(float deg);

// COMPONENTS

PX_EXTERN float Sprite_GetWidth(Entity entity);
PX_EXTERN float Sprite_GetHeight(Entity entity);

PX_EXTERN Color* Sprite_GetColorPtr(Entity entity);
#define spriteColor (*Sprite_GetColorPtr(self))

PX_EXTERN void Script_TryCall(Entity entity, char* funcName);
PX_EXTERN void Perpixel_SpawnPixel(Entity entity, Pixel pixel);

PX_EXTERN void PhysicsBody_SetVelocity(Entity entity, Vec2 velocity);

// TODO: PX_EXTERN void Entity_PerpixelSpawnPixel(Entity entity);

typedef void* Sound;
PX_EXTERN void Scene_PlaySound(Scene* scene, const char* filepath);
#define play_sound(filepath) (Scene_PlaySound(scene, filepath))

PX_EXTERN Sound Scene_StartLoop(Scene* scene, const char* filepath);
#define start_loop(filepath) (Scene_StartLoop(scene, filepath))

PX_EXTERN void Scene_EndLoop(Scene* scene, Sound sound);
#define end_loop(sound) (Scene_EndLoop(scene, sound))

PX_EXTERN void SceneAsset_Load(SceneAsset sceneAsset);
PX_EXTERN void Scene_SetTimescale(Scene* scene, float timescale);

// Text

PX_EXTERN void TextRenderer_SetText(Entity entity, const char* str);
PX_EXTERN const char* TextRenderer_GetText(Entity entity);