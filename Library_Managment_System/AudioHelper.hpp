#pragma once

#include <SFML/Audio.hpp>
#include <string>

// ============================================================================
// AudioHelper: Sound Effect System & Procedural Audio Trigger
// Handles loan stamps, pickup sounds, error chimes, and theme audio
// ============================================================================

enum class SoundEffect {
    BOOK_PICKUP,
    BOOK_ISSUE,
    PATRON_ANGRY,
    TERMINAL_BEEP,
    SAVE_CONFIRM
};

class AudioHelper {
private:
    static bool soundEnabled;

public:
    static void init();
    static void playSound(SoundEffect effect);
    static void toggleMute();
    static bool isMuted() { return !soundEnabled; }
};
