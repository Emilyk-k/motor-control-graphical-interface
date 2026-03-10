#ifndef MODEL_HPP
#define MODEL_HPP

#include <utility>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

	#ifdef SIMULATOR
	#else
    void LD1ON();
    void LD1OFF();
	std::pair<float, float> getRawADCValues();
	void setDAC(float voltage);
	#endif

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
