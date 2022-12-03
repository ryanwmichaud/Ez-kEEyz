/*
  ==============================================================================

    MidiProcessor.h
    Created: 3 Dec 2022 12:32:56pm
    Author:  Ryan Michaud

    I moved all the fun processing code to this new class so you can see what I actually did outside of the plugin stuff
    
    I created an instance of this class and when it wants to process an audio block, it calls this class's process function
    and gives it a reference to the midi buffer so we can change it in memory
    
    This class also has a member processedBuffer that is a new buffer with the altered midi. I replace the buffer with 
    this in every block that gets processed.


  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class MidiProcessor {


public:
    void process(juce::MidiBuffer& midiMessages) {       //pass in midi buffer by reference so we can change it in memory

        processedBuffer.clear();
                                                        //processed buffer is a class member so it keeps what we give it outside 
                                                        //the scope of this so we need to clear it



        juce::MidiBuffer::Iterator it(midiMessages);    //use this iterator to iterate through midi events in the block
        juce::MidiMessage currentMessage;
        int samplePos;                                  //midi events can happen at the same sample position 
                                                        //and there can be no events at a sample position so we need to keep track of it


        while (it.getNextEvent(currentMessage, samplePos)) {

            DBG(currentMessage.getDescription());

            if (currentMessage.isNoteOnOrOff()) {                   //only look at notes
                int oldNote = currentMessage.getNoteNumber();       
                int octave = oldNote / 12;
                currentMessage.setNoteNumber(octave*12 + 4);        //round the "nearest" E lol
            }
            DBG(currentMessage.getDescription());
            processedBuffer.addEvent(currentMessage, samplePos);    //add to the processed buffer at the same time as original note
        }

        midiMessages.swapWith(processedBuffer);             //swap reference to midi buffer w processed buffer
    }

    juce::MidiBuffer processedBuffer;  //class member. this is gonna hold our temporary altered notes at any given time before swapping

};
