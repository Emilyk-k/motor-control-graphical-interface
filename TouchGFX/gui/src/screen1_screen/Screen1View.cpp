#include <gui/screen1_screen/Screen1View.hpp>
#include <utility>

Screen1View::Screen1View()
{
	int graph_sampling_rate = 100; // Hz
	ticks_per_sample = 1000/graph_sampling_rate; // 1000 Hz declared in freeRTOS
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    graphCurrent1.clear();
    graphCurrent2.clear();

	graphCurrent1.setGraphRangeX(0, GRAPH_POINTS);
	graphCurrent1.setGraphRangeY(-100, 100);
	graphCurrent2.setGraphRangeX(0, GRAPH_POINTS);
	graphCurrent2.setGraphRangeY(-100, 100);
	graphCurrent1.setTouchable(false);
	graphCurrent2.setTouchable(false);

	for (int i = 0; i < GRAPH_POINTS; i++) {
	        graphBuffer1[i] = 0;
	        graphBuffer2[i] = 0;
	}
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::goToLast()
{
    int totalPages = swipeContainerMain.getNumberOfPages();

    swipeContainerMain.setSelectedPage(totalPages - 1);
}

void Screen1View::updateDisplay()
{
	speedSlider.setValue(presenter->getSpeed());
	speedSlider.invalidate();

	Unicode::snprintf(speedBuffer, 12, "%d", presenter->getSpeed()); // Speed
	speedText.setWildcard(speedBuffer);
	speedText.invalidate();
	
	Unicode::snprintf(currentBuffer, 12, "%d", presenter->getCurrent()); // Current
    currentText.setWildcard(currentBuffer);
    currentText.invalidate();
	
	Unicode::snprintf(torqueBuffer, 12, "%d", presenter->getTorque()); // Torque
    torqueText.setWildcard(torqueBuffer);
    torqueText.invalidate();
	
	if (presenter->getStatus()) {
        Unicode::fromUTF8((const uint8_t*)"Running..", statusBuffer, 15); // Status
    } else {
        Unicode::fromUTF8((const uint8_t*)"Stopped", statusBuffer, 15);
    }
    statusText.setWildcard(statusBuffer);
    statusText.invalidate();
	
	Unicode::snprintf(PBuffer, 12, "%d", presenter->getP()); // P
    PGainText.setWildcard(PBuffer);
    PGainText.invalidate();
	
	Unicode::snprintf(IBuffer, 12, "%d", presenter->getI()); // I
    IGainText.setWildcard(IBuffer);
    IGainText.invalidate();
	
	Unicode::fromUTF8((const uint8_t*)presenter->getAlertStr().c_str(), alertBuffer, 50); // Alert/Error Text
	alertText.setWildcard(alertBuffer);
    alertText.invalidate();
	
    int error = presenter->getAlert(); // Alert Image
    if (error != 0)
    {
        imageAlert.setVisible(true);
    }
    else
    {
        imageAlert.setVisible(false);
    }
    imageAlert.invalidate();
}

void Screen1View::updateIPDisplay()
{
    char ipStr[32];
    int ipDigits[12];

    for (int i = 0; i < 12; i++) {
        ipDigits[i] = presenter->getIPv4Digit(i);
    }

    snprintf(ipStr, sizeof(ipStr), "%d%d%d.%d%d%d.%d%d%d.%d%d%d",
             ipDigits[0], ipDigits[1], ipDigits[2],
             ipDigits[3], ipDigits[4], ipDigits[5],
             ipDigits[6], ipDigits[7], ipDigits[8],
             ipDigits[9], ipDigits[10], ipDigits[11]);

    Unicode::fromUTF8((const uint8_t*)ipStr, IPBuffer, 32);
    textIP.setWildcard(IPBuffer);
    textIP.invalidate();
}

void Screen1View::updateIPBoxPosition()
{
    int xOffset = 0;
    switch (currentIPIdx)
    {
        case 0:  xOffset = 151; break;
        case 1:  xOffset = 163; break;
        case 2:  xOffset = 176; break;
        case 3:  xOffset = 197; break;
        case 4:  xOffset = 210; break;
        case 5:  xOffset = 223; break;
        case 6:  xOffset = 243; break;
        case 7:  xOffset = 256; break;
        case 8:  xOffset = 269; break;
        case 9:  xOffset = 289; break;
        case 10: xOffset = 302; break;
        case 11: xOffset = 315; break;
        default: xOffset = 151; break;
    }

    boxIPIdx.setPosition(
        xOffset,
        textIP.getY(),
        boxIPIdx.getWidth(),
        textIP.getHeight()
    );

    boxIPIdx.invalidate();
}



void Screen1View::buttonSpeedUpClicked()
{
    presenter->changeSpeed(1); 
}

void Screen1View::buttonSpeedDownClicked()
{
    presenter->changeSpeed(-1);    
}

void Screen1View::buttonStartClicked()
{
	if(presenter->getSpeed() == 0) {
		presenter->changeSpeed(1);
	}
}

void Screen1View::buttonStopClicked()
{
	presenter->changeSpeed(0);
}

void Screen1View::buttonPUpClicked()
{
    presenter->changeP(true);
}

void Screen1View::buttonPDownClicked()
{
    presenter->changeP(false);            
}

void Screen1View::buttonIUpClicked()
{
    presenter->changeI(true);
}

void Screen1View::buttonIDownClicked()
{
    presenter->changeI(false);            
}

void Screen1View::buttonIPUpClicked()
{
    int value = presenter->getIPv4Digit(currentIPIdx);
    value = (value + 1) % 10; // 9 -> 0
    presenter->setIPv4Digit(currentIPIdx, value);
    updateIPDisplay();
}

void Screen1View::buttonIPDownClicked()
{
    int value = presenter->getIPv4Digit(currentIPIdx);
    value = (value - 1 + 10) % 10;
    presenter->setIPv4Digit(currentIPIdx, value);
    updateIPDisplay();
}

void Screen1View::buttonIPLeftClicked()
{
    currentIPIdx--;
    if (currentIPIdx < 0)
        currentIPIdx = 11;
    updateIPDisplay();
	updateIPBoxPosition();
}

void Screen1View::buttonIPRightClicked()
{
    currentIPIdx++;
    if (currentIPIdx > 11)
        currentIPIdx = 0;
    updateIPDisplay();
	updateIPBoxPosition();
}

void Screen1View::speedSliderChanged(int speed)
{
    presenter->setSpeed(speed);
}

#ifndef SIMULATOR
void Screen1View::handleTickEvent()
{
    tickCounter++;

    if (tickCounter >= ticks_per_sample)
    {
    	tickCounter = 0;

        auto [x, y] = presenter->getXY();

        graphBuffer1[graphIndex] = x;
        graphBuffer2[graphIndex] = y;
        graphCurrent1.clear();
        graphCurrent2.clear();

        for (int i = 0; i < GRAPH_POINTS; i++)
        {
            int idx = (graphIndex + i) % GRAPH_POINTS;
            graphCurrent1.addDataPoint(graphBuffer1[idx]);
            graphCurrent2.addDataPoint(graphBuffer2[idx]);
        }

        graphIndex = (graphIndex + 1) % GRAPH_POINTS;

    }
}
#endif

