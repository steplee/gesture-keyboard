#/usr/bin/python3

PORT = 9002

msgs = ['{"type":"begin"}', '{"type":"move","time":7.996,"x":2.3775,"y":1.8733333333333333}', '{"type":"move","time":8.335,"x":0.7425,"y":0.48}', '{"type":"move","time":8.684,"x":0.6375,"y":0.39}', '{"type":"move","time":9.019,"x":0.61875,"y":0.24}', '{"type":"move","time":9.369,"x":0.535,"y":0.13}', '{"type":"move","time":9.718,"x":0.4075,"y":0.10333333333333333}', '{"type":"move","time":10.068,"x":0.3175,"y":0.20333333333333334}', '{"type":"move","time":10.418,"x":0.2825,"y":0.3566666666666667}', '{"type":"move","time":10.768,"x":0.2825,"y":0.5166666666666667}', '{"type":"move","time":11.118,"x":0.32875,"y":0.64}', '{"type":"move","time":11.452,"x":0.40625,"y":0.68}', '{"type":"move","time":11.801,"x":0.49375,"y":0.68}', '{"type":"move","time":12.152,"x":0.59375,"y":0.6466666666666666}', '{"type":"move","time":12.501,"x":0.6525,"y":0.6133333333333333}', '{"type":"move","time":12.842,"x":0.67375,"y":0.5933333333333334}', '{"type":"move","time":13.185,"x":0.7275,"y":0.37666666666666665}', '{"type":"move","time":13.537,"x":0.785,"y":0.24333333333333335}','{"type":"move","time":13.885,"x":0.80375,"y":0.10666666666666667}', '{"type":"move","time":14.235,"x":0.77375,"y":0.05}', '{"type":"move","time":14.585,"x":0.7025,"y":0.056666666666666664}', '{"type":"end"}', '{"type":"begin"}', '{"type":"begin"}', '{"type":"move","time":17.343,"x":0.69,"y":0.056666666666666664}', '{"type":"move","time":18.366,"x":0.6925,"y":0.056666666666666664}', '{"type":"move","time":18.701,"x":1.38875,"y":0.2833333333333333}']


import websocket
try:
    import thread
except ImportError:
    import _thread as thread
import time

def on_message(ws, message):
    print(message)

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    for i,m in enumerate(msgs):
        ws.send(m)
        time.sleep(.2)

    ws.close()


if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://localhost:{}/".format(PORT),
            on_message = on_message,
            on_error = on_error,
            on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
