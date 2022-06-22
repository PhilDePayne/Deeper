//
// Created by Wojtek on 25.05.2022.
//

#ifndef DEEPER_LOGMACROS_H
#define DEEPER_LOGMACROS_H


#define LOGS_ENABLED // comment this macro if you want to disable all logs
//#define FREETYPE_ENABLED
#define GAME_OVER_ENABLED

#ifdef LOGS_ENABLED // check if logs are enabled

#ifndef NDEBUG // check if is DEBUG build

#define DEEPER_MODEL_CLASS_LOGS
//#define DEEPER_MESH_CLASS_LOGS
//#define DEEPER_GAMEOBJECT_CLASS_LOGS
//#define DEEPER_LIGHTSOURCE_CLASS_LOGS
//#define DEEPER_TRANSFORM_CLASS_LOGS
//#define DEEPER_COMPONENT_CLASS_LOGS
//#define DEEPER_PLAINMESH_CLASS_LOGS
//#define DEEPER_SPHEREMESH_CLASS_LOGS
//#define DEEPER_SPHERECOLLIDER_CLASS_LOGS
//#define DEEPER_CUBEMESH_CLASS_LOGS
//#define DEEPER_BOXCOLLIDER_CLASS_LOGS
//#define DEEPER_AI_CLASS_LOGS
//#define DEEPER_LAMPAI_CLASS_LOGS
//#define DEEPER_SOUNDSYSTEM_CLASS_LOGS
//#define DEEPER_SOUND_CLASS_LOGS
//#define DEEPER_LEVELGENERATOR_LOGS



#endif

#endif


#endif //DEEPER_LOGMACROS_H
