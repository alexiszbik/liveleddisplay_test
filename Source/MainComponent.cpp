#include "MainComponent.h"

#define HEADER_SIZE 48

#define PIXEL_RATIO 16

#define HARDWARE_WIDTH 64
#define HARDWARE_HEIGHT 16

#define MATRIX_WIDTH PIXEL_RATIO*HARDWARE_WIDTH
#define MATRIX_HEIGHT PIXEL_RATIO*HARDWARE_HEIGHT
/*
const bool res[16][16] = {
    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0},
    {0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,0},
    {1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1}
};*/
const bool res[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0},
    {0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,0},
    {0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

//#include "../liveleddisplay/HearthRes.h"

//==============================================================================
MainComponent::MainComponent()
{
    speed.addListener(this);
    speed.setRange (0, 1);
    
    displayToggle.onClick = [this] {
        isOtherDisplay = !isOtherDisplay;
    };
    
    addAndMakeVisible(speed);
    addAndMakeVisible(displayToggle);
    
    setSize (MATRIX_WIDTH, HEADER_SIZE + MATRIX_HEIGHT);
    setFramesPerSecond(framesPerSecond);
    
    
    unsigned int result[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    for(int y=0;y<=15;y++){
        for(int x=0;x<=15;x++){
            result[y] |= (res[y][x] << x);
        }
        std::cout << result[y];
        std::cout << std::endl;
    }
    
    /*
    std::cout << std::endl;
    
    for(int y=0;y<=15;y++){
        for(int x=0;x<=15;x++){
            std::cout << ((result & (1<<x)) == (1 << x));
            std::cout << std::endl;
        }
    }*/
    
    //std::cout << result;
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::update()
{
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    if (framesPerSecondeChanged) {
        framesPerSecondeChanged = false;
        setFramesPerSecond(framesPerSecond);
        return;
    }
    
    test.tick(true);

    test.showFrame();

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    for (int x = 0; x < HARDWARE_WIDTH; x++) {
        for (int y = 0; y < HARDWARE_HEIGHT; y++) {
            
            auto localRect = juce::Rectangle<int>(x * PIXEL_RATIO, HEADER_SIZE + y * PIXEL_RATIO, PIXEL_RATIO, PIXEL_RATIO);
            
            
            uint16_t color = matrix.buffer[x][y];

            unsigned red = (color & 0xF800) >> 8;
            unsigned green = (color & 0x07E0) >> 3;
            unsigned blue = (color & 0x1F) << 3;
            
            g.fillRect(localRect);
            
            auto c = juce::Colour(red, green, blue);
            
            g.setColour (c);
            
            g.fillRect(localRect);
            
            //Draw grid
            //g.setColour (juce::Colours::grey);
            //g.drawRect(localRect);
            
        }
    }
}

void MainComponent::resized()
{
    speed.setBounds(0, 0, 200, HEADER_SIZE);
    displayToggle.setBounds(speed.getRight(), 0, 100, HEADER_SIZE);
}

void MainComponent::sliderValueChanged (Slider* slider) {
    float value = slider->getValue();
    int newframesPerSecond = (round(value * 39) + 1);
    if (framesPerSecond != newframesPerSecond) {
        framesPerSecondeChanged = true;
        framesPerSecond = newframesPerSecond;
    }
    
}
