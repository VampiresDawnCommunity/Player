/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_MIDIOUT_COREAUDIO_H
#define EP_MIDIOUT_COREAUDIO_H

#include "audio_midi.h"
#include <AudioToolbox/AudioToolbox.h>

/**
 * Plays MIDI through the Apple's AudioToolbox
 */
class CoreAudioMidiOutDevice : public MidiDecoder {
public:
	CoreAudioMidiOutDevice(std::string& status_message);
	~CoreAudioMidiOutDevice();

	void SendMidiMessage(uint32_t message) override;
	void SendSysExMessage(const uint8_t* data, size_t size) override;
	std::string GetName() override;
	bool NeedsSoftReset() override;
	bool IsInitialized() const;

private:
	AudioUnit midi_out;
	AUGraph graph;
	bool works = false;
};

#endif
