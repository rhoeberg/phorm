#define VOICE_AMOUNT 8
#define SAMPLE_RATE (44100)

enum AudioStatus
{
    AUDIO_PLAYING,
    AUDIO_FREE
};

struct AudioVoice
{
    short *buffer;
    int pos;
    int length;
    AudioStatus status;
};

struct AudioMixer
{
    AudioVoice voices[VOICE_AMOUNT];
};

struct paData
{
    short *buffer;
    int sampleAmount;
    int playbackIndex;
    int channelCount;
};

struct AudioState
{
	PaStream *stream;
	short *buffer;
	int sampleAmount;

	// static paData data;
	// static int sampleRate;
	AudioMixer mixer;
};

/* PaStream *stream; */
/* static short *buffer; */
/* static int sampleAmount; */
/* // static paData data; */
/* // static int sampleRate; */
/* static AudioMixer mixer; */

bool audioInitialize();
bool audioClose();
void shutdownAudio();
void playAudio(AudioMixer *mixer, int n);


global AudioState *_audioState;
