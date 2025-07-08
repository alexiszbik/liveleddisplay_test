#pragma once

#include <JuceHeader.h>
#include "Matrix.h"

#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <chrono>

static void randomSeed(int x) {
}

#define PI 3.141592
#define TWO_PI 2*PI

static long long millis() {
    static auto start_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

       return ms;
}

inline const char* F(const char* thr) {
    return thr;
}

typedef int byte;
#define min std::min<int>
#define random arc4random_uniform

#define A0 0
#define A1 0
#define A2 0

#include "../liveleddisplay/Spiral.h"

#include "../liveleddisplay/Arrows.h"
#include "../liveleddisplay/AutoVu.h"
#include "../liveleddisplay/BigVu.h"
#include "../liveleddisplay/BigYMNK.h"
#include "../liveleddisplay/Circles.h"
#include "../liveleddisplay/Osc.h"
#include "../liveleddisplay/RainDrops.h"
#include "../liveleddisplay/RandomYMNK.h"
#include "../liveleddisplay/RectangleGroup.h"
#include "../liveleddisplay/StarTour.h"
#include "../liveleddisplay/Sticks.h"
#include "../liveleddisplay/SquareDrops.h"
#include "../liveleddisplay/Turnstile.h"
#include "../liveleddisplay/Gfx.h"
#include "../liveleddisplay/Vortex.h"
#include "../liveleddisplay/Intro.h"
#include "../liveleddisplay/Explode.h"
#include "../liveleddisplay/Fire.h"
#include "../liveleddisplay/Plasma.h"

//#include "../liveleddisplay/Sparkles.h"
//#include "../liveleddisplay/Wash.h"

using namespace juce;

class MainComponent  : public AnimatedAppComponent, Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void update() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    
private:
    virtual void sliderValueChanged (Slider* slider) override;

private:
    ToggleButton displayToggle;
    Slider speed;
    
    int tickFreq = 4;
    bool isOtherDisplay = false;

#define BLUE_P new Palette(8, 0, 0, 7)
#define BLUE_P_S new Palette(4, 0, 7, 7)
#define PINKU_P new Palette(8, 7, 0, 7)
#define CYAN new Palette(8, 0, 7, 7)
#define RED_P new Palette(8, 7, 0, 0)
    
    //BigVu test = BigVu(new VuPalette(), 60, 4, BigVu::verticalWide);
    
    //Sparkles test = Sparkles();
    //SquareTrail test = SquareTrail(BLUE_P_S, false);
    //NightSky test = NightSky();
    //Plasma test = Plasma();
    //Arrows test = Arrows();
    //Gfx test = Gfx(new RainbowPalette(), GfxEnum::gfx_smiley, Gfx::EGfxMode::gfxMode_normal, false, false);
    //SquareDrops test = SquareDrops(new RainbowPalette(), SquareDrops::wash);
    //Osc test = Osc(new Palette(COLOR(7,0,0), COLOR(0,7,0), COLOR(0,0,7)));
    //Osc test = Osc(new Palette(COLOR(7,0,0)));
    Spiral test = Spiral();
    
    int framesPerSecond = 12;
    bool framesPerSecondeChanged = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
