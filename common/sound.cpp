#include"sound.hpp"

float GPITCH=1.2f;


float clamp(float val, float minVal, float maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}


void SDLCALL pitchEffect(int chan, void* stream, int len, void* udata) {
	
	GPITCH = clamp(GPITCH, 0.5f, 2.0f); // ou 3.0 max
    // données audio = 16 bits signé
    Sint16* buffer = (Sint16*)stream;
    int samples = len / sizeof(Sint16);

    // on copie l'audio original
    std::vector<Sint16> original(buffer, buffer + samples);

    for (int i = 0; i < samples; i++) {
        float srcIndex = i / GPITCH; // lecture + rapide ou + lente
        if (srcIndex >= samples - 1) srcIndex = samples - 1.001f;
        int i0 = (int)srcIndex;
        int i1 = i0 + 1;
		float frac = srcIndex - i0;
		
		i0 = std::min(i0, samples - 1);
		i1 = std::min(i1, samples - 1);
        // interpolation linéaire
        buffer[i] = (Sint16)((1.0f - frac) * original[i0] + frac * original[i1]);
    }
}

soundSetup::soundSetup()
{
	
}

soundSetup::~soundSetup()
{
	
}

		
		
bool soundSetup::initAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Erreur init audio: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

bool soundSetup::loadSoundWav(const char* filename)
{

    chunk = Mix_LoadWAV(filename);
    if (!chunk) {
        std::cerr << "Erreur chargement WAV: " << Mix_GetError() << std::endl;
        return false;
    }
	
	Mix_VolumeChunk(chunk, 50); // 50% du volume
    int channel = Mix_PlayChannel(-1, chunk, -1); // looping infini
    if (channel == -1) {
        std::cerr << "Erreur PlayChannel : " << Mix_GetError() << std::endl;
        return false;
    }

    if (Mix_RegisterEffect(channel, pitchEffect, nullptr, nullptr) == 0) {
        std::cerr << "Erreur RegisterEffect : " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void soundSetup::loadSoundMus(const char* filename)
{

musique = Mix_LoadMUS(filename);
if (!musique) {
    std::cerr << "Erreur chargement WAV: " << Mix_GetError() << std::endl;
}
}

void soundSetup::playSoundWav()
{
	Mix_PlayChannel(-1, chunk, 0);  // -1 = en boucle
}


void soundSetup::playSoundMus()
{
	Mix_PlayMusic( musique, -1);  // -1 = en boucle
}
