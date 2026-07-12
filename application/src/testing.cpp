#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

int main() {
    // Play a WAV file asynchronously
    PlaySoundA("../../assets/sound/Recording.wav", NULL, SND_FILENAME | SND_ASYNC);

    std::cout << "Playing sound...\n";

    // Keep the program alive long enough to hear it
    // (remove this in a real app with its own event loop)
    Sleep(2000);

    return 0;
}