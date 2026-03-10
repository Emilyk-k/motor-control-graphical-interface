#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <touchgfx/Callback.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
	
	virtual void goToLast() override;
	virtual void updateDisplay();
	
	void buttonSpeedUpClicked();
    void buttonSpeedDownClicked();
	void buttonStartClicked();
	void buttonStopClicked();
	void buttonPUpClicked();
	void buttonPDownClicked();
	void buttonIUpClicked();
	void buttonIDownClicked();
	
	void updateIPDisplay();
	void buttonIPUpClicked();
	void buttonIPDownClicked();
	void buttonIPLeftClicked();
	void buttonIPRightClicked();
	void updateIPBoxPosition();
	void speedSliderChanged(int speed);
	
	#ifndef SIMULATOR
	void handleTickEvent();
	#endif

	
protected:
	Unicode::UnicodeChar speedBuffer[12];
	Unicode::UnicodeChar currentBuffer[12];
	Unicode::UnicodeChar torqueBuffer[12];
	Unicode::UnicodeChar statusBuffer[15];
	Unicode::UnicodeChar PBuffer[12];
	Unicode::UnicodeChar IBuffer[12];
	Unicode::UnicodeChar alertBuffer[50];
	Unicode::UnicodeChar IPBuffer[32];
	
private:
	static const int GRAPH_POINTS = 100;
	float graphBuffer1[GRAPH_POINTS];
	float graphBuffer2[GRAPH_POINTS];

	int currentIPIdx = 0;
	uint32_t tickCounter = 0;
	uint16_t ticks_per_sample = 0;
	int graphIndex = 0;
};

#endif // SCREEN1VIEW_HPP
