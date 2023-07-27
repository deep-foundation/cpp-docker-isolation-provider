FROM python:3.10

RUN pip3 install deepclient==1.0.1

WORKDIR /app

COPY . /app