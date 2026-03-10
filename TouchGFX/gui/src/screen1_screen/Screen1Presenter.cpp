#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <utility>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::changeSpeed(int speed)
{
    if(speed == 1) {
		currentSpeed++;
	} else if(speed == -1) {
		currentSpeed--;
	} else {
		currentSpeed = 0;
	}

    updateMotor(); 
}

void Screen1Presenter::setSpeed(int speed)
{
	currentSpeed = speed;
    updateMotor(); 
}

void Screen1Presenter::changeP(bool p)
{
	if(p) {
		currentP++;
	} else {
		currentP--;
	}

	if(currentP < 0) {
		currentP = 0;
	}
	
    updateMotor(); 
}

void Screen1Presenter::changeI(bool i)
{
	if(i) {
		currentI++;
	} else {
		currentI--;
	}

	if(currentI < 0) {
		currentI = 0;
	}
	
    updateMotor(); 
}

std::string Screen1Presenter::getAlertStr() {
    std::string message;
    int error = errorCode;

    switch (error) {
        case 0:
            message = "Running: system normal.";
            break;
        case 1:
            message = "Unable to reach server.";
            break;
		case 2:
            message = "Warning: high motor speed.";
            break;
        default:
            message = "Unknown error.";
            break;
    }

    if (error == 0 || error == 2)
        return message;
    else
        return "Error " + std::to_string(error) + ": " + message;
}

int Screen1Presenter::getIPv4Digit(int index) const
{
    return (index >= 0 && index < 12) ? IPv4[index] : 0;
}

void Screen1Presenter::setIPv4Digit(int index, int value)
{
    if (index >= 0 && index < 12 && value >= 0 && value <= 9) {
        IPv4[index] = value;
	}
	
	updateMotor(); 
}

void Screen1Presenter::updateMotor()
{
	if(currentSpeed == 0 || errorCode == 1) {
		currentSpeed = 0;
		currentStatus = false;
		currentCurrent = 0;
		currentTorque = 0;
	} else {
		currentStatus = true;
		float ratedSpeed = 1000.0;
		float ratedTorque = 8.0;
		float stallTorque = 15.0;
		float noLoadCurrent = 1.0;
		
		if (currentSpeed <= ratedSpeed) {
			currentTorque = stallTorque - ((stallTorque - ratedTorque) * (currentSpeed / ratedSpeed));
		} else {
			currentTorque = ratedTorque * (ratedSpeed / currentSpeed);
		}
		
		float torqueConstant = 0.5;
		currentCurrent = noLoadCurrent + (currentTorque * torqueConstant);
		
		currentTorque = (currentTorque < 0.0) ? 0.0 : currentTorque;
		currentCurrent = (currentCurrent < noLoadCurrent) ? noLoadCurrent : currentCurrent;
	}

	if(currentSpeed > 800) {
		errorCode = 2;
	} else if(currentSpeed <= 800 && errorCode == 2) {
		errorCode = 0;
	}

	#ifndef SIMULATOR
    if (currentStatus) {
    	model->LD1ON();
    } else {
    	model->LD1OFF();
    }
	#endif

    view.updateDisplay();
}

void Screen1Presenter::key_event()
{
	view.handleKeyEvent(0);
}

#ifndef SIMULATOR
std::pair<float, float> Screen1Presenter::getXY()
{
	return model->getRawADCValues();
}
#endif

