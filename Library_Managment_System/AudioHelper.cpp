#include "AudioHelper.hpp"
#include <iostream>

bool AudioHelper::soundEnabled = true;

void AudioHelper::init() {
    soundEnabled = true;
}

void AudioHelper::playSound(SoundEffect effect) {
    if (!soundEnabled) return;

    
    switch (effect) {
        case SoundEffect::BOOK_PICKUP:
            std::cout << "[AUDIO FX]: *Rustle* Book retrieved from shelf.\n";
            break;
        case SoundEffect::BOOK_ISSUE:
            std::cout << "[AUDIO FX]: *Ding!* Checkout counter loan approved.\n";
            break;
        case SoundEffect::PATRON_ANGRY:
            std::cout << "[AUDIO FX]: *Sigh* Patron patience expired.\n";
            break;
        case SoundEffect::TERMINAL_BEEP:
            std::cout << "[AUDIO FX]: *Beep* Catalog search executed.\n";
            break;
        case SoundEffect::SAVE_CONFIRM:
            std::cout << "[AUDIO FX]: *Chime* Game state saved to disk.\n";
            break;
    }
}

void AudioHelper::toggleMute() {
    soundEnabled = !soundEnabled;
    std::cout << "[AUDIO]: Sound effects " << (soundEnabled ? "Enabled" : "Muted") << ".\n";
}
