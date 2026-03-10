#ifndef SCREEN1PRESENTER_HPP
#define SCREEN1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <string>
#include <utility>

using namespace touchgfx;

class Screen1View;

class Screen1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen1Presenter(Screen1View& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Screen1Presenter() {}
	
	void changeSpeed(int speed);
	void changeP(bool p);
	void changeI(bool i);
	void setSpeed(int speed);
    int getSpeed() const { return currentSpeed; }
	int getCurrent() const { return currentCurrent; }
	int getTorque() const { return currentTorque; }
	bool getStatus() const { return currentStatus; }
	int getP() const { return currentP; }
	int getI() const { return currentI; }
	int getAlert() const { return errorCode; }
	void updateStatus(bool status);
	std::string getAlertStr();
	
	int getIPv4Digit(int index) const;
	void setIPv4Digit(int index, int value);

	void key_event() override;

	std::pair<float, float> getXY();

private:
    Screen1Presenter();

    Screen1View& view;
	
	int currentSpeed = 0;
	int currentCurrent = 0;
	int currentTorque = 0;
	bool currentStatus = false;
    void updateMotor(); // send data to motor
	int currentP = 0;
	int currentI = 0;
	int errorCode = 0;
	int IPv4[12] = {1, 9, 2, 1, 6, 8, 1, 0, 0, 1, 0, 0};
};

#endif // SCREEN1PRESENTER_HPP
