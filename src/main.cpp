#include <fftw3.h>                      // For performing fft
#include "Portaudio.hpp"
#include <ctime>
#include "lib.hpp"
#include "Message.hpp"
#include "MQTT.hpp"

int main()
{
    int fft_size    = 8192;
    int sample_rate = 44100;
    int fft_result  = (fft_size) / 2;
    int qos         = 0;


    // Initialize the mosquitto client that will be used
    MQTT mqtt("52.213.208.76", "Bot1");

    // data is going to be where audio data is stored
    // data will be the input to fft
    // out will be the output
    float         data[fft_size];
    fftwf_complex out[fft_size / 2];
    float         message[fft_size/2];

    // Initialize PortAudio
    Portaudio PA;
    PA.open_stream(fft_size, sample_rate, paClipOff);
    
    int   pt = 0;
    float lat = 0; // Latency variable
    while (true)
    {
        int t = time(nullptr);
        if (t - pt > 4 && t % 5 == 0)
        {    
            pt = t;
	    PA.start_stream();
            // Create the fftw plan
            fftwf_plan plan = fftwf_plan_dft_r2c_1d(fft_size, data, out, FFTW_ESTIMATE);        
            
            // Pa_ReadStream is a blocking call to take in mic input
            PA.read_stream(data, fft_size);
            fftwf_execute(plan);
	    PA.stop_stream();
            // Here, I prepare the message that will be sent over MQTT
            Message m("Nimbus/Bot/1/Audio", out, fft_result, t, lat);
            
            clock_t time_before_publish = clock();
            mqtt.publish_message(m, "Nimbus/Bot/1/Audio", qos);
            clock_t time_after_publish = clock();
            
            clock_t diff = time_after_publish - time_before_publish;
            lat = ((float)diff)/CLOCKS_PER_SEC;

            Pa_Sleep(3000);
        }

        else
        {
            Pa_Sleep(1000);
        }
    }
    return 0;
}
