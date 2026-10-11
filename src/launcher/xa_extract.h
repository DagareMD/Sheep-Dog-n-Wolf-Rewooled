// PSX XA audio + level data extractor (library interface).
//
// Typical GUI use:
//
//   ExtractionSettings settings;
//   settings.binPath = ".../game.bin";
//   settings.outputDir = ".../extracted";          // receives Musics/, Voices/ and Levels/
//   std::thread worker([&] { extractXaAudio(&settings); });
//
//   // UI thread, e.g. on a timer:
//   bar.setValue(settings.progress);
//   label.setText(settings.filesLeft + " files left, writing " + settings.currentFile());
//   if (userPressedCancel) settings.cancelRequested = true;
//   if (settings.finished()) { worker.join(); ... settings.errorMessage() ... }
//
// The settings object must outlive the extraction. All progress fields may be read
// from any thread while the extraction runs.

#pragma once

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <mutex>
#include <string>

enum class ExtractionStatus {
    Idle,        // not started
    Counting,    // decoding without writing, to know the exact number of files
    Extracting,  // writing WAV files
    Done,
    Cancelled,   // cancelRequested was set; files written so far are kept
    Failed,      // see errorMessage()
};

struct ExtractionSettings {
    // ---- input: set before calling extractXaAudio
    std::filesystem::path binPath;    // disc image: .bin (raw 2352-byte sectors) or .iso (2048); the PSX release needs a .bin
    std::filesystem::path outputDir;  // the game folder: the PC release's folders, or the PSX release's converted files
    uint32_t musicGapMs = 1000;       // silence that splits unknown music into tracks (0 = never)
    uint32_t voiceGapMs = 300;        // silence that splits unknown voice audio into clips (0 = never)

    // Optional: receives one line per written file (called on the extraction thread).
    std::function<void(const std::string&)> log;

    // ---- control: may be set from the UI thread while running
    std::atomic<bool> cancelRequested{false};

    // ---- progress: written by the extraction, readable from any thread
    std::atomic<ExtractionStatus> status{ExtractionStatus::Idle};
    std::atomic<float> progress{0.0f};  // 0..1 over the whole job (counting + extracting)
    std::atomic<int> filesTotal{0};     // exact once status reaches Extracting (WAVs + level files)
    std::atomic<int> filesWritten{0};
    std::atomic<int> filesLeft{0};      // filesTotal - filesWritten

    bool finished() const {
        ExtractionStatus s = status;
        return s == ExtractionStatus::Done || s == ExtractionStatus::Cancelled || s == ExtractionStatus::Failed;
    }

    // Path (relative to outputDir) of the file being written, e.g. "Voices/english/sc_01.wav".
    std::string currentFile() const {
        std::lock_guard<std::mutex> lock(textMutex);
        return currentFile_;
    }

    std::string errorMessage() const {
        std::lock_guard<std::mutex> lock(textMutex);
        return error_;
    }

private:
    friend struct ExtractionProgress;
    mutable std::mutex textMutex;
    std::string currentFile_;
    std::string error_;
};

// Installs the game from a disc image into outputDir. The PC release (no .XA files on the disc):
// its game folders copied. The PSX release: every .XA file extracted to WAV, the level data files
// copied (sound banks converted to the PC format) and References/GREET_txt.BSM written.
// Blocks until finished, cancelled or failed; run it on a worker thread.
// Returns true when status is Done.
bool extractXaAudio(ExtractionSettings* settings);
