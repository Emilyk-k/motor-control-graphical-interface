#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <cstdint>
#include <utility>
#include <cstdio>

#ifndef SIMULATOR
#include "main.h"
#include "cmsis_os.h"
extern osMessageQueueId_t sensor_data_channel_1Handle;
extern osMessageQueueId_t sensor_data_channel_2Handle;
#endif

Model::Model() : modelListener(0)
{
}

#ifdef SIMULATOR
void Model::tick() {}
#endif

#ifndef SIMULATOR
uint8_t b1=GPIO_PIN_RESET, b1_prev=GPIO_PIN_RESET;

void Model::tick()
{
	b1 = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
	if(b1 != b1_prev) {
		modelListener->key_event();
	}
	b1_prev = b1;
}

void Model::LD1ON()
{
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
}

void Model::LD1OFF()
{
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
}

std::pair<float, float> Model::getRawADCValues()
{
    float sample_in0 = 0.0f, sample_in8 = 0.0f;

    uint32_t count0 = osMessageQueueGetCount(sensor_data_channel_1Handle);
    uint32_t count8 = osMessageQueueGetCount(sensor_data_channel_2Handle);

    for (uint32_t i = 0; i < count0; i++) {
        osMessageQueueGet(sensor_data_channel_1Handle, &sample_in0, NULL, 0);
    }

    for (uint32_t i = 0; i < count8; i++) {
        osMessageQueueGet(sensor_data_channel_2Handle, &sample_in8, NULL, 0);
    }

    static float last_sample0 = 0.0f, last_sample8 = 0.0f;
    if (count0 > 0) last_sample0 = sample_in0;
    if (count8 > 0) last_sample8 = sample_in8;

    return {last_sample0, last_sample8};
}
#endif

