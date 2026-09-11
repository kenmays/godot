#include "audio_driver_haiku.h"

#include "servers/audio/audio_server.h"

void AudioDriverHaiku::_play_buffer(void *cookie, void *buffer, size_t size, const media_raw_audio_format &format) {
	static_cast<AudioDriverHaiku *>(cookie)->_mix(buffer, size, format);
}

void AudioDriverHaiku::_mix(void *buffer, size_t size, const media_raw_audio_format &format) {
	if (!buffer || size == 0) return;
	memset(buffer, 0, size);
	const int bytes_per_sample = sizeof(float);
	const int frames = (int)(size / (bytes_per_sample * channels));
	if (active && frames > 0) {
		audio_server_process(frames, reinterpret_cast<int32_t *>(buffer));
	}
}

Error AudioDriverHaiku::init() {
	mix_rate = _get_configured_mix_rate();
	channels = get_total_channels_by_speaker_mode(speaker_mode);
	media_raw_audio_format format;
	memset(&format, 0, sizeof(format));
	format.format = B_AUDIO_FLOAT;
	format.byte_order = B_MEDIA_LITTLE_ENDIAN;
	format.buffer_size = 0;
	format.frame_rate = mix_rate;
	format.channel_count = channels;
	player = memnew(BSoundPlayer(&format, "Godot Audio", _play_buffer, nullptr, this));
	if (!player || player->InitCheck() != B_OK) {
		if (player) {
			memdelete(player);
			player = nullptr;
		}
		return ERR_CANT_OPEN;
	}
	player->SetHasData(true);
	player->SetVolume(1.0f);
	return OK;
}

void AudioDriverHaiku::start() {
	if (player) {
		active = true;
		player->Start();
	}
}

float AudioDriverHaiku::get_latency() {
	return player ? (float)player->Latency() / 1000000.0f : 0.0f;
}

void AudioDriverHaiku::finish() {
	active = false;
	if (player) {
		player->Stop();
		memdelete(player);
		player = nullptr;
	}
}
