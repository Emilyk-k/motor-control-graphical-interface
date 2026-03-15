# motor-control-graphical-interface
Graphical interface for motor control using TouchGFX for STM32F746G_DISCO board using FreeRTOS library.

# Important code:
main:  
Core/Src/main.c  

microcontroller config: 
STM32F746G_DISCO.ioc  

view:  
TouchGFX/gui/src/screen1_screen/Screen1View.cpp  
TouchGFX/gui/include/gui/screen1_screen/Screen1View.hpp  

presenter:  
TouchGFX/gui/src/screen1_screen/Screen1Presenter.cpp  
TouchGFX/gui/include/gui/screen1_screen/Screen1Presenter.hpp  

model:  
TouchGFX/gui/src/model/Model.cpp  
TouchGFX/gui/include/gui/model/Model.hpp  
TouchGFX/gui/include/gui/model/ModelListener.hpp  
