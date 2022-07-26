/*
    Copyright (C) 2020, 2021 by Grégoire Locqueville <gregoireloc@gmail.com>

    This file is part of Amati.

    Amati is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Amati is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Amati.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <JuceHeader.h>

#include <faust/dsp/dsp.h>
#include <faust/gui/APIUI.h>

// Wrapper around Faust-related stuff, namely:
// - a Faust program
// - the associated user interface
class FaustProgram
{

public:
  class CompileError : public std::runtime_error
  {
  public:
    explicit CompileError (const char* message) : std::runtime_error (message) {}
    explicit CompileError (const std::string& message) : std::runtime_error (message) {}
    explicit CompileError (const juce::String& message) : CompileError (message.toStdString()) {}
  };

  enum class ItemType {
      // Unavailabe is for when we don't/can't include the UI element;
      Unavailable,
      Slider,
      Button,
      CheckButton,
    };

  enum class Backend {
    LLVM,
    Interpreter,
  };

  /// Construct a Faust Program.
  /// @throws CompileError
  FaustProgram (juce::String source, Backend, int sampRate);
  ~FaustProgram ();

    size_t getParamCount ();
    int getNumInChannels ();
    int getNumOutChannels ();

    // Getters for values associated with a parameter
    // (UI element type; minimum, maximum and initial value)
    // Argument has to be less than parameter count!
    ItemType getType (size_t);
    double getMin (size_t);
    double getMax (size_t);
    double getInit (size_t);

    float getValue (size_t);
    void setValue (size_t, float);
    juce::String getLabel(size_t idx);

    void compute(int sampleCount, const float** input, float** output);

private:
  class DspFactory;
  void compileSource(juce::String);

  Backend backend;

  dsp_factory* dspFactory;
  std::unique_ptr<dsp> dspInstance;
  std::unique_ptr<APIUI> faustInterface;

  int sampleRate;
};
