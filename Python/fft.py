import plotly.plotly as py
import plotly.graph_objs as go
import paho.mqtt.client as mqtt
import numpy as np


Fs = 44100
Ts = 1.0/Fs
i = 0

# mag is storing the mqtt messages
mag = []

# For the moment, the only thing I want to do
# is subscribe to the sound messages
def on_connect(client, userdata, flags, rc):
    client.subscribe('sound')


# Messages are received as a string
# format is [a, b, c, d, e, f,]
# the numbers are of type float
# they may have value nan or inf
def on_message(client, userdata, msg):
    temp = str(msg.payload)
    # Start at 3 because mqtt adds b' and I added [
    # end 4 char before end because I added , ] and mqtt adds something?
    temp = temp[3:-4]
    result = temp.split(',')
    
    n = len(result)
    norm = [float(x)/n for x in result]
    final = 20 * np.log10(norm)
    k = np.arange(n)
    T = n/Fs
    frq = k/T

    # This isn a great idea but for the sake of simplicity
    # I'm making a new graph with every result
    # Don run this for a long time or you will have a lot of graphs

    trace = go.Scatter(
                    x = frq,
                    y = final
                    )

    data = [trace]
    layout = go.Layout(
                # Limiting range just to where human voice has most effect
                xaxis = dict(
                            type = 'log',
                            range = [1, 4.3010],
                            title = "Frequency (Hz)",
                            rangemode = "normal",
                            exponentformat = "B",
                            showgrid = True,
                            anchor = "y",
                            ticks = "inside",
                            nticks = 50, 
                            autorange = False,
                            showexponent = "all"
                            ),
                yaxis = dict(
                            title = "Amplitude (dB)",
                            ticks = "inside",
                            showgrid = True,
                            rangemode = "normal",
                            range = [-120, 0],
                            showexponent = "all",
                            exponentformat = "B"
                            )
    )
    fig = go.Figure(data = data, layout = layout)
    py.plot(fig)

# Defining the client and assigning the callback methods defined above
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect('localhost')
client.loop_forever()