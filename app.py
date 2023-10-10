import asyncio
from websockets.server import serve
import cv2
import numpy as np
import pyzbar.pyzbar as pyzbar

prev=""
pres=""

PORT = 8080

async def process_img(data):
    imgnp=np.array(bytearray(data),dtype=np.uint8)
    frame=cv2.imdecode(imgnp,-1)
    return frame

async def get_text(data):
    frame = await process_img(data)
    decodedObjects = pyzbar.decode(frame)
    for obj in decodedObjects:
        pres=obj.data
        if prev == pres:
            pass
        else:
            #prev=pres
            return str(obj.data)

async def echo(websocket):
    try:
        async for message in websocket:
            data = await get_text(message)
            if data != None:
                print(data)
                data = data.replace('b','')
                await websocket.send(f'{data}')
    except:
        pass

async def main():
    print(f"Running on PORT {PORT}")
    async with serve(echo, "0.0.0.0", PORT):
        await asyncio.Future()  # run forever

asyncio.run(main())