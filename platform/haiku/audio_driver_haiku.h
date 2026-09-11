#pragma once

#include <SoundPlayer.h>

#include "core/os/mutex.h"
#include "servers/audio/audio_driver.h"

class AudioDriverHaiku : public AudioDriver {
	BSoundPlayer *player = nullptr;
	Mutex mutex;
	int mix_rate = 44100;
	SpeakerMode speaker_mode = SPEAKER_MODE_STEREO;
	int channels = 2;
	bool active = false;

	static void _play_buffer(void *cookie, void *buffer, size_t size, const media_raw_audio_format &format);
	void _mix(void *buffer, size_t size, const media_raw_audio_format &format);

public:
	const char *get_name() const override { return "Haiku Media Kit"; }
	Error init() override;
	void start() override;
	int get_mix_rate() const override { return mix_rate; }
	SpeakerMode get_speaker_mode() const override { return speaker_mode; }
	float get_latency() override;
	void lock() override { mutex.lock(); }
	void unlock() override { mutex.unlock(); }
	void finish() override;

	AudioDriverHaiku() = default;
	~AudioDriverHaiku() override { finish(); }
};
