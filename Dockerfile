FROM python:3.9
RUN apt-get update
RUN apt-get install ffmpeg libsm6 libxext6  -y && apt-get install libzbar0 -y && pip install pyzbar 

RUN pip install opencv-python
RUN pip install websockets

ADD app.py .

CMD [ "python", "./app.py" ]
