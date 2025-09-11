#include"captureSounds.hpp"

template<typename T>
T clamp(T val, T minVal, T maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

captureSound::captureSound()
    : dev(0), audioDataSize(0) {
    memset(&header, 0, sizeof(header));
}

captureSound::~captureSound() {
    stop();
}

bool captureSound::start(const char* filename, int sampleRate, int channels) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Erreur SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

	SDL_AudioSpec desired, obtained;
	SDL_zero(desired);
	
	desired.freq = 44100;
	desired.format = AUDIO_F32LSB; // float 32 bits
	desired.channels = 2;
	desired.samples = 4096;
	
	dev = SDL_OpenAudioDevice(nullptr, SDL_TRUE, &desired, &obtained, 0);
	
    if (!dev) {
        std::cerr << "Erreur ouverture capture audio: " << SDL_GetError() << std::endl;
        return false;
    }

    // Préparer entête WAV (toujours en PCM16)
    memcpy(header.riff, "RIFF", 4);
    memcpy(header.wave, "WAVE", 4);
    memcpy(header.fmt,  "fmt ", 4);
    memcpy(header.data, "data", 4);

    header.subchunk1Size = 16;
    header.audioFormat   = 1; // PCM
    header.numChannels   = obtained.channels;
    header.sampleRate    = obtained.freq;
    header.bitsPerSample = 16; // toujours 16 bits
    header.blockAlign    = header.numChannels * header.bitsPerSample / 8;
    header.byteRate      = header.sampleRate * header.blockAlign;
    header.dataSize      = 0;
    header.chunkSize     = 36;

    file.open(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Erreur ouverture fichier WAV" << std::endl;
        SDL_CloseAudioDevice(dev);
        return false;
    }

    // Écrire entête provisoire
    file.write((char*)&header, sizeof(header));
    audioDataSize = 0;

    SDL_PauseAudioDevice(dev, 0); // démarrer la capture
    return true;
}

void captureSound::update() {
   if (!dev) return;

    float buffer[4096];
    int len = SDL_DequeueAudio(dev, buffer, sizeof(buffer));
    if (len <= 0) return;

    int samples = len / sizeof(float);
    std::vector<int16_t> pcm16(samples);

    float gain = 5.0f; // amplification du son
    for (int i = 0; i < samples; ++i) {
        float f = buffer[i] * gain;
        if (f > 1.0f) f = 1.0f;
        if (f < -1.0f) f = -1.0f;
        pcm16[i] = static_cast<int16_t>(f * 32767.0f);
    }

    file.write(reinterpret_cast<char*>(pcm16.data()), pcm16.size() * sizeof(int16_t));
    audioDataSize += pcm16.size() * sizeof(int16_t);
}

void captureSound::stop() {
     if (!dev) return;
    SDL_CloseAudioDevice(dev);
    dev = 0;

    header.dataSize = audioDataSize;
    header.chunkSize = 36 + audioDataSize;

    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    file.close();
}

void captureSound::captureSegment(int numSamples) {
    if (!dev) return;
    int len = numSamples * sizeof(float) * header.numChannels;
    if (audioBuffer.size() < numSamples * header.numChannels)
    audioBuffer.resize(numSamples * header.numChannels);
    
    int dequeued = SDL_DequeueAudio(dev, audioBuffer.data(), numSamples * sizeof(float) * header.numChannels);

    if (dequeued <= 0) return;

    int numSamplesDequeued = dequeued / (sizeof(float) * header.numChannels);

    // --- Place ici le calcul du gain automatique ---
    float maxVal = 0.0f;
    for (int i = 0; i < numSamplesDequeued * header.numChannels; ++i)
        maxVal = std::max(maxVal, std::abs(audioBuffer[i]));

    float gain = (maxVal > 0.0f) ? (0.95f / maxVal) : 1.0f;

    // Convertir float -> PCM16 avec gain
    std::vector<int16_t> pcm16(numSamplesDequeued * header.numChannels);
    for (int i = 0; i < pcm16.size(); ++i) {
        float f = audioBuffer[i] * gain;
        pcm16[i] = static_cast<int16_t>(clamp(f, -1.0f, 1.0f) * 32767);
    }


    file.write(reinterpret_cast<char*>(pcm16.data()), pcm16.size() * sizeof(int16_t));
    audioDataSize += pcm16.size() * sizeof(int16_t);
}

